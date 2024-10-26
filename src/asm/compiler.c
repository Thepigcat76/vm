#include "compiler.h"
#include "ast.h"
#include "../bin/code.h"

#include <stdbool.h>
#include <stdio.h>

Compiler compiler_new(vec_CasmElement_t *elements, uint8_t *bytes) {
  return (Compiler) {
    .elements = elements,
    .bytes = bytes,
    .bytesptr = 0,
  };
}

static Opcode ins_opcode(Instruction ins) {
  switch (ins.type) {
  case AST_INS_MOV_I2R:
    return OP_MOVI2R;
  case AST_INS_MOV_R2R:
    return OP_MOVR2R;
  case AST_INS_SYSCALL:
    return OP_SYSCALL;
  }
}

static void emit(Compiler *compiler, uint8_t byte) {
  compiler->bytes[compiler->bytesptr++] = byte;
}

static void emit_op(Compiler *compiler, Instruction ins) {
  emit(compiler, ins_opcode(ins));
}

static void compile_ins(Compiler *compiler, Instruction ins) {
  emit_op(compiler, ins);

  switch (ins.type) {
  case AST_INS_MOV_I2R:
    emit(compiler, ins.var.mov_i2r.immediate);
    emit(compiler, ins.var.mov_i2r.reg);
  case AST_INS_MOV_R2R:
  case AST_INS_SYSCALL:
    break;
  }
}

static void compile_elem(Compiler *compiler, CasmElement elem) {
  switch (elem.type) {
  case AST_LABEL:
    break;
  case AST_INSTRUCTION:
    compile_ins(compiler, elem.var.ins);
  }
}

void compile(Compiler *compiler) {
  for (size_t i = 0; i < compiler->elements->length; i++) {
    CasmElement elem = vec_get(CasmElement, compiler->elements, i);
    compile_elem(compiler, elem);
  }
}
