#include "dbg.h"
#include "../bin/code.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_byte_code(const uint8_t *bytes, size_t bytes_len) {
  size_t i = 0;
  while (i < bytes_len) {
    uint8_t opcode = bytes[i];
    size_t len = ins_len(opcode);
    printf("Ins:");
    for (size_t j = 0; j < len; j++) {
      printf(" %d,", bytes[i + j]);
    }
    i += len;
    putchar('\n');
  }
}

static void byte_to_bin(char *buf, uint8_t num) {
  buf[0] = '\0';
  // Assuming we want to print a 32-bit representation
  for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
    // Print each bit starting from the highest bit
    strcat(buf, (num & (1 << i)) ? "1" : "0");
  }
}

void save_byte_code(const uint8_t *bytes, size_t bytes_len) {
  char bytecode[BYTECODE_SIZE * 2];

  size_t i = 0;
  while (i < bytes_len) {
    uint8_t opcode = bytes[i];
    size_t len = ins_len(opcode);

    for (size_t j = 0; j < len; j++) {
      char byte[9];
      uint8_t cur_byte = bytes[i + j];
      byte_to_bin(byte, cur_byte);
      if (strlen(bytecode) + strlen(byte) < sizeof(bytecode)) {
        strcat(bytecode, byte);
      } else {
        printf("Error: Too much code :p. bytecode: %s\n", bytecode);
        exit(1);
      }
    }
    i += len;
  }

  FILE *file = fopen("out.basm", "w");
  fprintf(file, "%s", bytecode);
}