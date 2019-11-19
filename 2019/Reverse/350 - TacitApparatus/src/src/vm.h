#pragma once

#include <cassert>
#include <stdint.h>

constexpr int MAX_VM_INPUT_SIZE = 64;

enum class Opcode : uint8_t
{
  Nop = 0xff,
  Move = 0x44,
  MoveReg = 0x43,
  Jnz = 0x12,
  Inc = 0x13,
  Dec = 0xdf,
  Load = 0xca,
  Store = 0x36,
  RotateLeft = 0x37,
  LoadIndirect = 0x47,
  StoreIndirect = 0x48,
  ScrambleBits = 0x38,
  Permute = 0xce,
  AddReg = 0xbb,
  MulReg = 0xbc,
  Xor = 0xaa,
  XorReg = 0x34,
  Exit = 0x23,
};

struct Machine
{
  uint8_t memory[256] {};
  int regs[8] {};
  int pc = MAX_VM_INPUT_SIZE;
  bool exit = false;

  void step()
  {
    auto code = (Opcode)memory[pc + 0];
    auto op1 = memory[pc + 1];
    auto op2 = memory[pc + 2];

    switch(code)
    {
    case Opcode::Nop:
      break;
    case Opcode::Move:
      regs[op1] = op2;
      break;
    case Opcode::MoveReg:
      regs[op1] = regs[op2];
      break;
    case Opcode::Jnz:

      if(regs[op1] != 0)
        pc = op2 - 4;

      break;
    case Opcode::Inc:
      regs[op1]++;
      break;
    case Opcode::Dec:
      regs[op1]--;
      break;
    case Opcode::RotateLeft:
      regs[op1] = rotateLeft(regs[op1]);
      break;
    case Opcode::Load:
      regs[op1] = memory[op2];
      break;
    case Opcode::Store:
      memory[op1] = regs[op2];
      break;
    case Opcode::LoadIndirect:
      regs[op1] = memory[regs[op2]];
      break;
    case Opcode::StoreIndirect:
      memory[regs[op1]] = regs[op2];
      break;
    case Opcode::ScrambleBits:
      scrambleBits(memory + regs[op1]);
      break;
    case Opcode::Permute:
      permute(memory, regs[op1]);
      break;
    case Opcode::Xor:
      regs[op1] ^= op2;
      break;
    case Opcode::AddReg:
      regs[op1] += regs[op2];
      break;
    case Opcode::MulReg:
      regs[op1] *= regs[op2];
      break;
    case Opcode::XorReg:
      regs[op1] ^= regs[op2];
      break;
    case Opcode::Exit:
      exit = true;
      break;
    }

    pc += 4;
    pc %= 256;
  }

  static constexpr auto N = 50;

  static uint8_t rotateLeft(uint8_t val)
  {
    return (val >> 7) | (val << 1);
  }

  static void scrambleBits(uint8_t* val)
  {
    const auto v0 = val[0];
    const auto v1 = val[1];

    val[0] = val[1] = 0;

    auto b0 = (v0 & 0b00000001) ? 1 : 0;
    auto b1 = (v0 & 0b00000010) ? 1 : 0;
    auto b2 = (v0 & 0b00000100) ? 1 : 0;
    auto b3 = (v0 & 0b00001000) ? 1 : 0;
    auto b4 = (v0 & 0b00010000) ? 1 : 0;
    auto b5 = (v0 & 0b00100000) ? 1 : 0;
    auto b6 = (v0 & 0b01000000) ? 1 : 0;
    auto b7 = (v0 & 0b10000000) ? 1 : 0;

    auto b8 = (v1 & 0b00000001) ? 1 : 0;
    auto b9 = (v1 & 0b00000010) ? 1 : 0;
    auto bA = (v1 & 0b00000100) ? 1 : 0;
    auto bB = (v1 & 0b00001000) ? 1 : 0;
    auto bC = (v1 & 0b00010000) ? 1 : 0;
    auto bD = (v1 & 0b00100000) ? 1 : 0;
    auto bE = (v1 & 0b01000000) ? 1 : 0;
    auto bF = (v1 & 0b10000000) ? 1 : 0;

    uint8_t p = 0;
    uint8_t q = 0;

    p = (p << 1) | b8;
    p = (p << 1) | b1;
    p = (p << 1) | b2;
    p = (p << 1) | bD;
    p = (p << 1) | b4;
    p = (p << 1) | bA;
    p = (p << 1) | b6;
    p = (p << 1) | b3;

    q = (q << 1) | b7;
    q = (q << 1) | bF;
    q = (q << 1) | b5;
    q = (q << 1) | bB;
    q = (q << 1) | bE;
    q = (q << 1) | bC;
    q = (q << 1) | b0;
    q = (q << 1) | b9;

    val[0] = p;
    val[1] = q;
  }

  static void permute(uint8_t* memory, int seed)
  {
    const int A = 0x19;
    const int C = 0x1231;
    const int modulus = 0x65531;

    for(int i = 0; i < N; ++i)
    {
      seed = (A * seed + C) % modulus;
      int i1 = seed % N;

      seed = (A * seed + C) % modulus;
      int i2 = seed % N;

      std::swap(memory[i1], memory[i2]);
    }
  }
};

