#pragma once

#include <stdint.h>
#include <stdio.h>

#define SC_EXIT 0
#define SC_PRINT 1

// Integer usage:
// uint8_t indicates a single byte
// uint16_t indicates a stack location
//
// uint64_t indicates a memory address/register

typedef uint8_t *VMInstruction;

typedef struct {
  uint8_t *stack;
  uint64_t *regs;
  uint8_t *constants;
  size_t constans_count;

  VMInstruction *instructions;
  // Pointer to the current instruction
  size_t ip;
} VirtualMachine;

void mov_i2r(VirtualMachine *vm, uint8_t immediate, uint8_t reg);

void mov_r2r(VirtualMachine *vm, uint8_t reg0, uint8_t reg1);

void mov_c2r(VirtualMachine *vm, uint8_t constant, uint8_t reg);

void syscall(VirtualMachine *vm);

void decl(VirtualMachine *vm, uint8_t val);

void dump(VirtualMachine *vm);
