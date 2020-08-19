#include "atEUIImage.h"
#include "atGraphics.h"
#include "atCanvas2D.h"

atEUIImage::atEUIImage(atTexture *pImage) {}

bool atEUIImage::Draw(atCanvas2D *pGraphics)
{
  atRectF screenRect = ScreenRect();
  pGraphics->AddRectTextured(m_pTexture, screenRect.m_min, screenRect.m_max);
  return true;
}

void atEUIImage::SetImage(atTexture *pImage)
{
  pImage->AddReference();
  atGraphics::Release(m_pTexture);
  m_pTexture = pImage;
}

atTexture* atEUIImage::GetImage() const { return m_pTexture; }
