#include "runner.h"
#include "../bin/code.h"
#include "../shared.h"
#include "../vm/vm.h"

#include <stdint.h>

#define RUN_BIN_OP_I2R(type)                                                   \
  {                                                                            \
    bin_op_i2r(vm, type, ins_bytes[1], ins_bytes[2]);                          \
    break;                                                                     \
  }

#define RUN_BIN_OP_I2M(type)                                                   \
  {                                                                            \
    bin_op_i2m(vm, type, ins_bytes[1], read_uint64(ins_bytes, 2));                          \
    break;                                                                     \
  }

static uint64_t pack_uint64(uint8_t bytes[8]) {
  uint64_t val = 0;
  for (size_t i = 0; i < sizeof(uint64_t); i++) {
    val |= (bytes[i]) << (i * 8);
  }
  return val;
}

static uint64_t read_uint64(uint8_t *ins_bytes, size_t pos) {
  uint8_t bytes[8];
  for (size_t i = 0; i < sizeof(uint64_t); i++) {
    bytes[i] = ins_bytes[i + pos];
  }
  return pack_uint64(bytes);
}

static void run_ins(VirtualMachine *vm, uint8_t *ins_bytes,
                    size_t instruction_len) {
  Opcode opcode = (Opcode)ins_bytes[0];
  switch (opcode) {
  case OP_MOVI2R: {
    mov_i2r(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_MOVR2R: {
    mov_r2r(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_MOVM2R: {
    uint64_t addr = read_uint64(ins_bytes, 1);
    mov_m2r(vm, addr, ins_bytes[sizeof(uint64_t) + 1]);
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
  case OP_JNE: {
    jne(vm, ins_bytes[1]);
    break;
  }
  case OP_CMP: {
    cmp(vm, ins_bytes[1], ins_bytes[2]);
    break;
  }
  case OP_DECL_BYTE: {
    decl(vm, ins_bytes[1]);
    break;
  }
  case OP_ADDI2R:
    RUN_BIN_OP_I2R(BIN_OP_ADD)
  case OP_SUBI2R:
    RUN_BIN_OP_I2R(BIN_OP_SUB)
  case OP_MULI2R:
    RUN_BIN_OP_I2R(BIN_OP_MUL)
  case OP_DIVI2R:
    RUN_BIN_OP_I2R(BIN_OP_DIV)
  case OP_ADDI2M:
    RUN_BIN_OP_I2M(BIN_OP_ADD)
  case OP_SUBI2M:
    RUN_BIN_OP_I2M(BIN_OP_SUB)
  case OP_MULI2M:
    RUN_BIN_OP_I2M(BIN_OP_MUL)
  case OP_DIVI2M:
    RUN_BIN_OP_I2M(BIN_OP_DIV)
  }
}

void run_asm(uint8_t *byte_code, size_t byte_code_len) {
  uint8_t stack[STACK_SIZE] = {0};
  uint8_t constants[CONSTANT_COUNT] = {0};
  uint64_t regs[REGISTER_COUNT] = {0};
  VirtualMachine vm = {.regs = regs,
                       .stack = stack,
                       .constants = constants,
                       .constans_count = 0,
                       .ip = 0};

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

  dump(&vm);
}
