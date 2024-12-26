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

make_node(SyscallIns, {});

make_node(Label, { char *name; });

make_node(DeclByteIns, {
  char *ident;
  uint8_t byte;
});

make_node(DeclStrIns, {
  char *ident;
  char *str;
});

make_node(JmpIns, { char *label; });

make_node(JNeIns, { char *label; });

make_node(CmpIns, {
  uint8_t val0;
  uint8_t val1;
});

make_node(BinOpI2R, {
  BinOpType type;
  uint8_t val0;
  Register dest;
});

make_node(BinOpI2M, {
  BinOpType type;
  uint8_t val0;
  uint64_t addr;
});

make_node(BinOpI2C, {
  BinOpType type;
  uint8_t val0;
  char *ident;
});

typedef struct {
  enum {
    AST_INS_MOV_I2R,
    AST_INS_MOV_R2R,
    AST_INS_MOV_C2R,
    AST_INS_SYSCALL,
    AST_INS_DECL_BYTE,
    AST_INS_DECL_STR,
    AST_INS_JMP,
    AST_INS_CMP,
    AST_INS_JNE,
    AST_INS_BIN_I2R,
    AST_INS_BIN_I2M,
    AST_INS_BIN_I2C,
  } type;
  union {
    MovI2RIns mov_i2r;
    MovR2RIns mov_r2r;
    MovC2RIns mov_c2r;
    SyscallIns syscall;
    DeclByteIns decl_byte;
    DeclStrIns decl_str;
    JmpIns jmp;
    CmpIns cmp;
    JNeIns jne;
    BinOpI2R bin_i2r;
    BinOpI2M bin_i2m;
    BinOpI2C bin_i2c;
  } var;
} Instruction;

typedef struct {
  enum { AST_LABEL, AST_INSTRUCTION } type;
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
