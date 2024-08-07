#pragma once

#include "ast.h"
#include "lexer.h"

typedef struct {
  Lexer *lexer;

  Token cur_tok;
  Token peek_tok;
} Parser;

vec_gt(CasmElement) *parse_all(Parser *parser);

CasmElement parse(Parser *parser);
