#include "code.h"

#include <stdint.h>
#include <stdlib.h>

size_t ins_len(Opcode opcode) {
  switch (opcode) {
  case OP_SYSCALL:
    return 1;
  case OP_MOVI2R:
  case OP_MOVR2R:
  case OP_MOVC2R:
    return 3;
  case OP_JMP:
    return 2;
  case OP_DECL_BYTE:
    return 2;
  }
}