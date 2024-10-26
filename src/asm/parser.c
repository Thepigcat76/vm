#include "parser.h"
#include "../shared.h"
#include "ast.h"
#include "lexer.h"

#include "surtils/src/generics/vec.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DEFINE_VEC(CasmElement);

static void next_tok(Parser *parser);

static void mem_swap(void *a, void *b, size_t size);

Parser parser_new(Lexer *lexer) {
  return (Parser){
      .lexer = lexer,
      .cur_tok = tokenize(lexer),
      .peek_tok = tokenize(lexer),
  };
}

static Operand parse_operand(Parser *parser) {
  switch (parser->cur_tok.type) {
  case TOK_NUMBER: {
    char *literal = parser->cur_tok.lit;
    int32_t num = atoi(literal);
    free(literal);
    return (Operand){.type = OP_NUMBER, .var = {.number = num}};
  }
  case TOK_IDENT: {
    char *str = parser->cur_tok.lit;
    Register reg;
    for (size_t i = 0; i < REGISTER_COUNT; i++) {
      if (strcmp(str, REGISTERS[i]) == 0) {
        reg = (Register)i;
      }
    }
    free(str);
    // TODO: Constants
    return (Operand){.type = OP_REGISTER, .var = {.reg = reg}};
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
  Operand val = parse_operand(parser);
  next_tok(parser);
  next_tok(parser);
  Operand dest = parse_operand(parser);
  next_tok(parser);
  switch (val.type) {
  case OP_NUMBER: {
    if (dest.type == OP_REGISTER) {
      return (CasmElement){
          .type = AST_INSTRUCTION,
          .var = {.ins = {.type = AST_INS_MOV_I2R,
                          .var = {.mov_i2r = {.immediate = val.var.number,
                                              .reg = dest.var.reg}}}}};
    }
    fprintf(stderr, "Error parsing mov instruction, with immediate value: %s\n",
            tok_to_string(parser->cur_tok.type));
    exit(1);
  }
  case OP_REGISTER: {
    if (dest.type == OP_REGISTER) {
      return (CasmElement){
          .type = AST_INSTRUCTION,
          .var = {.ins = {.type = AST_INS_MOV_I2R,
                          .var = {.mov_r2r = {.reg0 = val.var.reg,
                                              .reg1 = dest.var.reg}}}}};
      fprintf(stderr,
              "Error parsing mov instruction, with register value: %s\n",
              tok_to_string(parser->cur_tok.type));
      exit(1);
    }
  }
  case OP_CONST:
    fprintf(stderr,
            "Error parsing mov instruction, const is not implemented yet: %s\n",
            tok_to_string(parser->cur_tok.type));
    exit(1);
  }
}

static CasmElement parse_syscall(Parser *parser) {
  next_tok(parser);
  return (CasmElement){.type = AST_INSTRUCTION,
                       .var = {.ins = {.type = AST_INS_SYSCALL, .var = {}}}};
}

static CasmElement parse_label(Parser *parser) {
  char *name = parser->cur_tok.lit;
  // colon
  next_tok(parser);
  next_tok(parser);
  return (CasmElement){.type = AST_LABEL, .var = {.label = {.name = name}}};
}

vec_gt(CasmElement)* parse_all(Parser *parser) {
  vec_gt(CasmElement) *vec = vec_new(CasmElement);
  size_t input_len = strlen(parser->lexer->input);
  while (parser->cur_tok.type != TOK_ILLEGAL) {
    CasmElement elem = parse(parser);
    vec_push_back(CasmElement, vec, elem);
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