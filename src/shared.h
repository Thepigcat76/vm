#pragma once

#define new(type_name, fields) (type_name) fields

// TODO: Move this to vm.h

#define STACK_SIZE 10000
#define REGISTER_COUNT 5
#define CONSTANT_COUNT 1000

extern const char *REGISTERS[];

typedef enum {
  RSP,
  RA0,
  RA1,
  RA2,
  RA3,
} Register;

typedef enum {
  BIN_OP_ADD,
  BIN_OP_SUB,
  BIN_OP_MUL,
  BIN_OP_DIV,
} BinOpType;
