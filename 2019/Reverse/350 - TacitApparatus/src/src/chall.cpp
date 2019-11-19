//-----------------------------------------------------------------------------
// The actual challenge program.
//-----------------------------------------------------------------------------
#include <algorithm>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "span.h"
#include "vm.h"
#include "program.h"

#include "model_hashed_flag.h"

// VM-version of the program in 'model_hasher.h'.
static constexpr uint8_t THE_PROGRAM[] =
{
  INST_MOVE(REG(1), IMM(16)),
  INST_MOVE(REG(2), IMM(0)),
  INST_NOP(),
  INST_NOP(),
  INST_NOP(),
  INST_NOP(),
  INST_NOP(),
  INST_NOP(),

  // RotateLeft pass
  INST_MOVE(REG(3), IMM(50)),
  INST_MOVE(REG(4), IMM(0)),
  INST_LOADIND(REG(0), REG(4)),
  INST_ROL(REG(0)),
  INST_STOREIND(REG(4), REG(0)),
  INST_INC(REG(4)),
  INST_DEC(REG(3)),
  INST_JNZ(REG(3), MAX_VM_INPUT_SIZE + (8 + 2) * 4),

  // Permute pass
  INST_MOVE(REG(4), IMM(0)),
  INST_ADD_REG(REG(4), REG(2)),
  INST_MOVE(REG(3), IMM(0xAC)),
  INST_MUL_REG(REG(4), REG(3)),
  INST_PERMUTE(REG(4)),

  // Bit-Scramble pass
  INST_MOVE(REG(3), IMM(50)),
  INST_MOVE(REG(4), IMM(0)),
  INST_SCRAMBLE_BITS(REG(4)),
  INST_MOVE(REG(0), IMM(0x33)),
  INST_MUL_REG(REG(0), REG(2)),

  INST_LOADIND(REG(5), REG(4)),
  INST_XOR_REG(REG(5), REG(0)),
  INST_STOREIND(REG(4), REG(5)),
  INST_INC(REG(4)),

  INST_LOADIND(REG(5), REG(4)),
  INST_XOR_REG(REG(5), REG(0)),
  INST_STOREIND(REG(4), REG(5)),
  INST_INC(REG(4)),

  INST_DEC(REG(3)),
  INST_DEC(REG(3)),
  INST_JNZ(REG(3), MAX_VM_INPUT_SIZE + 23 * 4),

  // -----------
  INST_INC(REG(2)),
  INST_DEC(REG(1)),
  INST_JNZ(REG(1), MAX_VM_INPUT_SIZE + 8 * 4),

  INST_EXIT(),
};

static_assert(MAX_VM_INPUT_SIZE >= sizeof HashedFlag);

Span<const uint8_t> runProgram(Span<const uint8_t> input, Span<const uint8_t> program, Span<uint8_t> buffer)
{
  Machine vm;

  auto const N = std::min(MAX_VM_INPUT_SIZE, (int)input.len);

  for(int i = 0; i < N; ++i)
    vm.memory[i] = input.ptr[i];

  for(size_t i = 0; i < program.len; ++i)
    vm.memory[MAX_VM_INPUT_SIZE + i] = program.ptr[i];

  while(!vm.exit)
    vm.step();

  for(int i = 0; i < N; ++i)
    buffer.ptr[i] = vm.memory[i];

  return { buffer.ptr, (size_t)N };
}

bool check(Span<const uint8_t> input)
{
  uint8_t buffer[256];
  auto result = runProgram(input, THE_PROGRAM, buffer);
  result.len = 50;
  return result.isEqual(HashedFlag);
}

static Span<const uint8_t> fromStringZ(const char* s)
{
  return Span<const uint8_t> { (uint8_t*)s, strlen(s) };
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Usage: %s <flag>\n", argv[0]);
    return 1;
  }

  auto input = fromStringZ(argv[1]);

  if(!(input.len >= 4
       && input.ptr[0] == 'G'
       && input.ptr[1] == 'H'
       && input.ptr[2] == '1'
       && input.ptr[3] == '9'
       ))
  {
    fprintf(stderr, "Now there is no sound for we all live underground. Try again ...\n");
    return 1;
  }

  if(input.len != sizeof HashedFlag)
  {
    fprintf(stderr, "Nope!\n");
    return 1;
  }

  if(!check(input))
  {
    fprintf(stderr, "Nice try... but still no luck!\n");
    return 1;
  }

  fprintf(stderr, "See, its a crazy world we're living in! Well done!\n");
  return 0;
}

