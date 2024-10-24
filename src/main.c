#include "asm/compiler.h"
#include "asm/runner.h"
#include "cli.h"

#include "surtils/src/generics/set.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static char *read_file_to_string(const char *filename);

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

static bool cmp_str(const string_t *s1, const string_t *s2) {
  return strcmp(*s1, *s2);
}

static void print_byte_code(Compiler *compiler) {
  for (size_t i = 0; i < compiler->bytesptr; i++) {
    printf("Ins: %d\n", compiler->bytes[i]);
  }
}

int main(int argc, char **argv) {
  char *file = parse_args(argc, argv);

  if (file == NULL) {
    return 0;
  }

  char *asm_file = read_file_to_string(file);

  if (asm_file == NULL) {
    return 0;
  }

  Lexer lexer = {.input = asm_file, .cur_pos = 0};

  Parser parser = {.lexer = &lexer,
                   .cur_tok = tokenize(&lexer),
                   .peek_tok = tokenize(&lexer)};

  vec_gt(CasmElement) *elems = parse_all(&parser);

  uint8_t bytes[2000] = {0};

  Compiler compiler = {.elements = elems, .bytes = bytes, .bytesptr = 0};

  compile(&compiler);

  print_byte_code(&compiler);

  Runner runner = {.labels = set_new_cmp(string_t, cmp_str)};

  run_asm(&runner, compiler.bytes, compiler.bytesptr);

  free(asm_file);

  vec_free(CasmElement, elems);
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