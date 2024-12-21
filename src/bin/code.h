#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  OP_SYSCALL,
  OP_MOVI2R,
  OP_MOVR2R,
  OP_MOVC2R,
  OP_DECL_BYTE,
  OP_JMP,
} Opcode;

size_t ins_len(Opcode opcode);
