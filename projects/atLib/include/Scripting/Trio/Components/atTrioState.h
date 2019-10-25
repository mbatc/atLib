#ifndef atTrioState_h__
#define atTrioState_h__

#include "atTrioProgram.h"
#include "atTrioMachine.h"
#include "atFilename.h"
#include "atHashMap.h"

enum atTrioStateRegister
{
  atTSR_In1,
  atTSR_In2,
  atTSR_Out1,
  atTSR_Count,
};

class atTrioState
{
  friend atTrioProgram;
  friend atTrioMachine;

public:
  atTrioState();

  atTrioResult Run(const atString &script);
  atTrioResult RunFile(const atFilename &script);
  atTrioProgram Load(const atString &script);
  atTrioProgram LoadFile(const atFilename &script);

protected:
  // Load a variables address into the specified register 'dst'
  int64_t VarLoc(int64_t envIdx, int64_t varIdx) const;
  int64_t CurVarCount() const;

  void FindVar(const atString &name, int64_t *pEnv, int64_t *pVar) const;
  bool AddVar(const atString &name, int64_t address);
  void PushScope();
  void PopScope();

  struct Env
  {
    atVector<int64_t> varLoc;
    atVector<atString> varNames;
  };

  atVector<Env> m_envs;
  atTrioMachine m_vm;
};

#endif // atTrioState_h__
