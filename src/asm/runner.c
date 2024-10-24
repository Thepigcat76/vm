#include "runner.h"
#include "../vm/vm.h"
#include "code.h"
#include <stdint.h>
#include <stdio.h>

DEFINE_SET(string_t);

static void run_ins(VirtualMachine *vm, uint8_t *ins_bytes,
                    size_t instruction_len) {
  uint8_t opcode = ins_bytes[0];
  switch (opcode) {
  case OP_MOVI2R: {
    mov_i2r(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_MOVR2R: {
    mov_r2r(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_SYSCALL: {
    syscall(vm);
    break;
  }
  }
}

void run_asm(Runner *runner, uint8_t *byte_code, size_t byte_code_len) {
  uint8_t stack[STACK_SIZE] = {0};
  uint64_t regs[REG_AMOUNT] = {0};
  VirtualMachine vm = {.regs = regs, .stack = stack};

  size_t byte_code_index = 0;
  while (byte_code_index < byte_code_len) {
    uint8_t opcode = byte_code[byte_code_index];
    size_t instruction_len = ins_len(opcode);

    uint8_t bytes[instruction_len];

    for (size_t i = 0; i < instruction_len; i++) {
      bytes[i] = byte_code[byte_code_index + i];
    }
    run_ins(&vm, bytes, instruction_len);
    byte_code_index += instruction_len;
  }

  set_free(string_t, runner->labels);
  dump(&vm);
}