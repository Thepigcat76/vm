#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t parse_binary(const char *binary_str) {
  uint8_t result = 0;
  size_t length = strlen(binary_str);

  for (size_t i = 0; i < length; i++) {
    if (binary_str[i] != '0' && binary_str[i] != '1') {
      fprintf(stderr, "Could not parse binary string: %s\n", binary_str);
      exit(1);
    }

    result = (result << 1) | (binary_str[i] - '0');
  }

  return result;
}

size_t parse_bin(const char *file, uint8_t *bytes_buf) {
  size_t str_len = strlen(file);
  size_t i = 0;
  for (; i < str_len / 8; i++) {
    char byte[9];
    for (size_t j = 0; j < 8; j++) {
        byte[j] = file[i * 8 + j];
    }
    byte[8] = '\0';

    bytes_buf[i] = parse_binary(byte);
  }
  return i;
}