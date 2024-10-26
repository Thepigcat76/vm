#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(char *cmd) {
  printf("VM project by %s\n", PCK_AUTHOR);
  printf("\n");
  printf("USAGE: %s <asmfile name>\n", cmd);
  printf("\n");
  printf("OPTIONS:\n");
  printf("    -h, --help    Displays this help message\n");
  printf("    -v, --version Displays the version\n");
  printf("\n");
}

void print_version() {
  printf("VM project v%s by %s\n", PCK_VERSION, PCK_AUTHOR);
}

static void reverse_string(char *str) {
  int n = strlen(str); // Get the length of the string
  for (int i = 0; i < n / 2; i++) {
    // Swap characters
    char temp = str[i];
    str[i] = str[n - i - 1];
    str[n - i - 1] = temp;
  }
}

static enum filetype filetype_from_name(const char *filename) {
  size_t str_len = strlen(filename);
  char rev_name[str_len];
  strcpy(rev_name, filename);
  reverse_string(rev_name);

  char extension[str_len];
  size_t i = 0;
  for (; i < str_len; i++) {
    if (rev_name[i] != '.') {
      extension[i] = rev_name[i];
    } else {
      break;
    }
  }
  extension[i] = '\0';
  reverse_string(extension);
  
  if (strcmp(extension, "casm") == 0) {
    return FT_ASM;
  } else if (strcmp(extension, "basm") == 0) {
    return FT_BIN;
  }
  fprintf(stderr, "Invalid file extension: .%s\n", extension);
  exit(1);
}

struct args parse_args(int argc, char **argv) {
  argc -= 1; // so ["vm.exe", "main.casm"] gets to ["main.casm"]

  if (argc == 1) {
    char *file = argv[1];
    if (strcmp(file, "-h") == 0 || strcmp(file, "--help") == 0) {
      print_help(argv[0]);
    } else if (strcmp(file, "-v") == 0 || strcmp(file, "--version") == 0) {
      print_version();
    } else {
      enum filetype file_type = filetype_from_name(file);
      return (struct args){
          .filename = file,
          .filetype = file_type,
      };
    }
  } else if (argc == 0) {
    print_help(argv[0]);
  }

  fprintf(stderr, "Issue parsing args. Too many args supplied");
  exit(1);
}