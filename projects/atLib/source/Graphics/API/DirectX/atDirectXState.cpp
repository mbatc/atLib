#ifdef atPLATFORM_WIN32

#include "atDirectXState.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atDXInclude_Internal.h"

// DX11 State objects
struct __atDXStateImpl
{
  ID3D11DepthStencilState *pDepthState = nullptr;
  ID3D11RasterizerState *pRasterState = nullptr;
  ID3D11BlendState *pBlendState = nullptr;

  D3D11_RASTERIZER_DESC rasterDesc;
  D3D11_BLEND_DESC blendDesc;
  D3D11_DEPTH_STENCIL_DESC depthDesc;
  D3D11_VIEWPORT vpDesc;
  D3D11_RECT scissorDesc;
};

// ----------------------------------------------------------------------------------------
// DirectX Type Description Comparison Functions

bool operator==(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_SAMPLER_DESC)) == 0; }
bool operator==(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_BLEND_DESC)) == 0; }
bool operator==(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_RASTERIZER_DESC)) == 0; }
bool operator==(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_DEPTH_STENCIL_DESC)) == 0; }
bool operator!=(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return !(lhs == rhs); }

// ----------------------------------------------------------------------------------------

// Extract DX11 state descriptions from atRenderStateCore::State
static D3D11_DEPTH_STENCIL_DESC _DX11DepthDesc(const atRenderState::State &state);
static D3D11_RASTERIZER_DESC _DX11RasterDesc(const atRenderState::State &state);
static D3D11_BLEND_DESC _DX11BlendDesc(const atRenderState::State &state);
static D3D11_VIEWPORT _DX11ViewportDesc(const atRenderState::State &state);
static D3D11_RECT _DX11ScissorDesc(const atRenderState::State &state);

atDirectXState::atDirectXState() { m_pImpl = atNew(__atDXStateImpl); }
atDirectXState::~atDirectXState() { atDelete((__atDXStateImpl*)m_pImpl); }

void atDirectXState::Set(const atRenderState::State &state)
{
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  __atDXStateImpl &imp = *(__atDXStateImpl*)m_pImpl;

  D3D11_BLEND_DESC blendDesc = _DX11BlendDesc(state);

  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();

  if (blendDesc != imp.blendDesc || !imp.pBlendState)
  {
    pCtx->OMSetBlendState(nullptr, 0, 0x00);
    atDirectX::SafeRelease(imp.pBlendState);
    pDev->CreateBlendState(&blendDesc, &imp.pBlendState);
    pCtx->OMSetBlendState(imp.pBlendState, 0, 0xFFFFFF);
    imp.blendDesc = blendDesc;
  }

  D3D11_DEPTH_STENCIL_DESC depthDesc = _DX11DepthDesc(state);
  if (depthDesc != imp.depthDesc || !imp.pDepthState)
  {
    pCtx->OMSetDepthStencilState(nullptr, 0);
    atDirectX::SafeRelease(imp.pDepthState);
    pDev->CreateDepthStencilState(&depthDesc, &imp.pDepthState);
    pCtx->OMSetDepthStencilState(imp.pDepthState, 0);
    imp.depthDesc = depthDesc;
  }

  D3D11_RASTERIZER_DESC rasterDesc = _DX11RasterDesc(state);
  if (rasterDesc != imp.rasterDesc || !imp.pRasterState)
  {
    pCtx->RSSetState(nullptr);
    atDirectX::SafeRelease(imp.pRasterState);
    pDev->CreateRasterizerState(&rasterDesc, &imp.pRasterState);
    pCtx->RSSetState(imp.pRasterState);
    imp.rasterDesc = rasterDesc;
  }

  D3D11_VIEWPORT vp = _DX11ViewportDesc(state);
  if (vp != imp.vpDesc)
  {
    pCtx->RSSetViewports(1, &vp);
    imp.vpDesc = vp;
  }

  D3D11_RECT sc = _DX11ScissorDesc(state);
  if (sc != imp.scissorDesc)
  {
    pCtx->RSSetScissorRects(1, &sc);
    imp.scissorDesc = sc;
  }
}

// ----------------------------------
// INTERNAL USE SOURCE ONLY FUNCTIONS

static D3D11_RASTERIZER_DESC _DX11RasterDesc(const atRenderState::State &state)
{
  D3D11_RASTERIZER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.AntialiasedLineEnable = true;
  desc.CullMode = /*state.cullEnabled ? D3D11_CULL_BACK : */D3D11_CULL_NONE;
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

  if (desc.TopLeftX < 0)
    desc.TopLeftX = 0;
  if (desc.TopLeftY < 0)
    desc.TopLeftY = 0;
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
  desc.right = state.scissor.x + state.scissor.z;
  desc.bottom = state.scissor.y + state.scissor.w;
  return desc;
}

#endif
