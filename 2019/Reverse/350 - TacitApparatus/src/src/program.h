#pragma once

#include "vm.h"

#define INST_NOP() \
  (uint8_t)Opcode::Nop, 0, 0, __COUNTER__

#define INST_PERMUTE(Reg) \
  (uint8_t)Opcode::Permute, Reg, 0, __COUNTER__

#define INST_SCRAMBLE_BITS(Reg) \
  (uint8_t)Opcode::ScrambleBits, Reg, 0, __COUNTER__

#define INST_INC(Reg) \
  (uint8_t)Opcode::Inc, Reg, 0, __COUNTER__

#define INST_DEC(Reg) \
  (uint8_t)Opcode::Dec, Reg, 0, __COUNTER__

#define INST_MOVE(Reg, Imm) \
  (uint8_t)Opcode::Move, Reg, Imm, __COUNTER__

#define INST_MOVE_REG(Reg, RegSrc) \
  (uint8_t)Opcode::MoveReg, Reg, RegSrc, __COUNTER__

#define INST_LOAD(Reg, Addr) \
  (uint8_t)Opcode::Load, Reg, Addr, __COUNTER__

#define INST_LOADIND(Reg, RegSrc) \
  (uint8_t)Opcode::LoadIndirect, Reg, RegSrc, __COUNTER__

#define INST_STORE(Reg, Addr) \
  (uint8_t)Opcode::Store, Reg, Addr, __COUNTER__

#define INST_STOREIND(Reg, RegSrc) \
  (uint8_t)Opcode::StoreIndirect, Reg, RegSrc, __COUNTER__

#define INST_ROL(Reg) \
  (uint8_t)Opcode::RotateLeft, Reg, 0, __COUNTER__

#define INST_XOR(Reg, Imm) \
  (uint8_t)Opcode::Xor, Reg, Imm, __COUNTER__

#define INST_ADD_REG(Reg, RegSrc) \
  (uint8_t)Opcode::AddReg, Reg, RegSrc, __COUNTER__

#define INST_MUL_REG(Reg, RegSrc) \
  (uint8_t)Opcode::MulReg, Reg, RegSrc, __COUNTER__

#define INST_XOR_REG(Reg, RegSrc) \
  (uint8_t)Opcode::XorReg, Reg, RegSrc, __COUNTER__

#define INST_JNZ(Reg, Imm) \
  (uint8_t)Opcode::Jnz, Reg, Imm, __COUNTER__

#define INST_EXIT() \
  (uint8_t)Opcode::Exit, 0, 0, __COUNTER__

#define REG(a) a
#define IMM(a) a
#define ADDR(a) a

