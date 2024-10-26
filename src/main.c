#include "asm/lib.h"
#include "bin/lib.h"
#include "cli.h"
#include "shared.h"
#include "vm/dbg.h"
#include "vm/runner.h"
#include "vm/vm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static char *read_file_to_string(const char *filename);

int main(int argc, char **argv) {
  /*
  struct args arguments = parse_args(argc, argv);

  char *asm_file = read_file_to_string(arguments.filename);

  if (asm_file == NULL) {
    return 0;
  }

  uint8_t bytes[BYTECODE_SIZE] = {0};
  size_t bytes_len;

  switch (arguments.filetype) {
  case FT_BIN:
    bytes_len = parse_bin(asm_file, bytes);
    break;
  case FT_ASM:
    bytes_len = compile_asm(asm_file, bytes);
    break;
  }

  print_byte_code(bytes, bytes_len);

  save_byte_code(bytes, bytes_len);

  run_asm(bytes, bytes_len);

  free(asm_file);
  */
  uint8_t stack[STACK_SIZE] = {0};
  uint64_t regs[REGISTER_COUNT] = {0};

  VirtualMachine vm = {.regs = regs, .stack = stack};

  char *text = "Deez!";
  size_t text_len = strlen(text);
  for (size_t i = 0; i < text_len; i++) {
    vm.stack[i] = text[i];
  }
  mov_i2r(&vm, 1, RA0);
  mov_i2r(&vm, 0, RA1);
  mov_i2r(&vm, text_len, RA2);
  syscall(&vm);
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