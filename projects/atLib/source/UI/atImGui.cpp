
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
#include "atScan.h"

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

bool atImGui::BeginFrame(atWindow *pWnd)
{
  _Initialise();
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = pWnd->Size();
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
  rs.SetScissorEnabled(true);
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

// Wrapping ImGui Functions (Useful for exposing functionality to Lua)

bool atImGui::Begin(const char *name) { return ImGui::Begin(name); }

bool atImGui::Begin(const char *name, const atVec2D &size)
{
  ImGui::SetNextWindowSize(size);
  return Begin(name);
}

bool atImGui::Begin(const char *name, const atVec2D &size, const atVec2D &pos)
{
  ImGui::SetNextWindowPos(pos);
  return Begin(name, size);
}


bool atImGui::Button(const char *label, const atVec2D &size, const atVec2D &pos)
{
  ImVec2 lastCursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(pos);
  bool result = Button(label, size);
  ImGui::SetCursorPos(lastCursorPos);
  return result;
}

bool atImGui::Selectable(const char *label, const bool selected, const atVec2D &pos)
{
  ImVec2 lastCursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(pos);
  bool result = Selectable(label, selected);
  ImGui::SetCursorPos(lastCursorPos);
  return result;
}

bool atImGui::BeginTreeNode(const char *label) { return ImGui::TreeNode(label); }

void atImGui::EndTreeNode() { ImGui::TreePop(); }

bool atImGui::BeginChild(const char *id, const atVec2D &size, const bool border) { return ImGui::BeginChild(id, size, border); }

bool atImGui::BeginChild(const char *id, const atVec2D &size) { return BeginChild(id, size, false); }

bool atImGui::BeginChild(const char *id) { return BeginChild(id, { 0, 0 }, false); }

void atImGui::EndChild() { ImGui::EndChild(); }

void atImGui::NewLine() { ImGui::NewLine(); }

void atImGui::SameLine() { ImGui::SameLine(); }

void atImGui::PushID(const char *label) { ImGui::PushID(label); }

void atImGui::PushID(const int64_t id) { ImGui::PushID((int)id); }

void atImGui::PopID() { ImGui::PopID(); }

void atImGui::End() { ImGui::End(); }

void atImGui::Text(const char *text) { ImGui::Text(text); }

bool atImGui::Button(const char *label) { return ImGui::Button(label); }

bool atImGui::Button(const char *label, const atVec2D &size) { return ImGui::Button(label, size); }

bool atImGui::Selectable(const char *label, const bool selected) { return ImGui::Selectable(label, selected); }

// Helpers to push a large collection of styles in one go

struct _atImGuiColours
{
  int64_t colID = -1;
  atVec4D colour = atVec4D::one();
};

struct _atImGuiStyles
{
  int64_t varID = -1;
  atVec2D vec2 = 0;
  int64_t nVals = 0;
};

static atVector<int64_t> _stylePopCount;
static atVector<int64_t> _styleColourPopCount;
static atHashMap<atString, int64_t> _styleLookup;
static atHashMap<atString, int64_t> _styleColourLookup;
static atHashMap<atString, atVector<_atImGuiColours>> _coloursCache;
static atHashMap<atString, atVector<_atImGuiStyles>> _stylesCache;

// ImGui provides a handy function to get the style colour names
// so we can generate a string to int hash map of colour id's quite easily

static int64_t _GetColourID(const atString &colour)
{
  // Generate style colour name to ID lookup
  if (_styleColourLookup.Size() == 0)
    for (int64_t id = 0; id < (int64_t)ImGuiCol_COUNT; ++id)
      _styleColourLookup.Add(atString(ImGui::GetStyleColorName((ImGuiCol)id)).to_lower(), id);
  atString lowerStr = colour.to_lower();
  int64_t *pID =_styleColourLookup.TryGet(lowerStr);
  return pID ? *pID : -1;
}

static int64_t _GetStyleID(const atString &style)
{
  // Generate style colour name to ID lookup
  if (_styleLookup.Size() == 0)
    for (int64_t id = 0; id < (int64_t)ImGuiStyleVar_COUNT; ++id)
      _styleLookup.Add(atString(ImGui::GetStyleVarName((ImGuiCol)id)).to_lower(), id);
  atString lowerStr = style.to_lower();
  int64_t *pID = _styleLookup.TryGet(lowerStr);
  return pID ? *pID : -1;
}

static _atImGuiStyles _ParseStyleString(const atString &str)
{
  static atVector<atString> args;
  args.clear();

  // remove whitespace and split string
  args = str.replace("}", "").replace(" ", "").replace("\n", "").replace("\r", "").replace("\t", "").split('{');

  _atImGuiStyles ret;

  if (args.size() == 2)
  {
    int64_t styleID = _GetStyleID(args[0]);
    if (styleID >= 0)
    {
      atVector<atString> styleVals = args[1].split(',');
      const ImGuiStyleVarInfo *pInfo = ImGui::GetStyleVarInfo((ImGuiStyleVar)styleID);

      if (styleVals.size() == pInfo->Count)
      {
        for (int64_t i = 0; i < styleVals.size(); ++i)
          ret.vec2[i] = atScan::Float(styleVals[0]);
        ret.varID = styleID;
        ret.nVals = pInfo->Count;
        return ret;
      }
      else
      {
        printf("atImGui Warning - Parsing style string:\n\t%d values needed for a valid style. %lld provided.\n", pInfo->Count, styleVals.size());
      }
    }
    else
    {
      printf("atImGui Warning - Parsing style string:\n\t'%s' is not a valid ImGui style id.\n", args[0].c_str());
    }
  }
  else
  {
    printf("atImGui Warning - Parsing style string:\n\tNot enough arguments in '%s'\n", str.c_str());
  }

  return _atImGuiStyles();
}

static _atImGuiColours _ParseColourString(const atString &str)
{
  static atVector<atString> args;
  args.clear();
  args = str.replace("}", "").replace(" ", "").replace("\n", "").replace("\r", "").replace("\t", "").split('{');

  _atImGuiColours ret;

  if (args.size() == 2)
  {
    int64_t colID = _GetColourID(args[0]);
    if (colID >= 0)
    {
      atVector<atString> colVals = args[1].split(',');
      if (colVals.size() == 4 || colVals.size() == 3)
      {
        for (int64_t i = 0; i < colVals.size(); ++i)
          ret.colour[i] = atScan::Float(colVals[i]);
        ret.colID = colID;
        return ret;
      }
      else
      {
        printf("atImGui Warning - Parsing style colour string:\n\t3 or 4 values needed for a valid colour. %lld where provided", colVals.size());
      }
    }
    else
    {
      printf("atImGui Warning - Parsing style colour string:\n\t'%s' is not a valid ImGui colour id.", args[0].c_str());
    }
  }
  else
  {
    printf("atImGui Warning - Parsing style colour string: Not enough arguments in '%s'", str.c_str());
  }

  return _atImGuiColours();
}

void atImGui::AddColours(const char *id, const char *colourString)
{
  RemoveColours(id);
  _coloursCache.TryAdd(id, atVector<_atImGuiColours>());
  atVector<_atImGuiColours> &col = _coloursCache.Get(id);
  col.clear();

  atVector<atString> tokens = atString(colourString).split(';');
  for (const atString &colString : tokens)
  {
    _atImGuiColours colDef = _ParseColourString(colString);
    if(colDef.colID >= 0)
      col.push_back(colDef);
  }
}

void atImGui::AddStyle(const char *id, const char *styleString)
{
  RemoveStyle(id);
  _stylesCache.TryAdd(id, atVector<_atImGuiStyles>());
  atVector<_atImGuiStyles> &style = _stylesCache.Get(id);
  style.clear();

  atVector<atString> tokens = atString(styleString).split(';');
  for (const atString &colString : tokens)
  {
    _atImGuiStyles styleDef = _ParseStyleString(colString);
    if (styleDef.varID >= 0)
      style.push_back(styleDef);
  }
}

void atImGui::RemoveColours(const char *id) { _coloursCache.Remove(id); }

void atImGui::RemoveStyle(const char *id) { _stylesCache.Remove(id); }

void atImGui::PushColour(const char *colourName, const atVec4D &colour)
{
  int64_t id = _GetColourID(colourName);
  if (id < 0)
  {
    printf("atImGui Warning - Push Colour: Invalid Colour ID '%s'", colourName);
    return;
  }

  _styleColourPopCount.push_back(1);
  ImGui::PushStyleColor((ImGuiCol)id, colour);
}

void atImGui::PushStyle(const char *styleName, const atVec2D &val)
{
  int64_t id = _GetStyleID(styleName);
  if (id < 0)
  {
    printf("atImGui Warning - Push Style: Invalid Style ID '%s'", styleName);
    return;
  }

  if (ImGui::GetStyleVarInfo((ImGuiStyleVar)id)->Count != 2)
  {
    printf("atImGui Warning - Push Style: '%s' expects a double. A Vec2 was passed.", styleName);
    return;
  }

  ImGui::PushStyleVar((ImGuiStyleVar)id, val);
  _stylePopCount.push_back(1);
}

void atImGui::PushStyle(const char *styleName, const double val)
{
  int64_t id = _GetStyleID(styleName);
  if (id < 0)
  {
    printf("atImGui Warning - Push Style: Invalid Style ID '%s'", styleName);
    return;
  }

  if (ImGui::GetStyleVarInfo((ImGuiStyleVar)id)->Count != 1)
  {
    printf("atImGui Warning - Push Style: '%s' expects a Vec2. A double was passed.", styleName);
    return;
  }

  ImGui::PushStyleVar((ImGuiStyleVar)id, (float)val);
  _stylePopCount.push_back(1);
}

void atImGui::PushColour(const char *colourID)
{
  atVector<_atImGuiColours> *pColours = _coloursCache.TryGet(colourID);
  if (!pColours)
  {
    printf("atImGui Warning - Push Colour: No colour style exists with the id %s. Use AddColours to push add a named colour style string.", colourID);
    return;
  }

  for (_atImGuiColours &colDef : *pColours)
    ImGui::PushStyleColor((ImGuiCol)colDef.colID, colDef.colour);
  _styleColourPopCount.push_back(pColours->size());
}

void atImGui::PushStyle(const char *styleID)
{
  atVector<_atImGuiStyles> *pStyles = _stylesCache.TryGet(styleID);
  if (!pStyles)
  {
    printf("atImGui Warning - Push Style: No style exists with the id %s. Use AddStyles to push add a named style string.", styleID);
    return;
  }

  for (_atImGuiStyles &styleDef : *pStyles)
  {
    if (styleDef.nVals == 1)
      ImGui::PushStyleVar((ImGuiStyleVar)styleDef.varID, (float)styleDef.vec2[0]);
    else
      ImGui::PushStyleVar((ImGuiStyleVar)styleDef.varID, ImVec2(styleDef.vec2));
  }
  _stylePopCount.push_back(pStyles->size());
}

void atImGui::PopColour(int64_t count)
{
  while (count-- > 0)
  {
    if (_styleColourPopCount.size() == 0)
      return;
    ImGui::PopStyleColor((int)_styleColourPopCount.back());
    _styleColourPopCount.pop_back();
  }
}

void atImGui::PopStyle(int64_t count)
{
  while (count-- > 0)
  {
    if (_stylePopCount.size() == 0)
      return;
    ImGui::PopStyleVar((int)_stylePopCount.back());
    _stylePopCount.pop_back();
  }
}

void atImGui::PopColour() { PopColour(1); }
void atImGui::PopStyle() { PopStyle(1); }
