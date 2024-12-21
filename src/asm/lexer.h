#pragma once

#include <stdio.h>

typedef enum {
  // Constants
  TOK_DECL,

  // Symbols
  TOK_COLON,
  TOK_COMMA,

  // Instructions
  TOK_MOV,
  TOK_SYSCALL,

  // Literals
  TOK_IDENT,
  TOK_NUMBER,
  TOK_STRING,

  TOK_ILLEGAL,
} TokenType;

typedef struct {
  TokenType type;
  char *lit;
} Token;

typedef struct {
  const char *input;

  size_t cur_pos;
} Lexer;

Lexer lexer_new(const char *input);

Token tokenize(Lexer *lexer);

const char *tok_to_string(TokenType tok);