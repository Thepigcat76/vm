#pragma once

#include "ast.h"
#include <stdint.h>
#include "parser.h"

typedef struct {
  uint8_t *bytes;
  vec_gt(CasmElement) *elements;
  size_t bytesptr;
} Compiler;

void compile(Compiler *compiler);

