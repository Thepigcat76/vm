#pragma once

#define new(type_name, fields) (type_name) fields

// TODO: Move this to vm.h

#define STACK_SIZE 10000
#define REGISTER_COUNT 5

extern const char *REGISTERS[];

typedef enum {
  RSP,
  RA0,
  RA1,
  RA2,
  RA3,
} Register;
