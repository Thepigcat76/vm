#pragma once

#include "ast.h"
#include "parser.h"

#include "surtils/src/generics/set.h"

#include <stdbool.h>

//  Required cuz importing unistd.h will cause a conflict with syscall
typedef __ssize_t ssize_t;
// Required for generics to work properly
typedef char *string_t;

DEFINE_SET_EXPORTS(string_t);

typedef struct {
    set_gt(string_t) *labels;
} Runner;

void run_asm(Runner *runner, uint8_t* byte_code, size_t byte_code_len);