
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atRenderState.h"
#include "atGraphics.h"
#include "atImGui.h"
#include "atImage.h"
#include "atHardwareTexture.h"
#include "atShaderPool.h"
#include "atInput.h"
#include <time.h>

struct VERTEX_CONSTANT_BUFFER
{
  float   mvp[4][4];
};

static const char* _vertexShader =
"cbuffer vertexBuffer : register(b0) \
            {\
            float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
            float2 pos : POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
            PS_INPUT output;\
            output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
            output.col = input.col;\
            output.uv  = input.uv;\
            return output;\
            }";

static const char* _pixelShader =
"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

static int64_t _shaderID = AT_INVALID_ID;
static int64_t _inputLayoutID = AT_INVALID_ID;
static int64_t _fontSamplerID = AT_INVALID_ID;
static int64_t _vbSize = AT_INVALID_ID;
static int64_t _ibSize = AT_INVALID_ID;
static int64_t _lastTime = -1;
static int64_t _ticksPerSecond = -1;
static ImGuiMouseCursor _lastMouseCursor = -1;

static ID3D11Buffer *_pVertexBuffer = nullptr;
static ID3D11Buffer *_pIndexBuffer = nullptr;

static bool _initialised = false;

static bool _UpdateMouseCursor()
{
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
    return false;

  ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
  if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
  {
    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
    SetCursor(NULL);
  }
  else
  {
    // Show OS mouse cursor
    LPTSTR win32_cursor = IDC_ARROW;
    switch (imgui_cursor)
    {
    case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
    case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
    case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
    case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
    case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
    case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
    case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
    case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
    }
    SetCursor(LoadCursor(NULL, win32_cursor));
  }
  return true;
}

static bool _Initialise()
{
  if (_initialised)
    return true;

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  uint8_t *pPixels = nullptr;
  int32_t width, height;
  io.Fonts->GetTexDataAsRGBA32(&pPixels, &width, &height);
  io.Fonts->TexID = (ImTextureID)atHardwareTexture::UploadTexture(atImage(pPixels, atVec2I(width, height), 4));
  atShaderPool::ReleaseShader(_shaderID);
  _shaderID = atShaderPool::GetShader(_pixelShader, _vertexShader);
  atHardwareTexture::DeleteSampler(_fontSamplerID);
  _fontSamplerID = atHardwareTexture::CreateSampler(21, atTCM_Wrap, atTCM_Wrap, atTCM_Wrap, 0, atComp_Always, { 0,0,0,0 }, 0.0, 0.0);
  _inputLayoutID = atShaderPool::GetInputLayout(_shaderID, { {"POSITION", atGetTypeDesc<atVec2F>() }, {"TEXCOORD", atGetTypeDesc<atVec2F>() }, {"COLOR", atGetTypeDesc<uint32_t>()} });

  if (!::QueryPerformanceFrequency((LARGE_INTEGER *)&_ticksPerSecond))
    return false;
  if (!::QueryPerformanceCounter((LARGE_INTEGER *)&_lastTime))
    return false;


  io.KeyMap[ImGuiKey_Tab] = atKC_Tab;
  io.KeyMap[ImGuiKey_LeftArrow] = atKC_Left;
  io.KeyMap[ImGuiKey_RightArrow] = atKC_Right;
  io.KeyMap[ImGuiKey_UpArrow] = atKC_Up;
  io.KeyMap[ImGuiKey_DownArrow] = atKC_Down;
  io.KeyMap[ImGuiKey_PageUp] = atKC_PageUp;
  io.KeyMap[ImGuiKey_PageDown] = atKC_PageDown;
  io.KeyMap[ImGuiKey_Home] = atKC_Home;
  io.KeyMap[ImGuiKey_End] = atKC_End;
  io.KeyMap[ImGuiKey_Insert] = atKC_Insert;
  io.KeyMap[ImGuiKey_Delete] = atKC_Delete;
  io.KeyMap[ImGuiKey_Backspace] = atKC_Backspace;
  io.KeyMap[ImGuiKey_Space] = atKC_Space;
  io.KeyMap[ImGuiKey_Enter] = atKC_Return;
  io.KeyMap[ImGuiKey_Escape] = atKC_Escape;
  _initialised = true;
  return true;
}

