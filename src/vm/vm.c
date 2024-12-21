#include "vm.h"
#include "../shared.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void syscall_exit(VirtualMachine *vm) {
  uint64_t exit_code = vm->regs[RA1];
  exit(exit_code);
}

static void syscall_print(VirtualMachine *vm) {
  uint64_t byte_ptr = vm->regs[RA1];
  uint64_t byte_len = vm->regs[RA2];

  char msg[byte_len];
  for (uint64_t i = byte_ptr; i < byte_ptr + byte_len; i++) {
    msg[i - byte_ptr] = (char)vm->constants[i];
  }
  msg[byte_ptr + byte_len] = '\0';
  puts(msg);
}

void mov_i2r(VirtualMachine *vm, uint8_t immediate, uint8_t reg) {
  vm->regs[reg] = immediate;
}

void mov_r2r(VirtualMachine *vm, uint8_t reg0, uint8_t reg1) {
  vm->regs[reg1] = vm->regs[reg0];
}

// TODO: Constant uint8_t needs to store info whether it is stack or constant
void mov_c2r(VirtualMachine *vm, uint8_t constant, uint8_t reg) {
  vm->regs[reg] = constant;
}

void syscall(VirtualMachine *vm) {
  uint64_t syscall = vm->regs[RA0];
  switch (syscall) {
  case SC_EXIT:
    syscall_exit(vm);
    break;
  case SC_PRINT:
    syscall_print(vm);
    break;
  }
}

void decl(VirtualMachine *vm, uint8_t val) {
  vm->constants[vm->constans_count++] = val;
}

static char *reg_to_string(int num) {
  switch (num) {
  case 0:
    return "rsp";
  case 1:
    return "ra0";
  case 2:
    return "ra1";
  case 3:
    return "ra2";
  case 4:
    return "ra3";
  default:
    return "unknown";
  }
}

void dump(VirtualMachine *vm) {
  printf("------ REGISTERS ------\n");
  for (int num_reg = 0; num_reg < REGISTER_COUNT; num_reg++) {
    printf("%s: %ld\n", reg_to_string(num_reg), vm->regs[num_reg]);
  }
  printf("------ REGISTERS ------\n");
  printf("------ CONSTANTS ------\n");
  for (int num_constant = 0; num_constant < vm->constans_count;
       num_constant++) {
    printf("%d: %d\n", num_constant, vm->constants[num_constant]);
  }
  printf("------ CONSTANTS ------\n");
}