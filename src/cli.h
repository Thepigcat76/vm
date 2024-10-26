#pragma once

#define PCK_AUTHOR "Thepigcat76"
#define PCK_VERSION "0.1"

enum filetype {
  FT_BIN,
  FT_ASM,
};

struct args {
  enum filetype filetype;
  const char *filename;
};

struct args parse_args(int argc, char** argv);

void print_help(char* cmd);