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

char *parse_args(int argc, char **argv) {
  argc -= 1; // so ["vm.exe", "main.casm"] gets to ["main.casm"]

  if (argc == 1) {
    char *file = argv[1];
    if (strcmp(file, "-h") == 0 || strcmp(file, "--help") == 0) {
      print_help(argv[0]);
    } else if (strcmp(file, "-v") == 0 || strcmp(file, "--version") == 0) {
      print_version();
    } else {
      return file;
    }
  } else if (argc == 0) {
    print_help(argv[0]);
  }

  return NULL;
}