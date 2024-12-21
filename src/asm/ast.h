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

make_node(MovC2RIns, {
  char *ident;
  Register reg;
});

make_node(Syscall, {});

make_node(Label, { const char *name; });

typedef struct {
  enum {
    DATA_STRING,
    DATA_INT,
  } type;
  union {
    char *string;
    int64_t integer;
  } var;
} DataValue;

make_node(DeclByte, {
  char *ident;
  uint8_t byte;
});

make_node(DeclStr, {
  char *ident;
  char *str;
});

typedef struct {
  enum {
    AST_INS_MOV_I2R,
    AST_INS_MOV_R2R,
    AST_INS_MOV_C2R,
    AST_INS_SYSCALL,
    AST_INS_DECL_BYTE,
    AST_INS_DECL_STR,
  } type;
  union {
    MovI2RIns mov_i2r;
    MovR2RIns mov_r2r;
    MovC2RIns mov_c2r;
    Syscall syscall;
    DeclByte decl_byte;
    DeclStr decl_str;
  } var;
} Instruction;

typedef struct {
  enum {
    AST_LABEL,
    AST_INSTRUCTION
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
    char *constant;
  } var;
} Operand;
