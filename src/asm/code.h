#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  OP_SYSCALL = 0,
  OP_MOVI2R = 1,
  OP_MOVR2R = 2,
} Opcode;

size_t ins_len(Opcode opcode);
