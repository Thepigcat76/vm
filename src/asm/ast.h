#pragma once

#include <stdint.h>

#define OPERANDS 3

#define make_node(name, fields) typedef struct fields name;

typedef enum {
  RSP,
  RA0,
  RA1,
  RA2,
  RA3,
} Register;

make_node(MovI2RIns, {
  uint8_t immediate;
  Register reg;
});

make_node(MovR2RIns, {
  Register reg0;
  Register reg1;
});

typedef struct {
  enum {
    AST_INS_MOV_I2R,
    AST_INS_MOV_R2R,
    AST_INS_SYSCALL,
  } type;
  union {
    MovI2RIns mov_i2r;
    MovR2RIns mov_r2r;
  } var;
} Instruction;

typedef enum {
  AST_LABEL,
  AST_INSTRUCTION,
} CasmElementType;

make_node(Label, { char *name; });

typedef struct {
  CasmElementType type;
  union {
    Label label;
    Instruction ins;
  } var;
} CasmElement;
