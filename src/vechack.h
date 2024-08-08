#pragma once

#include "asm/ast.h"
#include <stdlib.h>

typedef struct {
  CasmElement *data;
  size_t length;
  size_t capacity;
} vec_t;

vec_t *vec_new();

vec_t *vec_new_with_size(size_t initial_size);

void vec_push_back(vec_t *vec, CasmElement value);

void vec_push_front(vec_t *vect, CasmElement value);

CasmElement vec_remove(vec_t *vec, size_t index);

CasmElement vec_pop(vec_t *vec);

void vec_set(vec_t *vec, size_t index, CasmElement value);

CasmElement vec_get(const vec_t *vec, size_t index);

void vec_free(vec_t *vec);

size_t vec_length(const vec_t *vec);
