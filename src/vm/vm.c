#include "vm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void syscall_exit(VirtualMachine *vm) {
  uint64_t exit_code = vm->regs[REG_RA1];
  exit(exit_code);
}

static void syscall_print(VirtualMachine *vm) {
  uint64_t byte_ptr = vm->regs[REG_RA1];
  uint64_t byte_len = vm->regs[REG_RA2];

  char msg[byte_len];
  for (uint64_t i = byte_ptr; i < byte_ptr + byte_len; i++) {
    msg[i - byte_ptr] = (char) vm->stack[i];
  }
  msg[byte_ptr+byte_len] = '\0';
  puts(msg);
}

static void mov_to_reg(VirtualMachine *vm, Value val, uint8_t dest) {
  switch (val.type) {
  case VAL_REGISTER:
    vm->regs[dest] = vm->regs[val.data.reg];
    break;
  case VAL_ADDRESS:
    vm->regs[dest] = val.data.addr;
    break;
  case VAL_LITERAL:
    vm->regs[dest] = val.data.lit;
    break;
  }
}

static void mov_to_addr(VirtualMachine *vm, Value val, uint64_t dest) {
  switch (val.type) {
  case VAL_REGISTER:
    vm->stack[dest] = vm->regs[val.data.reg];
    break;
  case VAL_ADDRESS:
    vm->stack[dest] = val.data.addr;
    break;
  case VAL_LITERAL:
    vm->stack[dest] = val.data.lit;
    break;
  }
}

void add(VirtualMachine *vm, uint64_t val0, uint64_t val1, Dest dest) {
  uint64_t res = val0 + val1;
  switch (dest.type) {
  case DEST_REGISTER:
    vm->regs[dest.data.reg] = res;
    break;
  case DEST_ADDRESS:
    vm->stack[dest.data.addr] = res;
    break;
  }
}

void move(VirtualMachine *vm, Value val, Dest dest) {
  switch (dest.type) {
  case DEST_REGISTER:
    mov_to_reg(vm, val, dest.data.reg);
    break;
  case DEST_ADDRESS:
    mov_to_addr(vm, val, dest.data.addr);
    break;
  }
}

void syscall(VirtualMachine *vm) {
  uint64_t syscall = vm->regs[REG_RA0];
  switch (syscall) {
  case SC_EXIT:
    syscall_exit(vm);
    break;
  case SC_PRINT:
    syscall_print(vm);
    break;
  }
}

void dump(VirtualMachine vm) {
  printf("------ REGISTERS ------\n");
  for (int num_reg = 0; num_reg < REG_AMOUNT; num_reg++) {
    printf("ra%d: %ld\n", num_reg, vm.regs[num_reg]);
  }
  printf("------ REGISTERS ------\n");
}