static bool _UpdateBuffers(ImDrawData *pDrawData)
{
  if ((!_pVertexBuffer || _vbSize < pDrawData->TotalVtxCount) && pDrawData->TotalVtxCount > 0)
  {
    atGraphics::CreateBuffer(&_pVertexBuffer, nullptr, pDrawData->TotalVtxCount * sizeof(ImDrawVert), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    _vbSize = pDrawData->TotalVtxCount;
  }
  if ((!_pVertexBuffer || _vbSize < pDrawData->TotalIdxCount) && pDrawData->TotalIdxCount > 0)
  {
    atGraphics::CreateBuffer(&_pIndexBuffer, nullptr, pDrawData->TotalIdxCount * sizeof(ImDrawIdx), D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    _ibSize = pDrawData->TotalIdxCount;
  }

  if (!_pVertexBuffer || !_pIndexBuffer)
    return false;

  D3D11_MAPPED_SUBRESOURCE vtxResource, idxResource;
  if (FAILED(atGraphics::GetContext()->Map(_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtxResource)))
    return false;
  if (FAILED(atGraphics::GetContext()->Map(_pIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idxResource)))
    return false;

  ImDrawVert *pVtxDst = (ImDrawVert*)vtxResource.pData;
  ImDrawIdx *pIdxDst = (ImDrawIdx*)idxResource.pData;
  for (int64_t i = 0; i < pDrawData->CmdListsCount; ++i)
  {
    const ImDrawList* pCmdList = pDrawData->CmdLists[i];
    memcpy(pVtxDst, pCmdList->VtxBuffer.Data, pCmdList->VtxBuffer.Size * sizeof(ImDrawVert));
    memcpy(pIdxDst, pCmdList->IdxBuffer.Data, pCmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
    pVtxDst += pCmdList->VtxBuffer.Size;
    pIdxDst += pCmdList->IdxBuffer.Size;
  }
  atGraphics::GetContext()->Unmap(_pVertexBuffer, 0);
  atGraphics::GetContext()->Unmap(_pIndexBuffer, 0);

  {
    float L = pDrawData->DisplayPos.x;
    float T = pDrawData->DisplayPos.y;
    atMat4F ortho = atMatrixOrtho(L, L + pDrawData->DisplaySize.x, T, T + pDrawData->DisplaySize.y, -0.5f, 0.5f);
    atShaderPool::SetVariable(_shaderID, "ProjectionMatrix", &ortho.m_data, sizeof(ortho.m_data));
  }
  return true;
}

bool atImGui::BeginFrame(atWindow &wnd)
{
  _Initialise();
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = wnd.Size();
  io.MousePos = atInput::MousePos();

  int64_t curTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  io.DeltaTime = (float)(curTime - _lastTime) / (float)_ticksPerSecond;
  _lastTime = curTime;

  ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
  if (_lastMouseCursor != mouse_cursor)
  {
    _lastMouseCursor = mouse_cursor;
    _UpdateMouseCursor();
  }

  ImGui::NewFrame();
  return true;
}

bool atImGui::EndFrame() { ImGui::EndFrame(); return true; }

bool atImGui::Render()
{
  ImGui::Render();
  ImDrawData *pDrawData = ImGui::GetDrawData();
  if (!_UpdateBuffers(pDrawData))
    return false;

  atRenderState rs;
  rs.SetBlendEnabled(true);
  rs.SetDepthReadEnabled(false);
  rs.SetDepthWriteEnabled(false);
  rs.SetScissor(true);
  rs.SetStencilEnabled(false);
  rs.SetViewport(atVec4I(pDrawData->DisplayPos.x, pDrawData->DisplayPos.y, pDrawData->DisplaySize.x, pDrawData->DisplaySize.y));
  rs.SetShader(_shaderID, _inputLayoutID);

  unsigned int stride = sizeof(ImDrawVert);
  unsigned int offset = 0;
  atGraphics::GetContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
  atGraphics::GetContext()->IASetIndexBuffer(_pIndexBuffer, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
  atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  atGraphics::BindShaderResource(atST_Pixel, atSRT_Sampler, 0, atHardwareTexture::GetSampler(_fontSamplerID));

  int vtxOffset = 0;
  int idxOffset = 0;
  ImVec2 pos = pDrawData->DisplayPos;
  for (int64_t i = 0; i < pDrawData->CmdListsCount; i++)
  {
    const ImDrawList* pCmdList = pDrawData->CmdLists[(int)i];
    for (int64_t cmdIndex = 0; cmdIndex < pCmdList->CmdBuffer.Size; cmdIndex++)
    {
      const ImDrawCmd* pCmd = &pCmdList->CmdBuffer[(int)cmdIndex];
      if (pCmd->UserCallback)
        pCmd->UserCallback(pCmdList, pCmd);
      else
      {
        // Apply scissor/clipping rectangle
        rs.SetScissor({ (LONG)(pCmd->ClipRect.x - pos.x), (LONG)(pCmd->ClipRect.y - pos.y), (LONG)(pCmd->ClipRect.z - pos.x), (LONG)(pCmd->ClipRect.w - pos.y) });

        // Bind texture, Draw;
        atGraphics::BindShaderResource(atST_Pixel, atSRT_Texture, 0, atHardwareTexture::GetTexture((int64_t)pCmd->TextureId));
        atGraphics::DrawIndexed(pCmd->ElemCount, idxOffset, vtxOffset);
      }
      idxOffset += pCmd->ElemCount;
    }
    vtxOffset += pCmdList->VtxBuffer.Size;
  }
  return true;
}

bool atImGui::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (!_initialised)
    return false;

  ImGuiIO &io = ImGui::GetIO();
  switch (msg)
  {
  case WM_SYSKEYDOWN: case WM_KEYDOWN: io.KeysDown[wParam] = true; break;
  case WM_SYSKEYUP: case WM_KEYUP: io.KeysDown[wParam] = false; break;
  case WM_LBUTTONUP: io.MouseDown[atMB_Left] = false; break;
  case WM_RBUTTONUP: io.MouseDown[atMB_Right] = false; break;
  case WM_MBUTTONUP: io.MouseDown[atMB_Middle] = false; break;
  case WM_LBUTTONDOWN: io.MouseDown[atMB_Left] = true; break;
  case WM_RBUTTONDOWN: io.MouseDown[atMB_Right] = true; break;
  case WM_MBUTTONDOWN: io.MouseDown[atMB_Middle] = true; break;
  case WM_MOUSEWHEEL: io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA; break;
  case WM_MOUSEHWHEEL: io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA; break;
  case WM_CHAR:
    if (wParam > 0 && wParam < 0x10000)
      io.AddInputCharacter((unsigned short)wParam);
    break;
  default: return false;
  }
  io.KeyAlt = io.KeysDown[atKC_LAlt] | io.KeysDown[atKC_RAlt];
  io.KeyCtrl = io.KeysDown[atKC_LControl] | io.KeysDown[atKC_RControl];
  io.KeyShift = io.KeysDown[atKC_LShift] | io.KeysDown[atKC_RShift];
  io.KeySuper = false;
  return io.WantCaptureKeyboard || io.WantCaptureMouse;
}
