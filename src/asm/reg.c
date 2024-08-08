#include "ast.h"

char* reg_to_string(int num) {
    switch (num) {
        case 0: return "rsp";
        case 1: return "ra0";
        case 2: return "ra1";
        case 3: return "ra2";
        case 4: return "ra3";
        default: return "unknown";
    }
}