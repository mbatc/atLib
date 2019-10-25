#include "atTrioParser.h"
#include "atTrioState.h"
#include "atFile.h"

atTrioState::atTrioState() : m_vm(this) { m_envs.resize(1); }

atTrioResult atTrioState::Run(const atString &script) { return Load(script).Run(); }
atTrioResult atTrioState::RunFile(const atFilename &script) { return LoadFile(script).Run(); }
atTrioProgram atTrioState::LoadFile(const atFilename &script) { return Load(atFile::ReadText(script)); }

int64_t atTrioState::VarLoc(int64_t envIdx, int64_t varIdx) const
{
  return m_envs[envIdx].varLoc[varIdx];
}

int64_t atTrioState::CurVarCount() const
{
  return m_envs.back().varLoc.size();
}

void atTrioState::FindVar(const atString &name, int64_t *pEnv, int64_t *pVar) const
{
  for (int64_t env = m_envs.size() - 1; env >= 0; ++env)
    for (int64_t var = m_envs[env].varNames.size() - 1; var >= 0; ++var)
      if (m_envs[env].varNames[var] == name)
      {
        *pEnv = env;
        *pVar = var;
        return;
      }
}

bool atTrioState::AddVar(const atString &name, int64_t address)
{
  for (const atString &var : m_envs.back().varNames)
    if (var == name)
      return false;

  m_envs.back().varNames.push_back(name);
  m_envs.back().varLoc.push_back(address);
  return true;
}

void atTrioState::PushScope() { m_envs.push_back(Env()); }
void atTrioState::PopScope() { m_envs.pop_back(); }

atTrioProgram atTrioState::Load(const atString &script)
{
  atTrioParser parser(script);
  atTrioProgram pgrm = parser.EvaluateTree(this);
  return pgrm;
}

