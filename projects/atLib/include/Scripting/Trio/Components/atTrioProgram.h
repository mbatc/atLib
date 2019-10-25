#ifndef atTrioProgram_h__
#define atTrioProgram_h__

#include "atTrioResult.h"
#include "atTrioMachine.h"

class atTrioState;

class atTrioProgram
{
  struct Instruction
  {
    atTrioMachineOpCode code;
    int64_t args[4];
    void *pExData = nullptr;
  };

public:
  atTrioProgram(atTrioState *pState);

  atTrioResult Run() const;

  void AddInstruction(const atTrioMachineOpCode &opCode, const int64_t &a1, const int64_t &a2 = 0, const int64_t &a3 = 0, const int64_t &a4 = 0, void *pData = nullptr);

protected:
  atVector<Instruction> m_instructions;
  atTrioState *m_pState;
};

#endif // atTrioProgram_h__