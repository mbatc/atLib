
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
#include "atShaderPool.h"
#include "atHardwareTexture.h"

static atRenderState *s_pGlobalState = nullptr;

// DX11 State objects
static ID3D11DepthStencilState *s_pDepthState = nullptr;
static ID3D11RasterizerState *s_pRasterState = nullptr;
static ID3D11BlendState *s_pBlendState = nullptr;

// Extract DX11 state descriptions from atRenderStateCore::State

static D3D11_DEPTH_STENCIL_DESC _DX11DepthDesc(const atRenderState::State &state);
static D3D11_RASTERIZER_DESC _DX11RasterDesc(const atRenderState::State &state);
static D3D11_BLEND_DESC _DX11BlendDesc(const atRenderState::State &state);
static D3D11_VIEWPORT _DX11ViewportDesc(const atRenderState::State &state);
static D3D11_RECT _DX11ScissorDesc(const atRenderState::State &state);

atVector<atRenderState::State> atRenderState::m_stack;
atRenderState::State atRenderState::m_activeState;
bool atRenderState::m_setViewport = false;
bool atRenderState::m_alwaysBind = false;

atRenderState::atRenderState()
{
  if (!s_pGlobalState)
    s_pGlobalState = new atRenderState(0);
  Init();
}

atRenderState::~atRenderState() 
{
  m_stack.pop_back();
  if (m_alwaysBind && m_stack.size())
    Set(m_stack.back());
}

void atRenderState::Bind()
{
  State &top = m_stack.back();
  D3D11_BLEND_DESC topBlend = _DX11BlendDesc(top);
  D3D11_RASTERIZER_DESC topRaster = _DX11RasterDesc(top);
  D3D11_DEPTH_STENCIL_DESC topDepth = _DX11DepthDesc(top);

  if (_DX11BlendDesc(m_activeState) != topBlend || !s_pBlendState)
  {
    atGraphics::GetContext()->OMSetBlendState(nullptr, 0, 0x00);
    atGraphics::SafeRelease(s_pBlendState);
    atGraphics::GetDevice()->CreateBlendState(&topBlend, &s_pBlendState);
    atGraphics::GetContext()->OMSetBlendState(s_pBlendState, 0, 0xFFFFFF);
  }

  if (_DX11DepthDesc(m_activeState) != topDepth || !s_pDepthState)
  {
    atGraphics::GetContext()->OMSetDepthStencilState(nullptr, 0);
    atGraphics::SafeRelease(s_pDepthState);
    atGraphics::GetDevice()->CreateDepthStencilState(&topDepth, &s_pDepthState);
    atGraphics::GetContext()->OMSetDepthStencilState(s_pDepthState, 0);
  }
  
  if (_DX11RasterDesc(m_activeState) != topRaster || !s_pRasterState)
  {
    atGraphics::GetContext()->RSSetState(nullptr);
    atGraphics::SafeRelease(s_pRasterState);
    atGraphics::GetDevice()->CreateRasterizerState(&topRaster, &s_pRasterState);
    atGraphics::GetContext()->RSSetState(s_pRasterState);
  }

  if (m_activeState.viewport != top.viewport || m_activeState.depthRange != top.depthRange)
  {
    D3D11_VIEWPORT vp = _DX11ViewportDesc(top);
    atGraphics::GetContext()->RSSetViewports(1, &vp);
  }

  if (m_activeState.scissor != top.scissor)
  {
    D3D11_RECT sc = _DX11ScissorDesc(top);
    atGraphics::GetContext()->RSSetScissorRects(1, &sc);
  }

  // Bind Render Target Textures
  if (m_activeState.pColourTarget != top.pColourTarget || m_activeState.pDepthTarget != top.pDepthTarget ||
    (top.pColourTarget && top.pColourTarget->Dirty()) ||
    (top.pDepthTarget && top.pDepthTarget->Dirty()))
  {
    atTextureContext *pTex = top.pColourTarget ? atHardwareTexture::GetTexture(top.pColourTarget->GetColourTexID()) : nullptr;
    atTextureContext *pDepth = top.pDepthTarget ? atHardwareTexture::GetTexture(top.pDepthTarget->GetDepthTexID()) : nullptr;
    atGraphics::GetContext()->OMSetRenderTargets(pTex ? 1 : 0, pTex ? *pTex : nullptr, pDepth ? *pDepth : nullptr);
  }

  atAssert(atShaderPool::BindShader(top.shader) != AT_INVALID_ID, "Invalid shader ID");
  atAssert(atShaderPool::BindInputLayout(top.inputLayout) != AT_INVALID_ID, "Invalid shader ID");

  m_activeState = top;
}

