#include "vm.h"
#include <stdint.h>
#include <stdio.h>

void int_to_str(int num, char *str);

void run(size_t regs_size, size_t stack_size) {
  uint64_t regs[regs_size];
  uint8_t stack[stack_size];
  
  VirtualMachine vm = {.regs = regs, .stack = stack};

  // Allocate string on the stack
  uint64_t stack_ptr = vm.regs[RSP];
  move(&vm, VAL_LIT('H'), DEST_ADDR(stack_ptr));
  move(&vm, VAL_LIT('e'), DEST_ADDR(stack_ptr + 1));
  move(&vm, VAL_LIT('l'), DEST_ADDR(stack_ptr + 2));
  move(&vm, VAL_LIT('l'), DEST_ADDR(stack_ptr + 3));
  move(&vm, VAL_LIT('o'), DEST_ADDR(stack_ptr + 4));

  // Syscall to print string
  move(&vm, VAL_LIT(SC_PRINT), DEST_REG(RA0));
  move(&vm, VAL_LIT(stack_ptr), DEST_REG(RA1));
  move(&vm, VAL_LIT(5), DEST_REG(RA2));
  syscall(&vm);

  add(&vm, 100, 200, DEST_REG(RA0));
  char buf[4];
  int_to_str(vm.regs[RA0], buf);

  for (uint8_t i = 0; i < 3; i++) {
    move(&vm, VAL_LIT(buf[i]), DEST_ADDR(stack_ptr + i));
  }

  // Syscall to print string
  move(&vm, VAL_LIT(SC_PRINT), DEST_REG(RA0));
  move(&vm, VAL_LIT(stack_ptr), DEST_REG(RA1));
  move(&vm, VAL_LIT(3), DEST_REG(RA2));
  syscall(&vm);
}

int main(int argc, char **argv) { run(100, 100); }

void int_to_str(int num, char *str) {
  int i = 0, j, len, isNegative = 0;

  // Handle 0 explicitly
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  // Handle negative numbers
  if (num < 0) {
    isNegative = 1;
    num = -num; // Convert to positive for further processing
  }

  // Extract digits and store in reverse order
  while (num != 0) {
    str[i++] = (num % 10) + '0'; // Get the last digit and convert to character
    num /= 10;                   // Remove the last digit
  }

  // Add negative sign if needed
  if (isNegative) {
    str[i++] = '-';
  }

  str[i] = '\0'; // Null-terminate the string

  // Reverse the string
  len = i;
  for (j = 0; j < len / 2; j++) {
    char temp = str[j];
    str[j] = str[len - j - 1];
    str[len - j - 1] = temp;
  }
}
