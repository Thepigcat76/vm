#pragma once

#include <stdint.h>
#include <stdlib.h>

// returns amount of bytes
size_t compile_asm(char *file, uint8_t *bytes_buf);
