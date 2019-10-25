#include "atTrioProgram.h"
#include "atTrioState.h"

atTrioProgram::atTrioProgram(atTrioState *pState) : m_pState(pState) {}

atTrioResult atTrioProgram::Run() const
{
  while (m_pState->m_vm.Get(atTM_REG_PC) < m_instructions.size())
  {
    const Instruction &instr = m_instructions[m_pState->m_vm.Get(atTM_REG_PC)];
    m_pState->m_vm.Execute(instr.code, instr.args, instr.pExData);
  }
  return atTrioResult(atTRC_Success);
}

void atTrioProgram::AddInstruction(const atTrioMachineOpCode &opCode, const int64_t &a1, const int64_t &a2, const int64_t &a3, const int64_t &a4, void *pData)
{
  Instruction newInstr;
  newInstr.code = opCode;
  newInstr.args[0] = a1;
  newInstr.args[1] = a2;
  newInstr.args[2] = a3;
  newInstr.args[3] = a4;
  newInstr.pExData = pData;
  m_instructions.push_back(newInstr);
}
