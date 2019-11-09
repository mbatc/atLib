#include "atGFXPrgmInterface.h"

atGFXPrgmInterface::atGFXPrgmInterface() { memset(m_pStages, 0, sizeof(m_pStages)); }

void atGFXPrgmInterface::Reload() { Delete(); }

bool atGFXPrgmInterface::SetStage(atGFXShaderInterface *pShader)
{
  if (pShader->API() != API())
    return false;
  m_pStages[pShader->Stage()] = pShader;
  Reload();
  return true;
}