void atRenderState::SetRenderTarget(atWindow *pTarget) { SetRenderTarget(&pTarget->m_dxTarget); }

void atRenderState::SetRenderTarget(atRenderTarget *pTarget)
{
  if (pTarget->GetColourTexID() != AT_INVALID_ID)
    MyState().pColourTarget = pTarget;
  if (pTarget->GetDepthTexID() != AT_INVALID_ID)
    MyState().pDepthTarget = pTarget;
}

bool atRenderState::SetShader(const int64_t id, const int64_t inputLayoutID)
{
#ifdef DEBUG
  if (!atShaderPool::IsValid(id))
  {
    atAssert(false, "Invalid shader ID passed to atRenderStateCore::SetShader()");
    return false;
  }

#endif
  MyState().shader = id;
  MyState().inputLayout = inputLayoutID;
  return true;
}

void atRenderState::Init()
{
  m_id = m_stack.size();
  m_stack.push_back(m_stack.size() ? m_stack.back() : State());
  if (m_stack.size() == 1)
  {
    MyState().scissor = { -INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX };
    
    UINT vpCount = 0;
    D3D11_VIEWPORT vp;
    atGraphics::GetContext()->RSGetViewports(&vpCount, nullptr);
    if (vpCount > 0)
    {
      atGraphics::GetContext()->RSGetViewports(&vpCount, &vp);
      MyState().viewport = { vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height };
    }
  }
}

void atRenderState::SetViewport(const atVec4I &vp) 
{ 
  MyState().viewport = vp;
  if (m_alwaysBind) Bind();
}

void atRenderState::SetScissor(const atVec4I &scissor) 
{ 
  MyState().scissor = scissor; m_setViewport = true;
  if (m_alwaysBind) Bind();
}

void atRenderState::SetDepthRange(const float min, const float max)
{
  MyState().depthRange = atVec2F(min, max);
  if (m_alwaysBind) Bind();
}

void atRenderState::SetDepthWriteEnabled(const bool enabled) 
{ 
  MyState().depthWriteEnabled = enabled;
  if (m_alwaysBind) Bind();
}

void atRenderState::SetDepthReadEnabled(const bool enabled)
{
  MyState().depthReadEnabled = enabled;
  if (m_alwaysBind) Bind();
}
void atRenderState::SetStencilEnabled(const bool enabled)
{
  MyState().stencilEnabled = enabled;
  if (m_alwaysBind) Bind();
}
void atRenderState::SetScissorEnabled(const bool enabled)
{
  MyState().scissorEnabled = enabled;
  if (m_alwaysBind) Bind();
}

void atRenderState::SetBlendEnabled(const bool enabled)
{
  MyState().blendEnabled = enabled;
  if (m_alwaysBind) Bind();
}
void atRenderState::SetMSAAEnabled(const bool enabled)
{
  MyState().msaaEnabled = enabled;
  if (m_alwaysBind) Bind();
}
void atRenderState::SetCullEnabled(const bool enabled)
{
  MyState().cullEnabled = enabled;
  if (m_alwaysBind) Bind();
}
void atRenderState::SetAAEnabled(const bool enabled)
{
  MyState().aaEnabled = enabled;
  if (m_alwaysBind) Bind();
}

