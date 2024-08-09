#pragma once

#include <stdint.h>

#define STACK_SIZE 10000
#define REG_AMOUNT 5

#define REG_RSP 0
#define REG_RA0 1
#define REG_RA1 2
#define REG_RA2 3
#define REG_RA3 4

#define SC_EXIT 0
#define SC_PRINT 1

#define VAL_REG(reg_val)                                                           \
  (Value) { .data = {.reg = reg_val}, .type = VAL_REGISTER }
#define VAL_ADDR(addr_val)                                                         \
  (Value) { .data = {.addr = addr_val}, .type = VAL_ADDRESS }
#define VAL_LIT(lit_val)                                                           \
  (Value) { .data = {.lit = lit_val}, .type = VAL_LITERAL }

#define DEST_REG(reg_val)                                                       \
  (Dest) { .data = {.reg = reg_val}, .type = DEST_REGISTER }
#define DEST_ADDR(addr_val)                                                     \
  (Dest) { .data = {.addr = addr_val}, .type = DEST_ADDRESS }

// Integer usage:
// uint8_t indicates a single byte
// uint16_t indicates a stack location
//
// uint64_t indicates a memory address/register

typedef struct {
  uint8_t *stack;
  uint64_t *regs;
} VirtualMachine;

typedef enum {
  VAL_REGISTER,
  VAL_ADDRESS,
  VAL_LITERAL,
} ValueType;

typedef struct {
  union {
    uint8_t reg;
    uint64_t addr;
    uint8_t lit;
  } data;
  ValueType type;
} Value;

typedef enum {
  DEST_REGISTER,
  DEST_ADDRESS,
} DestType;

typedef struct {
  union {
    uint8_t reg;
    uint64_t addr;
  } data;
  DestType type;
} Dest;

void add(VirtualMachine *vm, uint64_t val0, uint64_t val1, Dest dest);

void move(VirtualMachine *vm, Value val, Dest dest);

void syscall(VirtualMachine *vm);

void dump(VirtualMachine *vm);


char* reg_to_string(int num);
