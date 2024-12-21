#pragma once

#include "ast.h"
#include "parser.h"
#include "../bin/code.h"

#include <stdint.h>
#include <surtils/src/generics/vec.h>

typedef struct {
  // String identifier (ASM)
  char *keys[CONSTANT_COUNT];
  // Integer identifiers (BYTECODE)
  uint32_t values[CONSTANT_COUNT];
  size_t len;
} SymbolTable;

typedef struct {
  uint8_t *bytes;
  VEC(CasmElement) elements;
  size_t bytesptr;
  SymbolTable symbol_table;
  uint8_t constants;
} Compiler;

Compiler compiler_new(VEC(CasmElement) elements, uint8_t *bytes);

void compile(Compiler *compiler);

Opcode ins_opcode(Instruction ins);

void symbol_table_insert(Compiler *compiler, char *key, uint32_t val);
