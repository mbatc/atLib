#include "atProgram.h"
#include "atFile.h"

atProgram::atProgram() {}

void atProgram::Reload() { Delete(); }

bool atProgram::SetStage(atShader *pShader)
{
  if (pShader->API() != API())
    return false;
  pShader->AddReference();
  m_pStages[pShader->Stage()] = pShader;
  Reload();
  return true;
}

bool atProgram::SetStage(const atString &source, const atPipelineStage &stage)
{
  atGraphics *pCtx = atGraphics::GetCurrent();
  atShader *pShader = pCtx->CreateShader(source, stage);
  SetStage(pShader);
  pCtx->Release(pShader);
  return true;
}

bool atProgram::SetStageFile(const atString &path, const atPipelineStage &stage)
{
  return SetStage(atFile::ReadText(path), stage);
}

