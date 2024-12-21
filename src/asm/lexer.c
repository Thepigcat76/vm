#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_TYPES 100

typedef struct {
  TokenType tok_types[TOKEN_TYPES];
  char *literals[TOKEN_TYPES];
  size_t len;
} TokenTable;

static TokenTable TOKENS = {.len = 0};

void create_token(TokenType tok_type, char *literal) {
  size_t index = TOKENS.len;
  TOKENS.tok_types[index] = tok_type;
  TOKENS.literals[index] = literal;
  TOKENS.len++;
}

Lexer lexer_new(char *input) {
  create_token(TOK_MOV, "mov");
  create_token(TOK_SYSCALL, "syscall");
  create_token(TOK_DECL, "decl");
  create_token(TOK_JMP, "jmp");
  create_token(TOK_CMP, "cmp");
  create_token(TOK_JNE, "jne");
  return (Lexer){
      .input = input,
      .cur_pos = 0,
  };
}

static Token determine_ident(const char *str, size_t str_len) {
  for (size_t i = 0; i < TOKENS.len; i++) {
    if (strcmp(str, TOKENS.literals[i]) == 0) {
      return (Token){.type = TOKENS.tok_types[i], .lit = TOKENS.literals[i]};
    }
  }
  
  char *ident = malloc(str_len + 1);
  strcpy(ident, str);
  return (Token){.type = TOK_IDENT, .lit = ident};
}

static Token tokenize_string(Lexer *lexer) {
  // Quotation mark
  lexer->cur_pos++;

  size_t string_len = 0;
  size_t peek_pos = lexer->cur_pos + 1;
  while (lexer->input[peek_pos] != '"') {
    string_len++;
    peek_pos++;
  }
  string_len++;
  peek_pos++;

  char *string = malloc(string_len + 1);

  for (size_t i = 0; i < string_len; i++) {
    string[i] = lexer->input[lexer->cur_pos + i];
  }
  string[string_len] = '\0';
  lexer->cur_pos = peek_pos;

  return (Token){
      .type = TOK_STRING,
      .lit = string,
  };
}

static Token tokenize_other(Lexer *lexer) {
  char cur_ch = lexer->input[lexer->cur_pos];
  if (isalpha(cur_ch) || cur_ch == '_') {
    size_t start_pos = lexer->cur_pos;

    while (isalpha(lexer->input[lexer->cur_pos]) ||
           isdigit(lexer->input[lexer->cur_pos]) ||
           lexer->input[lexer->cur_pos] == '_') {
      lexer->cur_pos++;
    }

    char str[lexer->cur_pos - start_pos];

    for (size_t i = start_pos; i < lexer->cur_pos; i++) {
      str[i - start_pos] = lexer->input[i];
    }
    str[lexer->cur_pos - start_pos] = '\0';

    return determine_ident(str, lexer->cur_pos - start_pos);
  } else if (isdigit(cur_ch)) {
    size_t start_pos = lexer->cur_pos;

    while (isdigit(lexer->input[lexer->cur_pos])) {
      lexer->cur_pos++;
    }

    char *str = malloc(lexer->cur_pos - start_pos + 1);

    for (size_t i = start_pos; i < lexer->cur_pos; i++) {
      str[i - start_pos] = lexer->input[i];
    }
    str[lexer->cur_pos - start_pos] = '\0';

    return (Token){.type = TOK_NUMBER, .lit = str};
  }
  return (Token){.type = TOK_ILLEGAL, .lit = ""};
}

Token tokenize(Lexer *lexer) {
  switch (lexer->input[lexer->cur_pos]) {
  case '\n':
  case ' ':
    lexer->cur_pos++;
    return tokenize(lexer);
  case ':':
    lexer->cur_pos++;
    return (Token){.type = TOK_COLON, .lit = ":"};
  case ',':
    lexer->cur_pos++;
    return (Token){.type = TOK_COMMA, .lit = ","};
  case '"':
    return tokenize_string(lexer);
  default:
    return tokenize_other(lexer);
  }
}

const char *tok_to_string(TokenType tok) {
  switch (tok) {
  case TOK_COLON:
    return "COLON";
  case TOK_COMMA:
    return "COMMA";
  case TOK_MOV:
    return "MOV";
  case TOK_SYSCALL:
    return "SYSCALL";
  case TOK_IDENT:
    return "IDENT";
  case TOK_NUMBER:
    return "NUMBER";
  case TOK_STRING:
    return "STRING";
  case TOK_DECL:
    return "DECL";
  case TOK_ILLEGAL:
    return "ILLEGAL";
  case TOK_JMP:
    return "JMP";
  default:
    return "UNKNOWN";
  }
}