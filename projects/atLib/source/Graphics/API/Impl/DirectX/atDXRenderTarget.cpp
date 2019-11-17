#include "atDXRenderTarget.h"
#include "atDXTexture.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atWindow.h"

bool atDXRenderTarget::Upload()
{
  return true;
}

bool atDXRenderTarget::Delete()
{
  return false;
}

bool atDXRenderTarget::Bind()
{
  ID3D11DepthStencilView *pDepthView = (ID3D11DepthStencilView*)((atDXTexture*)m_depth.pTex)->DepthView();
  atVector<ID3D11RenderTargetView*> renderTargets;
  int64_t maxSlot = 0;

  for (const Attachment &att : m_color)
    maxSlot = atMax(maxSlot, att.slot);
  renderTargets.resize(maxSlot);

  for (const Attachment &att : m_color)
    renderTargets[att.slot] = (ID3D11RenderTargetView*)((atDXTexture*)att.pTex)->RenderView();
  
  ((atDirectX*)atGraphics::GetCtx())->GetContext()->OMSetRenderTargets((UINT)maxSlot, renderTargets.data(), pDepthView);
  return true;
}

bool atDXRenderTarget::Clear(const atVec4F &color, const float &depth)
{
  for (int64_t i = 0; i < m_color.size(); ++i)
  {
    if (!m_color[i].pTex)
      continue;

    ID3D11RenderTargetView *pRenderView = (ID3D11RenderTargetView*)((atDXTexture*)m_color[i].pTex)->RenderView();
    ((atDirectX*)atGraphics::GetCtx())->GetContext()->ClearRenderTargetView(pRenderView, &color[0]);
  }

  if (m_depth.pTex)
  {
    ID3D11DepthStencilView *pDepthView = (ID3D11DepthStencilView*)((atDXTexture*)m_depth.pTex)->DepthView();
    ((atDirectX*)atGraphics::GetCtx())->GetContext()->ClearDepthStencilView(pDepthView, D3D11_CLEAR_DEPTH, depth, 0);
  }

  return true;
}
