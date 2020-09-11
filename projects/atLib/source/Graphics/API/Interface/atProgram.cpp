#include "atProgram.h"
#include "atFile.h"

int64_t atProgram::m_globalShaderRound = 0;

atProgram::atProgram() {}

void atProgram::ReloadAll() { ++m_globalShaderRound; }
void atProgram::Reload() { m_shaderRound = -1; }

bool atProgram::SetStage(atShader *pShader)
{
  if (pShader->API() != API())
    return false;
  pShader->AddReference();
  if (m_pStages[pShader->Stage()])
    m_pStages[pShader->Stage()]->Release();
  m_pStages[pShader->Stage()] = pShader;
  Reload();
  return true;
}

bool atProgram::SetStage(const atString &source, const atPipelineStage &stage)
{
  atGraphics *pCtx = atGraphics::GetCurrent();
  atShader *pShader = pCtx->CreateShader(stage);
  pShader->SetSource(source);
  SetStage(pShader);
  pCtx->Release(pShader);
  return true;
}

bool atProgram::SetStageFile(const atString &path, const atPipelineStage &stage)
{
  atGraphics *pCtx = atGraphics::GetCurrent();
  atShader *pShader = pCtx->CreateShader(stage);
  pShader->SetSourceFile(path);
  SetStage(pShader);
  pCtx->Release(pShader);
  return true;
}

bool atProgram::ShouldReload() const { return m_shaderRound != m_globalShaderRound; }
