#include "lib.h"
#include "ast.h"
#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <surtils/src/generics/iter.h>
#include <surtils/src/generics/vec.h>

size_t compile_asm(char *file, uint8_t *bytes_buf) {
  Lexer lexer = lexer_new(file);
  Parser parser = parser_new(&lexer);

  VEC(CasmElement) elements = parse_all(&parser);

  Compiler compiler = compiler_new(elements, bytes_buf);

  size_t total_len = 0;

  FOREACH(CasmElement, compiler.elements, elem, {
    if (elem.type == AST_INSTRUCTION) {
      if (elem.var.ins.type != AST_INS_DECL_STR) {
        Opcode opcode = ins_opcode(elem.var.ins);
        size_t len = ins_len(opcode);
        total_len += len;
      } else {
        size_t len = strlen(elem.var.ins.var.decl_str.str) * 2;
        total_len += len;
      }
    } else if (elem.type == AST_LABEL) {
      symbol_table_insert(&compiler, elem.var.label.name, total_len);
    }
  });

  compile(&compiler);

  for (int32_t i = 0; i < compiler.symbol_table.len; i++) {
    printf("K: %s, V: %u\n", compiler.symbol_table.keys[i],
           compiler.symbol_table.values[i]);
  }

  return compiler.bytesptr;
}