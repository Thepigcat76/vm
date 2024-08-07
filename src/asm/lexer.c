#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token determine_ident(char *str) {
  if (strcmp(str, "mov") == 0) {
    free(str);
    return (Token){.type = TOK_MOV, .lit = "mov"};
  } else if (strcmp(str, "syscall") == 0) {
    free(str);
    return (Token){.type = TOK_SYSCALL, .lit = "syscall"};
  } else {
    return (Token){.type = TOK_IDENT, .lit = str};
  }
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

    char *str = malloc(lexer->cur_pos - start_pos);

    for (size_t i = start_pos; i < lexer->cur_pos; i++) {
      str[i - start_pos] = lexer->input[i];
    }
    str[lexer->cur_pos - start_pos] = '\0';

    return determine_ident(str);
  } else if (isdigit(cur_ch)) {
    size_t start_pos = lexer->cur_pos;

    while (isdigit(lexer->input[lexer->cur_pos])) {
      lexer->cur_pos++;
    }

    // TODO: Free this
    char *str = malloc(lexer->cur_pos - start_pos);

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
  case TOK_ILLEGAL:
    return "ILLEGAL";
  }
}