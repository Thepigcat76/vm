#include "asm/ast.h"
#include "asm/lexer.h"
#include "asm/lib.h"
#include "asm/parser.h"
#include "bin/lib.h"
#include "cli.h"

#include "surtests/src/tests.h"
#include "surtils/src/generics/vec.h"
#include "surtils/src/generics/iter.h"
#include "vm/runner.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BYTECODE_SIZE 2000

DEFINE_VEC_EXPORTS(Token)

DEFINE_VEC(Token)

static char *read_file_to_string(const char *filename);

int run(int argc, char **argv) {
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

  run_asm(bytes, bytes_len);

  free(asm_file);

  return 0;
}

static int test_vm_data_sec();

int main(int argc, char **argv) {
  // Only run if no tests are running
  const char *env = getenv("SURTUR_TESTS");
  if (env == NULL) {
    return run(argc, argv);
  }

  // clang-format off
  const TokenType asm_lexer_start_expected[] = {
      TOK_IDENT,
      TOK_COLON,
      TOK_MOV,
      TOK_NUMBER,
      TOK_COMMA,
      TOK_IDENT,
      TOK_MOV,
      TOK_IDENT,
      TOK_COMMA,
      TOK_IDENT,
      TOK_MOV,
      TOK_IDENT,
      TOK_COMMA,
      TOK_IDENT,
      TOK_SYSCALL
  };
  // clang-format on

  TEST(ASM_LEXER_START, {
    char *file = read_file_to_string("tests/lexer_start.casm");

    if (file == NULL) {
      perror("Could not find specified file\n");
      return 1;
    }

    Lexer lexer = lexer_new(file);
    Token tok = tokenize(&lexer);
    size_t i = 0;
    while (tok.type != TOK_ILLEGAL) {
      ASSERT(tok.type == asm_lexer_start_expected[i]);
      printf("Token: %s, Literal: %s\n", tok_to_string(tok.type), tok.lit);
      tok = tokenize(&lexer);
      i++;
    }
  });

  // clang-format off
  const TokenType asm_lexer_data_sec_expected[] = {
    TOK_IDENT,
    TOK_COLON,
    TOK_DECL,
    TOK_IDENT,
    TOK_COMMA,
    TOK_STRING,
    TOK_DECL,
    TOK_IDENT,
    TOK_COMMA,
    TOK_NUMBER
  };
  // clang-format on

  TEST(ASM_LEXER_DATA_SEC, {
    char *file = read_file_to_string("tests/lexer_data.casm");

    if (file == NULL) {
      perror("Could not find specified file\n");
      return 1;
    }

    Lexer lexer = lexer_new(file);
    Token tok = tokenize(&lexer);
    size_t i = 0;
    while (tok.type != TOK_ILLEGAL) {
      printf("type: %s, expected: %s, literal: %s\n", tok_to_string(tok.type), tok_to_string(asm_lexer_data_sec_expected[i]), tok.lit);
      ASSERT(tok.type == asm_lexer_data_sec_expected[i]);
      printf("Token: %s, Literal: %s\n", tok_to_string(tok.type), tok.lit);
      tok = tokenize(&lexer);
      i++;
    }
  });

  // clang-format off
  const TokenType asm_parser_data_sec_expected[] = {
    TOK_IDENT,
    TOK_COLON,
    TOK_DECL,
    TOK_IDENT,
    TOK_COMMA,
    TOK_STRING,
    TOK_DECL,
    TOK_IDENT,
    TOK_COMMA,
    TOK_NUMBER
  };
  // clang-format on

  TEST(ASM_PARSER_DATA_SEC, {
    char *file = read_file_to_string("tests/lexer_data.casm");

    if (file == NULL) {
      perror("Could not find specified file\n");
      return 1;
    }

    Lexer lexer = lexer_new(file);
    Parser parser = parser_new(&lexer);
    VEC(CasmElement) ast = parse_all(&parser);
    FOREACH(CasmElement, ast, elem, {
      printf("%s\n", casm_element_to_string(&elem));
    });
    free(ast.data);
  });
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