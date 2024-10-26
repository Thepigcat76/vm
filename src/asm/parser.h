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

vec_gt(CasmElement) *parse_all(Parser *parser);

CasmElement parse(Parser *parser);
