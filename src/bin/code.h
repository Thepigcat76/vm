#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  OP_SYSCALL,
  OP_MOVI2R,
  OP_MOVR2R,
  OP_MOVM2R,
  OP_DECL_BYTE,
  OP_JMP,
  OP_CMP,
  OP_JNE,
  OP_ADDI2R,
  OP_SUBI2R,
  OP_MULI2R,
  OP_DIVI2R,
  OP_ADDI2M,
  OP_SUBI2M,
  OP_MULI2M,
  OP_DIVI2M,
} Opcode;

size_t ins_len(Opcode opcode);
