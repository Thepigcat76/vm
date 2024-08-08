#include "parser.h"
#include "lexer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void next_tok(Parser *parser);

static void mem_swap(void *a, void *b, size_t size);

static Operand parse_op(Parser *parser) {
  switch (parser->cur_tok.type) {
  case TOK_NUMBER: {
    int32_t num = atoi(parser->cur_tok.lit);
    return (Operand){.type = AST_OP_NUMBER, .var = {.number = num}};
  }
  case TOK_IDENT: {
    char *str = parser->cur_tok.lit;
    Register reg;
    if (strcmp(str, "rsp") == 0) {
      reg = RSP;
    } else if (strcmp(str, "ra0") == 0) {
      reg = RA0;
    } else if (strcmp(str, "ra1") == 0) {
      reg = RA1;
    } else if (strcmp(str, "ra2") == 0) {
      reg = RA2;
    } else if (strcmp(str, "ra3") == 0) {
      reg = RA3;
    }
    return (Operand) {.type = AST_OP_REGISTER, .var = {.reg = reg}};
  }
  default: {
    fprintf(stderr, "Error parsing operand from token: %s\n",
            tok_to_string(parser->cur_tok.type));
    exit(1);
  }
  }
}

static CasmElement parse_mov(Parser *parser) {
  next_tok(parser);
  Operand val = parse_op(parser);
  next_tok(parser);
  next_tok(parser);
  Operand dest = parse_op(parser);
  next_tok(parser);
  return (CasmElement){
      .type = AST_INSTRUCTION,
      .var = {.ins = {.type = AST_INS_MOV, .args = {val, dest}}}};
}

static CasmElement parse_syscall(Parser *parser) {
  next_tok(parser);
  return (CasmElement){.type = AST_INSTRUCTION,
                       .var = {.ins = {.type = AST_INS_SYSCALL, .args = {}}}};
}

static CasmElement parse_label(Parser *parser) {
  char *name = parser->cur_tok.lit;
  // colon
  next_tok(parser);
  next_tok(parser);
  return (CasmElement){.type = AST_LABEL, .var = {.label = {.name = name}}};
}

vec_t *parse_all(Parser *parser) {
  vec_t *vec = vec_new();
  size_t input_len = strlen(parser->lexer->input);
  while (parser->cur_tok.type != TOK_ILLEGAL) {
    CasmElement elem = parse(parser);
    vec_push_back(vec, elem);
  }
  return vec;
}

CasmElement parse(Parser *parser) {
  switch (parser->cur_tok.type) {
  case TOK_MOV:
    return parse_mov(parser);
  case TOK_SYSCALL:
    return parse_syscall(parser);
  case TOK_IDENT:
    return parse_label(parser);
  default:
    fprintf(stderr,
            "Error parsing token, invalid token that cannot be parsed: %s\n",
            tok_to_string(parser->cur_tok.type));
    exit(1);
  }
}

static void next_tok(Parser *parser) {
  mem_swap(&parser->cur_tok, &parser->peek_tok, sizeof(Token));
  parser->peek_tok = tokenize(parser->lexer);
}

static void mem_swap(void *a, void *b, size_t size) {
  void *temp = malloc(size); // Allocate memory for a temporary buffer
  if (temp == NULL) {
    // Handle memory allocation failure
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }

  memcpy(temp, a, size); // Copy contents of a to temp
  memcpy(a, b, size);    // Copy contents of b to a
  memcpy(b, temp, size); // Copy contents of temp to b

  free(temp); // Free the temporary buffer
}