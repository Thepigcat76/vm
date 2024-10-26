#include "lib.h"
#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include <surtils/src/generics/vec.h>

size_t compile_asm(const char *file, uint8_t *bytes_buf) {
  Lexer lexer = lexer_new(file);
  Parser parser = parser_new(&lexer);

  vec_gt(CasmElement) *elements = parse_all(&parser);

  Compiler compiler = compiler_new(elements, bytes_buf);

  compile(&compiler);

  return compiler.bytesptr;
}