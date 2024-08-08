#pragma once

#include <stdint.h>

#define OPERANDS 3

typedef enum {
  RSP,
  RA0,
  RA1,
  RA2,
  RA3,
} Register;

char* reg_to_string(int num);

typedef enum {
  AST_OP_NUMBER,
  AST_OP_REGISTER,
} OperandType;

typedef struct {
  OperandType type;
  union {
    int32_t number;
    Register reg;
  } var;
} Operand;

typedef enum {
  AST_INS_MOV,
  AST_INS_SYSCALL,
} InstructionType;

typedef struct {
  char *name;
} Label;

typedef struct {
  InstructionType type;
  Operand args[3];
} Instruction;

typedef enum {
  AST_LABEL,
  AST_INSTRUCTION,
} CasmElementType;

typedef struct {
  CasmElementType type;
  union {
    Label label;
    Instruction ins;
  } var;
} CasmElement;
