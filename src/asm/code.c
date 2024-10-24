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
  default:
    return 1;
  }
}