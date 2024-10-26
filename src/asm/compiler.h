#pragma once

#include "ast.h"
#include "parser.h"

#include <stdint.h>
#include <surtils/src/generics/vec.h>

typedef struct {
  uint8_t *bytes;
  vec_gt(CasmElement) *elements;
  size_t bytesptr;
} Compiler;

Compiler compiler_new(vec_gt(CasmElement) *elements, uint8_t *bytes);

void compile(Compiler *compiler);

