
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

ID3D11DepthStencilState *atRenderState::m_pDepthState;
ID3D11RasterizerState *atRenderState::m_pRasterState;
ID3D11BlendState *atRenderState::m_pBlendState;
D3D11_DEPTH_STENCIL_DESC atRenderState::m_depthDesc;
D3D11_RASTERIZER_DESC atRenderState::m_rasterDesc;
D3D11_BLEND_DESC atRenderState::m_blendDesc;
D3D11_DEPTH_STENCIL_DESC atRenderState::m_lastDepthDesc;
D3D11_RASTERIZER_DESC atRenderState::m_lastRasterDesc;
D3D11_BLEND_DESC atRenderState::m_lastBlendDesc;
D3D11_VIEWPORT atRenderState::m_viewport;
D3D11_CULL_MODE atRenderState::m_lastCullMode;
D3D11_RECT atRenderState::m_scissor;

int64_t atRenderState::m_shader = AT_INVALID_ID;
bool atRenderState::m_defaultSet = false;
bool atRenderState::m_viewDirty = true;
bool atRenderState::m_scissorDirty = false;
bool atRenderState::m_viewportSet = false;
bool atRenderState::m_scissorSet = false;

void atRenderState::Bind()
{
  SetDefaults();
  if (m_lastBlendDesc != m_blendDesc || !m_pBlendState)
  {
    atGraphics::GetContext()->OMSetBlendState(nullptr, 0, 0x00);
    atGraphics::SafeRelease(m_pBlendState);
    atGraphics::GetDevice()->CreateBlendState(&m_blendDesc, &m_pBlendState);
    atGraphics::GetContext()->OMSetBlendState(m_pBlendState, 0, 0xFFFFFF);
    m_lastBlendDesc = m_blendDesc;
  }

  if (m_lastDepthDesc != m_depthDesc || !m_pDepthState)
  {
    atGraphics::GetContext()->OMSetDepthStencilState(nullptr, 0);
    atGraphics::SafeRelease(m_pDepthState);
    atGraphics::GetDevice()->CreateDepthStencilState(&m_depthDesc, &m_pDepthState);
    atGraphics::GetContext()->OMSetDepthStencilState(m_pDepthState, 0);
    m_lastDepthDesc = m_depthDesc;
  }
  
  if (m_lastRasterDesc != m_rasterDesc || !m_pRasterState)
  {
    atGraphics::GetContext()->RSSetState(nullptr);
    atGraphics::SafeRelease(m_pRasterState);
    atGraphics::GetDevice()->CreateRasterizerState(&m_rasterDesc, &m_pRasterState);
    atGraphics::GetContext()->RSSetState(m_pRasterState);
    m_lastRasterDesc = m_rasterDesc;
  }

  if (m_viewDirty)
  {
    atGraphics::GetContext()->RSSetViewports(1, &m_viewport);
    m_viewDirty = false;
  }

  if (m_scissorDirty)
  {
    atGraphics::GetContext()->RSSetScissorRects(1, &m_scissor);
    m_scissorDirty = false;
  }
}

void atRenderState::BindShader(const int64_t id) 
{
  atShaderPool::BindShader(id);
  m_shader = id;
}

void atRenderState::SetViewport(const atVec4I &viewport, const bool updateFlag)
{
  SetDefaults();
  if (viewport.x != m_viewport.TopLeftX || viewport.y != m_viewport.TopLeftY ||
    viewport.z != m_viewport.Width || viewport.w != m_viewport.Height)
  {
    m_viewport.Height = (FLOAT)(viewport.w - viewport.y);
    m_viewport.Width = (FLOAT)(viewport.z - viewport.x);
    m_viewport.TopLeftX = (FLOAT)viewport.x;
    m_viewport.TopLeftY = (FLOAT)viewport.y;

    m_viewportSet |= updateFlag;
    m_viewDirty = true;
  }
}

void atRenderState::SetScissor(const atVec4I &scissor)
{
  SetDefaults();
  if (m_scissor.left != scissor.x || m_scissor.right != scissor.z || m_scissor.bottom != scissor.w || m_scissor.top != scissor.y)
  {
    m_scissor.left = scissor.x;
    m_scissor.right = scissor.z;
    m_scissor.bottom = scissor.w;
    m_scissor.top = scissor.y;
    m_scissorDirty = true;
  }
}

void atRenderState::SetDepthRange(const float min, const float max)
{
  SetDefaults();
  m_viewport.MinDepth = (FLOAT)min;
  m_viewport.MaxDepth = (FLOAT)max;
}

void atRenderState::EnableCulling(const bool enable)
{
  SetDefaults();
  m_rasterDesc.CullMode = enable ? m_lastCullMode : D3D11_CULL_NONE;
  m_lastCullMode = enable ? m_rasterDesc.CullMode : m_lastCullMode;
}

void atRenderState::EnableBlend(const bool enable)
{
  SetDefaults();
  m_blendDesc.RenderTarget[0].BlendEnable = (BOOL)enable;
}

void atRenderState::EnableScissor(const bool enable)
{
  SetDefaults();
  m_rasterDesc.ScissorEnable = (BOOL)enable;
}

void atRenderState::EnableAA(const bool enable)
{
  SetDefaults();
  m_rasterDesc.AntialiasedLineEnable = (BOOL)enable;
}

void atRenderState::EnableMultisample(const bool enable)
{
  SetDefaults();
  m_rasterDesc.MultisampleEnable = (BOOL)enable;
}

void atRenderState::SetDefaults()
{
  if (m_defaultSet)
    return;

  ZeroMemory(&m_lastRasterDesc, sizeof(D3D11_RASTERIZER_DESC)); 
  m_lastBlendDesc = { 0 };
  m_lastDepthDesc = { 0 };

  m_depthDesc.DepthEnable = true;
  m_depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  m_depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
  m_depthDesc.StencilEnable = false;
  m_depthDesc.StencilReadMask = 0xFF;
  m_depthDesc.StencilWriteMask = 0xFF;

  m_depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  m_depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  m_depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  m_depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  m_depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  m_depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  m_depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  m_depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  m_rasterDesc.AntialiasedLineEnable = true;
  m_rasterDesc.CullMode = D3D11_CULL_NONE;
  m_rasterDesc.DepthBias = 0;
  m_rasterDesc.DepthBiasClamp = 0.0f;
  m_rasterDesc.DepthClipEnable = true;
  m_rasterDesc.FillMode = D3D11_FILL_SOLID;
  m_rasterDesc.FrontCounterClockwise = false;
  m_rasterDesc.MultisampleEnable = true;
  m_rasterDesc.ScissorEnable = true;
  m_rasterDesc.SlopeScaledDepthBias = 0.0f;

  m_blendDesc.AlphaToCoverageEnable = false;
  m_blendDesc.IndependentBlendEnable = true; 
  m_blendDesc.RenderTarget[0].BlendEnable = true;
  m_blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  m_blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  m_blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  m_blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  m_blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  m_blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  m_blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

  m_viewport.MinDepth = 0.0f;
  m_viewport.MaxDepth = 1.0f;
}

void atRenderState::EnableDepthTest(const bool enable) { SetDefaults(); m_depthDesc.DepthEnable = enable; }
bool atRenderState::ViewportSet() { return m_viewportSet; }
bool atRenderState::ScissorSet() { return m_scissorSet; }
