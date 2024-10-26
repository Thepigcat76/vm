#pragma once

#include "../shared.h"

#include <stdint.h>

#define OPERANDS 3

#define make_node(name, fields) typedef struct fields name;

make_node(MovI2RIns, {
  uint8_t immediate;
  Register reg;
});

make_node(MovR2RIns, {
  Register reg0;
  Register reg1;
});

make_node(Syscall, {});

make_node(Label, { char *name; });

typedef struct {
  enum {
    AST_INS_MOV_I2R,
    AST_INS_MOV_R2R,
    AST_INS_SYSCALL,
  } type;
  union {
    MovI2RIns mov_i2r;
    MovR2RIns mov_r2r;
    Syscall syscall;
  } var;
} Instruction;

typedef struct {
  enum {
    AST_LABEL,
    AST_INSTRUCTION,
  } type;
  union {
    Label label;
    Instruction ins;
  } var;
} CasmElement;

typedef struct {
  enum {
    OP_REGISTER,
    OP_NUMBER,
    OP_CONST,
  } type;
  union {
    Register reg;
    uint8_t number;
    uint8_t constant;
  } var;
} Operand;
