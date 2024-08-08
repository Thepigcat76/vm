#pragma once

#include "ast.h"
#include "lexer.h"
#include "../vechack.h"

typedef struct {
  Lexer *lexer;

  Token cur_tok;
  Token peek_tok;
} Parser;

vec_t *parse_all(Parser *parser);

CasmElement parse(Parser *parser);
