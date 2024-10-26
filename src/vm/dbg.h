#pragma once

#include "../bin/code.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTECODE_SIZE 2000

void print_byte_code(const uint8_t *bytes, size_t bytes_len);

void save_byte_code(const uint8_t *bytes, size_t bytes_len);