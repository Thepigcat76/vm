#pragma once

#include "ast.h"
#include "lexer.h"

#include "surtils/src/generics/vec.h"

DEFINE_VEC_EXPORTS(CasmElement);

typedef struct {
  Lexer *lexer;

  Token cur_tok;
  Token peek_tok;
} Parser;

Parser parser_new(Lexer *lexer);

VEC(CasmElement) parse_all(Parser *parser);

CasmElement parse(Parser *parser);

char *casm_element_to_string(CasmElement *casm_element);
