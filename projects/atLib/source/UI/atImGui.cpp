
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
#include "atHashMap.h"
#include "atImGui.h"
#include "atImage.h"
#include "atInput.h"
#include "atScan.h"

#include "atRenderable.h"
#include "atDXTexture.h"
#include "atDXShader.h"
#include "atDXPrgm.h"

#include <memory>
#include <time.h>

static const char* _vertShaderSrc =
R"(

cbuffer vertexBuffer : register(b0)
{
  float4x4 ProjectionMatrix;
};

struct VS_INPUT
{
  float2 pos : POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output;
  output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));
  output.col = input.col;
  output.uv  = input.uv;
  return output;
}

)";

static const char* _fragShaderSrc =
R"(

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

sampler sampler0;
Texture2D texture0;

float4 main(PS_INPUT input) : SV_Target
{
  float4 out_col = input.col * texture0.Sample(sampler0, input.uv);
  return out_col;
}

)";

static std::shared_ptr<atDXPrgm> _prgm;
static std::shared_ptr<atDXShader> _vertShader;
static std::shared_ptr<atDXShader> _fragShader;

static std::shared_ptr<atDXSampler> _sampler;

static std::shared_ptr<atDXBuffer> _positions;
static std::shared_ptr<atDXBuffer> _texCoords;
static std::shared_ptr<atDXBuffer> _colours;
static std::shared_ptr<atDXBuffer> _indices;

static atVector<atFilename> _fontFiles;

static atRenderable _ro;

static int64_t _vbSize = AT_INVALID_ID;
static int64_t _ibSize = AT_INVALID_ID;
static int64_t _lastTime = -1;
static int64_t _ticksPerSecond = -1;

static bool _initialised = false;

