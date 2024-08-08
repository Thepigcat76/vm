#include "asm/parser.h"
#include "vm/vm.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static char *read_file_to_string(const char *filename);

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
}

const char *to_string(CasmElementType type) {
  switch (type) {
  case AST_LABEL:
    return "LABEL";
  case AST_INSTRUCTION:
    return "INSTRUCTION";
  default:
    return "AA";
  }
}

int main(int argc, char **argv) {
  // run(100, 100);
  char *asm_file = read_file_to_string("main.casm");
  size_t asm_len = strlen(asm_file);

  Lexer lexer = {.input = asm_file, .cur_pos = 0};
  Token cur_tok = tokenize(&lexer);
  Token peek_tok = tokenize(&lexer);
  Parser parser = {
      .lexer = &lexer,
      .cur_tok = cur_tok,
      .peek_tok = peek_tok,
  };

  CasmElement elem = parse(&parser);
  CasmElement elem1 = parse(&parser);
  const char *str = to_string(elem.type);
  const char *str1 = to_string(elem1.type);
  printf("Element: %s\n", str);
  printf("Element: %s\n", str1);
}

static char *read_file_to_string(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return NULL;
  }

  // Seek to the end of the file to get the file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET); // Go back to the beginning of the file

  if (file_size < 0) {
    perror("Error determining file size");
    fclose(file);
    return NULL;
  }

  // Allocate memory for the file content + null terminator
  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }

  // Read the file content into the buffer
  size_t read_size = fread(buffer, 1, file_size, file);
  if (read_size != file_size) {
    perror("Error reading file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  // Null-terminate the string
  buffer[file_size] = '\0';

  fclose(file);
  return buffer;
}