void atRenderState::Set(const State &state)
{
  m_stack[m_id] = state;
  if (m_alwaysBind) Bind();
}

bool atRenderState::IsDepthWriteEnabled() const { return MyState().depthWriteEnabled; }
bool atRenderState::IsDepthReadEnabled() const { return MyState().depthReadEnabled; }
bool atRenderState::IsStencilEnabled() const { return MyState().stencilEnabled; }
bool atRenderState::IsScissorEnabled() const { return MyState().scissorEnabled; }
bool atRenderState::IsBlendEnabled() const { return MyState().blendEnabled; }
bool atRenderState::IsMSAAEnabled() const { return MyState().msaaEnabled; }
bool atRenderState::IsCullEnabled() const { return MyState().cullEnabled; }
bool atRenderState::IsAAEnabled() const { return MyState().aaEnabled; }
const atVec4I& atRenderState::Viewport() const { return MyState().viewport; }
const atVec4I& atRenderState::Scissor() const { return MyState().scissor; }
const atVec2F& atRenderState::DepthRange() const { return MyState().depthRange; }
const atRenderState::State& atRenderState::MyState() const { return m_stack[m_id]; }
atRenderState::State& atRenderState::MyState() { return m_stack[m_id]; }
atRenderState::atRenderState(int unused) { Init(); }

// ----------------------------------
// INTERNAL USE SOURCE ONLY FUNCTIONS

static D3D11_RASTERIZER_DESC _DX11RasterDesc(const atRenderState::State &state)
{
  D3D11_RASTERIZER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.AntialiasedLineEnable = true;
  desc.CullMode = state.cullEnabled ? D3D11_CULL_BACK : D3D11_CULL_NONE;
  desc.DepthBias = 0;
  desc.DepthBiasClamp = 0.0f;
  desc.DepthClipEnable = true;
  desc.FillMode = D3D11_FILL_SOLID;
  desc.FrontCounterClockwise = false;
  desc.MultisampleEnable = state.msaaEnabled;
  desc.ScissorEnable = state.scissorEnabled;
  desc.SlopeScaledDepthBias = 0.0f;
  return desc;
}

static D3D11_DEPTH_STENCIL_DESC _DX11DepthDesc(const atRenderState::State &state)
{
  D3D11_DEPTH_STENCIL_DESC desc = { 0 };
  desc.DepthEnable = state.depthReadEnabled;
  desc.DepthWriteMask = state.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  desc.DepthFunc = D3D11_COMPARISON_LESS;
  desc.StencilEnable = state.stencilEnabled;
  desc.StencilReadMask = 0xFF;
  desc.StencilWriteMask = 0xFF;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  return desc;
}

static D3D11_VIEWPORT _DX11ViewportDesc(const atRenderState::State &state)
{
  D3D11_VIEWPORT desc;
  desc.Height = (float)state.viewport.w;
  desc.Width = (float)state.viewport.z;
  desc.TopLeftX = (float)state.viewport.x;
  desc.TopLeftY = (float)state.viewport.y;
  desc.MinDepth = (float)state.depthRange.x;
  desc.MaxDepth = (float)state.depthRange.y;
  return desc;
}

static D3D11_BLEND_DESC _DX11BlendDesc(const atRenderState::State &state)
{
  D3D11_BLEND_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.AlphaToCoverageEnable = false;
  desc.IndependentBlendEnable = true; 
  desc.RenderTarget[0].BlendEnable = state.blendEnabled;
  desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
  return desc;
}

static D3D11_RECT _DX11ScissorDesc(const atRenderState::State &state)
{
  D3D11_RECT desc;
  desc.left = state.scissor.x;
  desc.top = state.scissor.y;
  desc.right = state.scissor.z;
  desc.bottom = state.scissor.w;
  return desc;
}

struct CleanupStruct
{
  ~CleanupStruct()
  {
    delete s_pGlobalState;
    s_pGlobalState = nullptr;
  }
};

static CleanupStruct cleanup;