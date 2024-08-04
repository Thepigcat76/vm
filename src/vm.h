#pragma once

#include <stdint.h>

#define STACK_SIZE 10000
#define REG_AMOUNT 5

#define RSP 0
#define RA0 1
#define RA1 2
#define RA2 3
#define RA3 4

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


typedef union {
  uint8_t reg;
  uint64_t addr;
  uint8_t lit;
} ValueData;

typedef enum {
  VAL_REGISTER,
  VAL_ADDRESS,
  VAL_LITERAL,
} ValueType;

typedef struct {
  ValueData data;
  ValueType type;
} Value;


typedef union {
  uint8_t reg;
  uint64_t addr;
} DestData;

typedef enum {
  DEST_REGISTER,
  DEST_ADDRESS,
} DestType;

typedef struct {
  DestData data;
  DestType type;
} Dest;

VirtualMachine new_vm();

void add(VirtualMachine *vm, uint64_t val0, uint64_t val1, Dest dest);

void move(VirtualMachine *vm, Value val, Dest dest);

void syscall(VirtualMachine *vm);
