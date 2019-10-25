#ifndef atTrioMachine_h__
#define atTrioMachine_h__

#include "atTypes.h"
#include "atVector.h"
#include "atString.h"

enum atTrioMachineOpCode
{
  atTM_OP_BRN, // Branch                 | [ Condition Flags ][ Counter Offset ]              | 
  atTM_OP_ADD, // Integer Add            | [ Dst ][ Src 1 ][ Immediate Flag ][ Value/Src 2 ]  |
  atTM_OP_LD,  // Load                   | [ Dst ][ Address ]                                 | 
  atTM_OP_ST,  // Store                  | [ Src ][ Address ]                                 | 
  atTM_OP_JSR, // Jump Register          | [ Address ]                                        | 
  atTM_OP_AND, // Bitwise And            | [ Dst ][ Src 1 ][ Immediate Flag ][ Value/Src 2 ]  | 
  atTM_OP_LDR, // Load Register          | [ Dst ][ Src ][ Offset ]                           | Load data at value of r[Src] + Offset
  atTM_OP_STR, // Store Register         | [ Src ][ Dst ]                                     | Store value in register src at Address
  atTM_OP_RTI, // Unused                 | [ Dst ][ Src ]                                     | Interrupt
  atTM_OP_NOT, // Bitwise Not            | [ Dst ][ Src ]                                     | 
  atTM_OP_LDI, // Load Indirect          | [ Dst ][ Address ]                                 | The value of the location pointed to by 'Address' is loaded into 'Dst'
  atTM_OP_STI, // Store Indirect         | [ Src ][ Address ]                                 | Src is saved at the location pointed to by 'Address'
  atTM_OP_JMP, // Jump                   | [ Address ]                                        | 
  atTM_OP_RES, // Reserved               |                                                    | 
  atTM_OP_LEA, // Load Effective Address | [ Dst ][ Address ]                                 |
  atTM_OP_RET, // Return                 | [ Address ]                                        |
  atTM_OP_MOV, // Move                   | [ Dst ][ Src ]                                     | Copy the value of the 'Src' register to the 'Dst' register
  atTM_OP_HPA, // Heap Allocation        | [ Dst ][ Src ]                                     | Loads the address of the allocated memory into Dst
  atTM_OP_SKA, // Stack Allocation       | [ Dst ][ Src ]                                     | Loads the address of the allocated memory into Dst
  atTM_OP_SET, // Set                    | [ Dst ][ Value ]                                   | Sets the value of the 'Dst' register to 'value'
  atTM_OP_LDV, // Load Variable          | [ Dst ][ EnvStackID ][ VarID ]
  atTM_OP_FLV, // Find + Load Variable   | [ Dst ][ Dst2 ][ dst3 ]   Ex: [ varName ]          | Find and load a variable address into dst. dst2 and dst3 are intermidiate registers
  atTM_OP_STV, // Load Variable          | [ Dst ][ EnvStackID ][ VarID ]
  atTM_OP_FSV, // Load + store Variable  | [ Dst ][ EnvStackID ][ VarID ]

  atTM_OP_PSS, // Push Stack             
  atTM_OP_PPS, // Pop Stack              

  atTM_OP_Count,
};

enum atTrioMachineRegister
{
  atTM_REG_0,     // General Purpose
  atTM_REG_1,
  atTM_REG_2,
  atTM_REG_3,
  atTM_REG_4,
  atTM_REG_5,
  atTM_REG_6,
  atTM_REG_7,
  atTM_REG_SP,    // Stack pointer
  atTM_REG_PC,    // Program counter
  atTM_REG_COND,  // Condition flag
  atTM_REG_Count,
};

enum atTrioMachineCondFlag
{
  atTM_CF_POS = 1 << 0,
  atTM_CF_ZRO = 1 << 1,
  atTM_CF_NEG = 1 << 2,
};

// 64-bit virtual machine that emulates a Processing Unit

// TODO: Create a more complete virtual machine with multiple 'Cores'

class atTrioState;

class atTrioMachine
{
public:
  atTrioMachine(atTrioState *pState);

  void Execute(const atTrioMachineOpCode &op, const int64_t args[4], void *pData = nullptr);

  void Add(int64_t dst, int64_t src1, int64_t immediateFlag, int64_t src2);
  void And(int64_t dst, int64_t src1, int64_t immediateFlag, int64_t src2);
  void Branch(int64_t conditions, int64_t offset);
  void Jump(int64_t address);
  void JumpRegister(int64_t address);
  void Not(int64_t dst, int64_t src1);
  void Return();
  void Interrupt(int64_t dst, int64_t src);
  void Move(int64_t dst, int64_t src);
  void LoadVar(int64_t dst, int64_t env, int64_t var);
  void FindVar(int64_t dst1, int64_t dst2, const atString &name);
  
  void HeapAlloc(int64_t dst, int64_t src);
  void StackAlloc(int64_t dst, int64_t src);

  void Load(int64_t dst, int64_t address);
  void LoadRegister(int64_t dst, int64_t src);
  void LoadIndirect(int64_t dst, int64_t address);
  void LoadEffectiveAddress(int64_t dst, int64_t address);

  void Store(int64_t src, int64_t address);
  void StoreRegister(int64_t src, int64_t dst);
  void StoreIndirect(int64_t src, int64_t address);

  int64_t Step(); // Increment counter and return result
  int64_t Get(int64_t reg);

  void Set(int64_t reg, int64_t value);
  void SetCC(int64_t reg);

protected:
  static const int64_t m_stackBlockSize = 1024 * 1024;

  void SetStack(int64_t address, int64_t value, int64_t size = 4);
  int64_t GetStack(int64_t address, int64_t size = 4);

  uint8_t* GetStackMem(int64_t address);
  int64_t GetStackBlock(int64_t address);
  int64_t GetStackBlockOffset(int64_t address);

  int64_t m_register[atTM_REG_Count] = { 0 };

  atTrioState *m_pState;

  struct StackBlock { uint8_t data[m_stackBlockSize]; }; // 1 MB per block

  atVector<StackBlock> m_stack;
};

#endif // atTrioMachine_h__
