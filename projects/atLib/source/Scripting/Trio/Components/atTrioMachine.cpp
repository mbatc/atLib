#include "atTrioMachine.h"
#include "atTrioState.h"
#include "atMath.h"

atTrioMachine::atTrioMachine(atTrioState *pState) { m_pState = pState; }

void atTrioMachine::Execute(const atTrioMachineOpCode &op, const int64_t args[4], void *pData)
{
  Step();
  switch (op)
  {
  // Storage
  case atTM_OP_LD:  Load(args[0], args[1]); break;
  case atTM_OP_LDR: LoadRegister(args[0], args[1]); break;
  case atTM_OP_LDI: LoadIndirect(args[0], args[1]); break;
  case atTM_OP_LEA: LoadEffectiveAddress(args[0], args[1]); break;
  case atTM_OP_ST:  Store(args[0], args[1]); break;
  case atTM_OP_STR: StoreRegister(args[0], args[1]); break;
  case atTM_OP_STI: StoreIndirect(args[0], args[1]); break;
  case atTM_OP_SET: Set(args[0], args[1]); break;

  case atTM_OP_FLV: FindVar(args[1], args[2], *(atString*)pData); // Fall through to load variable
  case atTM_OP_LDV: LoadVar(args[0], args[1], args[2]); break;

  // Control Flow
  case atTM_OP_BRN: Branch(args[0], args[1]); break;
  case atTM_OP_JMP: Jump(args[0]); break;
  case atTM_OP_JSR: JumpRegister(args[0]); break;
  case atTM_OP_RTI: Interrupt(args[0], args[1]); break;

  // Arithmetic
  case atTM_OP_ADD: Add(args[0], args[1], args[2], args[3]); break;
  case atTM_OP_AND: And(args[0], args[1], args[2], args[3]); break;
  case atTM_OP_NOT: Not(args[0], args[1]); break;
  case atTM_OP_PPS: m_pState->PopScope(); break;
  case atTM_OP_PSS: m_pState->PushScope(); break;
  case atTM_OP_RES: break;
  }
}

void atTrioMachine::Add(int64_t dst, int64_t src1, int64_t immediateFlag, int64_t src2)
{
  Set(dst, Get(src1) + (immediateFlag ? src2 : Get(src2)));
  SetCC(dst);
}

void atTrioMachine::And(int64_t dst, int64_t src1, int64_t immediateFlag, int64_t src2)
{
  Set(dst, Get(src1) & (immediateFlag ? src2 : Get(src2)));
  SetCC(dst);
}

void atTrioMachine::Branch(int64_t conditions, int64_t offset)
{
  if ((Get(atTM_REG_COND) & conditions) > 0)
    Jump(atTM_REG_PC + offset);
}

void atTrioMachine::Jump(int64_t address)
{
  Set(atTM_REG_PC, address);
}

void atTrioMachine::JumpRegister(int64_t address)
{
  Move(atTM_REG_7, atTM_REG_PC);
  Jump(address);
}

void atTrioMachine::Load(int64_t dst, int64_t address)
{
  Set(dst, GetStack(address));
  SetCC(dst);
}

void atTrioMachine::LoadRegister(int64_t dst, int64_t src)
{
  Load(dst, Get(src));
}

void atTrioMachine::LoadIndirect(int64_t dst, int64_t address)
{
  Load(dst, GetStack(address));
}

void atTrioMachine::LoadEffectiveAddress(int64_t dst, int64_t address)
{
  Set(dst, address);
  SetCC(dst);
}

void atTrioMachine::Not(int64_t dst, int64_t src)
{
  Set(dst, ~Get(src));
  SetCC(dst);
}

void atTrioMachine::Return() { Move(atTM_REG_PC, atTM_REG_7); }

void atTrioMachine::Interrupt(int64_t dst, int64_t src) { atUnused(dst, src); }

void atTrioMachine::Move(int64_t dst, int64_t src)
{
  Set(dst, Get(src));
  SetCC(dst);
}

void atTrioMachine::LoadVar(int64_t dst, int64_t env, int64_t var)
{
  Set(dst, m_pState->VarLoc(env, var));
  SetCC(dst);
}

void atTrioMachine::FindVar(int64_t dst1, int64_t dst2, const atString &name)
{
  m_pState->FindVar(name, &m_register[dst1], &m_register[dst2]);
}

void atTrioMachine::HeapAlloc(int64_t dst, int64_t src)
{
  Set(dst, (int64_t)atAlloc(Get(src)));
}

void atTrioMachine::StackAlloc(int64_t dst, int64_t src)
{
  Set(dst, Get(atTM_REG_SP)); // Store current stack pointer
  Set(atTM_REG_SP, Get(atTM_REG_SP) + Get(src)); // Increment stack pointer (to reserve the memory)
}

void atTrioMachine::Store(int64_t src, int64_t address)
{
  SetStack(address, Get(src), sizeof(int64_t));
}

void atTrioMachine::StoreRegister(int64_t src, int64_t dst)
{
  Store(src, Get(dst));
}

void atTrioMachine::StoreIndirect(int64_t src, int64_t address)
{
  Store(src, GetStack(address, 4));
}

int64_t atTrioMachine::Step()
{
  m_register[atTM_REG_PC] += 1;
  return m_register[atTM_REG_PC];
}

int64_t atTrioMachine::GetStack(int64_t address, int64_t size)
{
  int64_t ret = 0;
  memcpy(&ret, GetStackMem(address), atClamp(size, 0, 4));
  return int64_t();
}

void atTrioMachine::SetStack(int64_t address, int64_t value, int64_t size) { memcpy(GetStackMem(address), &value, atClamp(size, 0, 4)); }
void atTrioMachine::Set(int64_t reg, int64_t value) { m_register[reg] = value; }

void atTrioMachine::SetCC(int64_t reg)
{
  if (m_register[reg] < 0)      m_register[atTM_REG_COND] = atTM_CF_NEG;
  else if (m_register[reg] > 0) m_register[atTM_REG_COND] = atTM_CF_POS;
  else                          m_register[atTM_REG_COND] = atTM_CF_ZRO;
}

uint8_t* atTrioMachine::GetStackMem(int64_t address) { return &m_stack[GetStackBlock(address)].data[GetStackBlockOffset(address)]; }

int64_t atTrioMachine::Get(int64_t reg) { return m_register[reg]; }
int64_t atTrioMachine::GetStackBlock(int64_t address) { return address / m_stackBlockSize; }
int64_t atTrioMachine::GetStackBlockOffset(int64_t address) { return address % m_stackBlockSize; }
