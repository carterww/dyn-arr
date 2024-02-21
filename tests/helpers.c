#include "helpers.h"
#include <string.h>

void assert(int exp, const char *format, ...) {
  if (!exp) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    exit(1);
  }
}

int cmp_user(const void *d1, const void *d2) {
  struct user *u1 = (struct user *)d1;
  struct user *u2 = (struct user *)d2;
  return strcmp(u1->name, u2->name);
}
