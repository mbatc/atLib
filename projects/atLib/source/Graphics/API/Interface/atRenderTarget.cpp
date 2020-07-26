#include "atRenderTarget.h"

void atRenderTarget::AttachColour(atTexture *pTexture, const int64_t &slot, const int64_t &layer)
{
  DetachColour(slot);
  Attachment att;
  att.pTex = pTexture;
  att.slot = slot;
  att.mipLayer = layer;
  m_color.push_back(att);
}

void atRenderTarget::AttachDepth(atTexture *pTexture)
{
  DetachDepth();
  m_depth.pTex = pTexture;
  m_depth.slot = 0;
  m_depth.mipLayer = 0;
}

void atRenderTarget::DetachColour(const int64_t &slot)
{
  for (int64_t i = 0; i < m_color.size(); ++i)
    if (m_color[i].slot == slot)
      m_color.erase(i);
}

void atRenderTarget::DetachDepth()
{
  m_depth = Attachment();
}

bool atRenderTarget::Clear(const atVec4F &color, const float &depth) { return false; }
