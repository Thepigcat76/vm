#include "runner.h"
#include "../bin/code.h"
#include "../shared.h"
#include "../vm/vm.h"

#include <stdint.h>

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
  case OP_MOVC2R: {
    mov_c2r(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_SYSCALL: {
    syscall(vm);
    break;
  }
  case OP_JMP: {
    jump(vm, ins_bytes[1]);
    break;
  }
  case OP_DECL_BYTE: {
    decl(vm, ins_bytes[1]);
    break;
  }
  }
}

void run_asm(uint8_t *byte_code, size_t byte_code_len) {
  uint8_t stack[STACK_SIZE] = {0};
  uint8_t constants[CONSTANT_COUNT] = {0};
  uint64_t regs[REGISTER_COUNT] = {0};
  VirtualMachine vm = {.regs = regs, .stack = stack, .constants = constants, .constans_count = 0, .ip = 0};

  while (vm.ip < byte_code_len) {
    uint8_t opcode = byte_code[vm.ip];
    size_t instruction_len = ins_len(opcode);

    uint8_t bytes[instruction_len];

    for (size_t i = 0; i < instruction_len; i++) {
      bytes[i] = byte_code[vm.ip + i];
    }
    vm.ip += instruction_len;
    run_ins(&vm, bytes, instruction_len);
  }

  //dump(&vm);
}
