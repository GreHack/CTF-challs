#pragma once

#include <algorithm> // swap

const int N = 50;

static uint8_t rotateLeft(uint8_t val)
{
  return (val >> 7) | (val << 1);
}

static void scrambleBits(uint8_t* val)
{
  auto v0 = val[0];
  auto v1 = val[1];

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

  val[0] = (val[0] << 1) | b8;
  val[0] = (val[0] << 1) | b1;
  val[0] = (val[0] << 1) | b2;
  val[0] = (val[0] << 1) | bD;
  val[0] = (val[0] << 1) | b4;
  val[0] = (val[0] << 1) | bA;
  val[0] = (val[0] << 1) | b6;
  val[0] = (val[0] << 1) | b3;

  val[1] = (val[1] << 1) | b7;
  val[1] = (val[1] << 1) | bF;
  val[1] = (val[1] << 1) | b5;
  val[1] = (val[1] << 1) | bB;
  val[1] = (val[1] << 1) | bE;
  val[1] = (val[1] << 1) | bC;
  val[1] = (val[1] << 1) | b0;
  val[1] = (val[1] << 1) | b9;
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

void hash(uint8_t* s)
{
  for(int k = 0; k < 16; ++k)
  {
    for(int i = 0; i < N; ++i)
      s[i] = rotateLeft(s[i]);

    permute(s, 0xAC * k);

    for(int i = 0; i < N; i += 2)
      scrambleBits(s + i);

    for(int i = 0; i < N; ++i)
      s[i] ^= ((0x33 * k) & 0xff);
  }
}

