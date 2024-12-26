#include "code.h"

#include <stdint.h>
#include <stdlib.h>

size_t ins_len(Opcode opcode) {
  switch (opcode) {
  case OP_SYSCALL:
    return 1;
  case OP_MOVI2R:
  case OP_MOVR2R:
    return 3;
  case OP_MOVM2R:
    return 2 + sizeof(uint64_t);
  case OP_JNE:
  case OP_JMP:
    return 2;
  case OP_CMP:
    return 3;
  case OP_DECL_BYTE:
    return 2;
  }
}