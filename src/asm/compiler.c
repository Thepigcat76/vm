#include "compiler.h"
#include "../bin/code.h"
#include "../bin/mem.h"
#include "ast.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Compiler compiler_new(VEC(CasmElement) elements, uint8_t *bytes) {
  return (Compiler){.elements = elements,
                    .bytes = bytes,
                    .bytesptr = 0,
                    .constants = 0,
                    .symbol_table = {.len = 0}};
}

Opcode ins_opcode(Instruction ins) {
  switch (ins.type) {
  case AST_INS_MOV_I2R:
    return OP_MOVI2R;
  case AST_INS_MOV_R2R:
    return OP_MOVR2R;
  case AST_INS_MOV_C2R:
    return OP_MOVM2R;
  case AST_INS_SYSCALL:
    return OP_SYSCALL;
  case AST_INS_DECL_BYTE:
    return OP_DECL_BYTE;
  case AST_INS_JMP:
    return OP_JMP;
  case AST_INS_JNE:
    return OP_JNE;
  case AST_INS_CMP:
    return OP_CMP;
  case AST_INS_DECL_STR:
    fprintf(stderr, "Tried to get opcode for AST_INS_DECL_STR\n");
    exit(1);
  }
}

static void emit(Compiler *compiler, uint8_t byte) {
  compiler->bytes[compiler->bytesptr++] = byte;
}

static void emit_op(Compiler *compiler, Instruction ins) {
  emit(compiler, ins_opcode(ins));
}

void symbol_table_insert(Compiler *compiler, char *key, uint32_t val) {
  size_t len = compiler->symbol_table.len;
  compiler->symbol_table.keys[len] = key;
  compiler->symbol_table.values[len] = val;
  compiler->symbol_table.len++;
}

static uint32_t symbol_table_get(Compiler *compiler, char *key) {
  size_t index = -1;
  for (size_t i = 0; i < compiler->symbol_table.len; i++) {
    if (strcmp(compiler->symbol_table.keys[i], key) == 0) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    fprintf(stderr, "Could not find key: %s\n", key);
    exit(1);
  }

  return compiler->symbol_table.values[index];
}

static void unpack_uint64(uint64_t val, uint8_t bytes[sizeof(uint64_t)]) {
  for (size_t i = 0; i < sizeof(uint64_t); i++) {
    bytes[i] = val >> (i * 8);
  }
}

static void compile_ins(Compiler *compiler, Instruction ins) {
  emit_op(compiler, ins);

  switch (ins.type) {
  case AST_INS_MOV_I2R: {
    emit(compiler, ins.var.mov_i2r.immediate);
    emit(compiler, ins.var.mov_i2r.reg);
    break;
  }
  case AST_INS_MOV_C2R: {
    uint64_t addr =
        DATA_MEM_START + symbol_table_get(compiler, ins.var.mov_c2r.ident);
    uint8_t bytes[sizeof(uint64_t)];
    unpack_uint64(addr, bytes);
    for (size_t i = 0; i < sizeof(uint64_t); i++) {
      emit(compiler, bytes[i]);
    }
    emit(compiler, ins.var.mov_c2r.reg);
    break;
  }
  case AST_INS_DECL_BYTE: {
    symbol_table_insert(compiler, ins.var.decl_byte.ident,
                        compiler->constants++);
    emit(compiler, ins.var.decl_byte.byte);
    break;
  }
  case AST_INS_JMP: {
    uint32_t label_id = symbol_table_get(compiler, ins.var.jmp.label);
    emit(compiler, label_id);
    break;
  }
  case AST_INS_JNE: {
    uint32_t label_id = symbol_table_get(compiler, ins.var.jne.label);
    emit(compiler, label_id);
    break;
  }
  case AST_INS_CMP: {
    uint8_t val0 = ins.var.cmp.val0;
    uint8_t val1 = ins.var.cmp.val1;
    emit(compiler, val0);
    emit(compiler, val1);
    break;
  }
  case AST_INS_MOV_R2R: {
    fprintf(stderr, "NYI - CANNOT COMPILE Mov R2R\n");
    exit(1);
    break;
  }
  case AST_INS_DECL_STR: {
    fprintf(stderr, "UNREACHABLE - CANNOT COMPILE DECL STR\n");
    exit(1);
    break;
  }
  case AST_INS_SYSCALL: {
    break;
  }
  }
}

static void compile_decl_str_ins(Compiler *compiler, DeclStrIns ins) {
  symbol_table_insert(compiler, ins.ident, compiler->constants++);

  size_t str_len = strlen(ins.str);
  char *str = ins.str;
  for (size_t i = 0; i < str_len; i++) {
    compiler->constants++;
    emit_op(compiler, (Instruction){.type = AST_INS_DECL_BYTE});
    emit(compiler, str[i]);
  }
}

static void compile_label(Compiler *compiler, Label label) {
  symbol_table_insert(compiler, label.name, compiler->bytesptr);
}

static void compile_elem(Compiler *compiler, CasmElement elem) {
  switch (elem.type) {
  case AST_LABEL:
    break;
  case AST_INSTRUCTION:
    if (elem.var.ins.type != AST_INS_DECL_STR) {
      compile_ins(compiler, elem.var.ins);
    } else {
      compile_decl_str_ins(compiler, elem.var.ins.var.decl_str);
    }
    break;
  }
}

void compile(Compiler *compiler) {
  for (size_t i = 0; i < compiler->elements.length; i++) {
    CasmElement elem = vec_get(CasmElement, &compiler->elements, i);
    compile_elem(compiler, elem);
  }
}

static void string_to_long(char *string, uint64_t *bytes) {
  for (int i = 0; i < sizeof(string); i++) {
    *bytes |= ((uint64_t)string[i]) << (i * 8);
  }
}

static void long_to_bytes(uint64_t long_value, uint8_t *bytes) {
  for (int i = 0; i < sizeof(long_value); i++) {
    bytes[i] = (uint8_t)((long_value >> (i * 8)) & 0xFF);
  }
}
