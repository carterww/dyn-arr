#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../dyn-arr.h"
#include <stdarg.h>
#include <stdio.h>

struct user {
  char *name;
  uint8_t age;
};

int cmp_user(const void *u1, const void *u2);

void assert(int exp, const char * format, ...);

#endif // TEST_HELPER_H
