#pragma once

#include <stdio.h>

typedef enum {
  // Symbols
  TOK_COLON,
  TOK_COMMA,

  // Instructions
  TOK_MOV,
  TOK_SYSCALL,

  // Literals
  TOK_IDENT,
  TOK_NUMBER,

  TOK_ILLEGAL,
} TokenType;

typedef struct {
  TokenType type;
  char *lit;
} Token;

typedef struct {
  char *input;

  size_t cur_pos;
} Lexer;

Token tokenize(Lexer *lexer);

const char *tok_to_string(TokenType tok);