#include "runner.h"
#include "../vm/vm.h"
#include <stdint.h>
#include <stdio.h>

static Value op_to_val(Operand *op) {
  switch (op->type) {
  case AST_OP_NUMBER:
    return (Value){.type = VAL_LITERAL, .data = {.lit = op->var.number}};
  case AST_OP_REGISTER:
    return (Value){.type = VAL_REGISTER, .data = {.lit = op->var.reg}};
  }
}

static Dest op_to_dest(Operand *op) {
  switch (op->type) {
  case AST_OP_REGISTER:
    return (Dest){.type = DEST_REGISTER, .data = {.reg = op->var.reg}};
  default:
    exit(1);
  }
}

static void run_ins(VirtualMachine *vm, CasmElement elem) {
  switch (elem.type) {
  case AST_LABEL:
    break;
  case AST_INSTRUCTION: {
    switch (elem.var.ins.type) {
    case AST_INS_MOV: {
      move(vm, op_to_val(&elem.var.ins.args[0]),
           op_to_dest(&elem.var.ins.args[1]));
      break;
    }
    case AST_INS_SYSCALL: {
      syscall(vm);
      break;
    }
    }
  }
  }
}

void run_asm(vec_t *elems) {
  uint8_t stack[STACK_SIZE] = {0};
  uint64_t regs[REG_AMOUNT] = {0};
  VirtualMachine vm = {.regs = regs, .stack = stack};

  for (size_t i = 0; i < elems->length; i++) {
    run_ins(&vm, vec_get(elems, i));
  }
}