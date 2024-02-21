#include "../dyn-arr.h"
#include "helpers.h"
#include "da_ops.h"

#define FLAGS 1

// Hide your eyes
int main(void) {
  printf("Testing da_new\n");
  struct dynamic_array *da = test_new(FLAGS);
  printf("Testing da_add\n");
  da = test_add(da, FLAGS);
  printf("Testing da_get\n");
  da = test_get(da, FLAGS);
  printf("Testing da_remove\n");
  da = test_rm(da, FLAGS);

  void *element;
  size_t iter = 0;
  while (da_iter(da, &iter, &element) == 1) {
    struct user *u = (struct user *)element;
    printf("User: %s, age: %d\n", u->name, u->age);
  }

  struct user u = {.age = 1, .name = "user3"};
  printf("adding user3\n");
  da_add(da, &u);
  u.name = "user2";
  printf("adding user2\n");
  da_add(da, &u);
  u.name = "cherry";
  printf("adding cherry\n");
  da_add(da, &u);
  u.name = "apple";
  printf("adding apple\n");
  da_add(da, &u);
  u.name = "xylophone";
  printf("adding xylophone\n");
  da_add(da, &u);

  printf("After adding more elements\n");
  iter = 0;
  while (da_iter(da, &iter, &element) == 1) {
    struct user *u = (struct user *)element;
    printf("User: %s, age: %d\n", u->name, u->age);
  }

  da_remove(da, &u);
  printf("Removed xylophone\n");
  u.name = "apple";
  da_remove(da, &u);
  printf("Removed apple\n");
  u.name = "user2";
  da_remove(da, &u);

  iter = 0;
  while (da_iter(da, &iter, &element) == 1) {
    struct user *u = (struct user *)element;
    printf("User: %s, age: %d\n", u->name, u->age);
  }

  da_clear(da);
  iter = 0;
  while (da_iter(da, &iter, &element) == 1) {
    struct user *u = (struct user *)element;
    printf("User: %s, age: %d\n", u->name, u->age);
  }

  da_free(da);

  return 0;
}