static bool _UpdateMouseCursor(ImGuiMouseCursor imguiCursor)
{
  static ImGuiMouseCursor lastCursor = ImGuiMouseCursor_None;
  if (imguiCursor == lastCursor)
    return false;
  lastCursor = imguiCursor;

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
    return false;

  if (imguiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
  {
    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
    SetCursor(NULL);
  }
  else
  {
    // Show OS mouse cursor
    LPTSTR win32Cursor = IDC_ARROW;
    switch (imguiCursor)
    {
    case ImGuiMouseCursor_Arrow:        win32Cursor = IDC_ARROW; break;
    case ImGuiMouseCursor_TextInput:    win32Cursor = IDC_IBEAM; break;
    case ImGuiMouseCursor_ResizeAll:    win32Cursor = IDC_SIZEALL; break;
    case ImGuiMouseCursor_ResizeEW:     win32Cursor = IDC_SIZEWE; break;
    case ImGuiMouseCursor_ResizeNS:     win32Cursor = IDC_SIZENS; break;
    case ImGuiMouseCursor_ResizeNESW:   win32Cursor = IDC_SIZENESW; break;
    case ImGuiMouseCursor_ResizeNWSE:   win32Cursor = IDC_SIZENWSE; break;
    case ImGuiMouseCursor_Hand:         win32Cursor = IDC_HAND; break;
    }
    SetCursor(LoadCursor(NULL, win32Cursor));
  }
  return true;
}

static bool _Initialise()
{
  if (_initialised)
    return true;

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImFontConfig config;
  config.OversampleH = config.OversampleV = 1;
  config.PixelSnapH = true;
  for (const atFilename &file : _fontFiles)
    io.Fonts->AddFontFromFileTTF(file.c_str(), 13.0f, &config);

  uint8_t *pPixels = nullptr;
  int32_t width, height;
  io.Fonts->GetTexDataAsRGBA32(&pPixels, &width, &height);

  std::shared_ptr<atDXTexture> *pTex = atNew<std::shared_ptr<atDXTexture>>(std::make_shared<atDXTexture>(atImage((uint8_t*)pPixels, {width, height}, 4)));
  io.Fonts->TexID = (ImTextureID)pTex;

  _prgm = std::make_shared<atDXPrgm>();
  _vertShader = std::make_shared<atDXShader>(_vertShaderSrc, atPS_Vertex);
  _fragShader = std::make_shared<atDXShader>(_fragShaderSrc, atPS_Fragment);
  _sampler = std::make_shared<atDXSampler>();

  _prgm->SetStage(_vertShader);
  _prgm->SetStage(_fragShader);
  _ro.SetProgram(_prgm);
  _ro.SetSampler("sampler0", _sampler);

  if (!::QueryPerformanceFrequency((LARGE_INTEGER *)&_ticksPerSecond))
    return false;
  if (!::QueryPerformanceCounter((LARGE_INTEGER *)&_lastTime))
    return false;

  io.BackendFlags = ImGuiBackendFlags_HasMouseCursors;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = false;
  io.ConfigDockingTabBarOnSingleWindows = true;
  io.ConfigDockingTransparentPayload = true;
  io.ConfigWindowsResizeFromEdges = true;
  io.ConfigDockingWithShift = true;

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
  static atVector<atVec4F> colours;
  static atVector<atVec2F> positions;
  static atVector<atVec2F> texcoords;
  static atVector<uint32_t> indices;

  indices.clear(); indices.resize(pDrawData->TotalIdxCount);
  colours.clear(); colours.resize(pDrawData->TotalVtxCount);
  positions.clear(); positions.resize(pDrawData->TotalVtxCount);
  texcoords.clear(); texcoords.resize(pDrawData->TotalVtxCount);

  int64_t idxOffset = 0;
  int64_t vtxOffset = 0;

  for (int64_t i = 0; i < pDrawData->CmdListsCount; ++i)
  {
    const ImDrawList* pCmdList = pDrawData->CmdLists[i];

    for (int64_t v = 0; v < pCmdList->VtxBuffer.Size; ++v)
    {
      memcpy(positions.data() + vtxOffset + v, (uint8_t*)pCmdList->VtxBuffer.Data + v * sizeof(ImDrawVert) + (int64_t)&((ImDrawVert*)(0))->pos, sizeof(ImDrawVert::pos));
      memcpy(texcoords.data() + vtxOffset + v, (uint8_t*)pCmdList->VtxBuffer.Data + v * sizeof(ImDrawVert) + (int64_t)&((ImDrawVert*)(0))->uv, sizeof(ImDrawVert::uv));
      colours.data()[vtxOffset + v] = atColor::UnPack<float>(*(uint32_t*)((uint8_t*)pCmdList->VtxBuffer.Data + v * sizeof(ImDrawVert) + (int64_t)&((ImDrawVert*)(0))->col));
    }

    memcpy(indices.data() + idxOffset, pCmdList->IdxBuffer.Data, pCmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
    idxOffset += pCmdList->IdxBuffer.Size;
    vtxOffset += pCmdList->VtxBuffer.Size;
  }

  {
    float L = pDrawData->DisplayPos.x;
    float T = pDrawData->DisplayPos.y;
    atMat4F ortho = atMatrixOrtho(L, L + pDrawData->DisplaySize.x, T, T + pDrawData->DisplaySize.y, -0.5f, 0.5f);
    _ro.SetUniform("ProjectionMatrix", ortho);
  }

  if (!_positions) _positions = std::make_shared<atDXBuffer>(positions, atBT_VertexData);
  else             _positions->Set(positions);

  if (!_texCoords) _texCoords = std::make_shared<atDXBuffer>(texcoords, atBT_VertexData);
  else             _texCoords->Set(texcoords);

  if (!_colours) _colours = std::make_shared<atDXBuffer>(colours, atBT_VertexData);
  else           _colours->Set(colours);

  if (!_indices) _indices = std::make_shared<atDXBuffer>(indices, atBT_IndexData);
  else           _indices->Set(indices);

  return true;
}

void atImGui::AddFontFile(const atFilename &file) { _fontFiles.push_back(file); }

bool atImGui::BeginFrame(atWindow *pWnd)
{
  _Initialise();
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = pWnd->Size();

  io.MousePos = atInput::MousePos();
  ImGui::GetPlatformIO().MainViewport->Size = io.DisplaySize;

  int64_t curTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  io.DeltaTime = (float)(curTime - _lastTime) / (float)_ticksPerSecond;
  _lastTime = curTime;

  _UpdateMouseCursor(io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor());

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

  _ro.SetAttribute("POSITION", _positions);
  _ro.SetAttribute("TEXCOORD", _texCoords);
  _ro.SetAttribute("COLOR", _colours);
  _ro.SetAttribute("indices", _indices);

  unsigned int stride = sizeof(ImDrawVert);
  unsigned int offset = 0;

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
      {
        pCmd->UserCallback(pCmdList, pCmd);
      }
      else
      {
        // Apply scissor/clipping rectangle
        rs.SetScissor(atVec4I{ (LONG)(pCmd->ClipRect.x - pos.x), (LONG)(pCmd->ClipRect.y - pos.y), (LONG)(pCmd->ClipRect.z - pCmd->ClipRect.x), (LONG)(pCmd->ClipRect.w - pCmd->ClipRect.y) });

        // Bind texture, Draw
        _ro.SetTexture("texture0", *(std::shared_ptr<atDXTexture>*)pCmd->TextureId);

        _ro.Draw(true, atGFX_PT_TriangleList, pCmd->ElemCount, idxOffset, vtxOffset);
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
  case WM_LBUTTONUP: io.MouseDown[atKC_ImGui_Left] = false; break;
  case WM_RBUTTONUP: io.MouseDown[atKC_ImGui_Right] = false; break;
  case WM_MBUTTONUP: io.MouseDown[atKC_ImGui_Middle] = false; break;
  case WM_LBUTTONDOWN: io.MouseDown[atKC_ImGui_Left] = true; break;
  case WM_RBUTTONDOWN: io.MouseDown[atKC_ImGui_Right] = true; break;
  case WM_MBUTTONDOWN: io.MouseDown[atKC_ImGui_Middle] = true; break;
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

bool atImGui::Checkbox(const char *label, const bool active)
{
  bool v = active;
  ImGui::Checkbox(label, &v);
  return v;
}

const char* atImGui::TextInput(const char *name, const char *initial)
{
  static atVector<char> inputBuffer;
  int64_t initialLen = strlen(initial);
  inputBuffer.resize(atMax(inputBuffer.size(), strlen(initial) + 128));
  memcpy(inputBuffer.data(), initial, initialLen + 1);
  ImGui::InputText(name, inputBuffer.data(), inputBuffer.size());
  return inputBuffer.data();
}

atVec2D atImGui::GetWindowSize() { return{ GetWindowWidth(), GetWindowHeight() }; }

int64_t atImGui::GetWindowWidth() { return (int64_t)ImGui::GetWindowWidth(); }

int64_t atImGui::GetWindowHeight() { return (int64_t)ImGui::GetWindowHeight(); }

double atImGui::InputFloat(const char *name, const double val) { double in = val; ImGui::InputDouble(name, &in); return in; }

atVec2D atImGui::InputFloat2(const char *name, const atVec2D &val) { atVec2F in = val; ImGui::InputFloat2(name, &in[0]); return in; }

atVec3D atImGui::InputFloat3(const char *name, const atVec3D &val) { atVec3F in = val; ImGui::InputFloat3(name, &in[0]); return in; }

atVec4D atImGui::InputFloat4(const char *name, const atVec4D &val) { atVec4F in = val; ImGui::InputFloat4(name, &in[0]); return in; }

int32_t atImGui::InputInt(const char *name, const int32_t val) { int32_t in = val; ImGui::InputInt(name, &in); return in; }

atVec2I atImGui::InputInt2(const char *name, const atVec2I &val) { atVec2I in = val; ImGui::InputInt2(name, &in[0]); return in; }

atVec3I atImGui::InputInt3(const char *name, const atVec3I &val) { atVec3I in = val; ImGui::InputInt3(name, &in[0]); return in; }

atVec4I atImGui::InputInt4(const char *name, const atVec4I &val) { atVec4I in = val; ImGui::InputInt4(name, &in[0]); return in; }

bool atImGui::IsKeyDown(const int64_t keyCode) { return ImGui::IsKeyDown((int)keyCode); }

bool atImGui::IsKeyPressed(const int64_t keyCode) { return ImGui::IsKeyPressed((int)keyCode); }

bool atImGui::IsKeyReleased(const int64_t keyCode) { return ImGui::IsKeyReleased((int)keyCode); }

bool atImGui::IsWindowFocused() { return ImGui::IsWindowFocused(); }

bool atImGui::IsWindowHovered() { return ImGui::IsWindowHovered(); } 

bool atImGui::IsItemHovered() { return ImGui::IsItemHovered(); }

bool atImGui::IsItemActive() { return ImGui::IsItemActive(); }

bool atImGui::IsItemClicked() { return ImGui::IsItemClicked(); }

bool atImGui::IsItemFocused() { return ImGui::IsItemFocused(); }

bool atImGui::Begin(const char *name) { return ImGui::Begin(name); }

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

void atImGui::OpenPopup(const char *name) { ImGui::OpenPopup(name); }

bool atImGui::BeginPopup(const char *name) { return ImGui::BeginPopup(name); }

void atImGui::EndPopup() { ImGui::EndPopup(); }

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

void atImGui::ApplyDefaultStyle()
{
  ImGuiStyle* style = &ImGui::GetStyle();
  ImVec4* colors = style->Colors;

  colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
  colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
  colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
  colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
  colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

  style->ChildRounding = 4.0f;
  style->FrameBorderSize = 1.0f;
  style->FrameRounding = 2.0f;
  style->GrabMinSize = 7.0f;
  style->PopupRounding = 2.0f;
  style->ScrollbarRounding = 12.0f;
  style->ScrollbarSize = 13.0f;
  style->TabBorderSize = 1.0f;
  style->TabRounding = 0.0f;
  style->WindowRounding = 4.0f;
}
