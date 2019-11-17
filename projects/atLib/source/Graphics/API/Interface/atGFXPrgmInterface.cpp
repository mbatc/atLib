#include "atGFXPrgmInterface.h"

atGFXPrgmInterface::atGFXPrgmInterface() {}

void atGFXPrgmInterface::Reload() { Delete(); }

bool atGFXPrgmInterface::SetStage(std::shared_ptr<atGFXShaderInterface> pShader)
{
  if (pShader->API() != API())
    return false;
  m_pStages[pShader->Stage()] = pShader;
  Reload();
  return true;
}

