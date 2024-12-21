#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  OP_SYSCALL = 0,
  OP_MOVI2R = 1,
  OP_MOVR2R = 2,
  OP_MOVC2R = 3,
  OP_DECL_BYTE = 4,
} Opcode;

size_t ins_len(Opcode opcode);
