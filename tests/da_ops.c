#include "da_ops.h"
#include "../dyn-arr.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 1

struct dynamic_array *test_new(int flags) {
  struct dynamic_array *da =
      da_new(sizeof(struct user), DEFAULT_SIZE, NULL, cmp_user, flags);
  assert(da != NULL, "da_new failed allocating DA");
  assert(da->elsize == sizeof(struct user), "da_new failed setting elsize");
  assert(da->cap == DEFAULT_SIZE, "da_new failed setting cap to %lu",
         DEFAULT_SIZE);
  assert(da->free_func == NULL, "da_new failed setting free_func");
  assert(da->cmp_func != NULL, "da_new failed setting cmp_func");
  assert(da->flags == flags, "da_new failed setting flags");
  return da;
}

struct dynamic_array *test_add(struct dynamic_array *da, int flags) {
  if (!da)
    da = test_new(flags);
  struct user u = {.age = 1, .name = "user3"};
  const void *added = da_add(da, &u);
  assert(added != NULL, "da_add failed adding element");
  assert(da->len == 1, "da_add failed setting len");
  assert(da->cap == 1, "da_add changed cap when it shouldn't");

  struct user u2 = {.age = 2, .name = "user2"};
  added = da_add(da, &u2);
  assert(added != NULL, "da_add failed adding element");
  assert(da->len == 2, "da_add failed setting len");
  assert(da->cap != 1, "da_add failed setting cap");

  struct user u3 = {.age = 3, .name = "user1"};
  added = da_add(da, &u3);
  assert(added != NULL, "da_add failed adding element");
  assert(da->len == 3, "da_add failed setting len");

  if (flags & FLAG_SORTED_ARR) {
    for (size_t i = 0; i < da->len - 1; ++i) {
      assert(da->cmp_func(da->arr + i * da->elsize,
                          da->arr + (i + 1) * da->elsize) < 0,
             "da_add failed adding element in sorted order");
    }
  }
  return da;
}

struct dynamic_array *test_get(struct dynamic_array *da, int flags) {
  if (!da)
    da = test_add(NULL, flags);

  struct user uget = {.age = 1, .name = "user3"};
  struct user *u = (struct user *)da_get(da, &uget);
  assert(u != NULL, "da_get failed getting element");
  assert(strcmp(u->name, uget.name) == 0, "da_get failed getting correct element");
  assert(u->age == uget.age, "da_get failed getting correct element");

  return da;
}

struct dynamic_array *test_rm(struct dynamic_array *da, int flags) {
  if (!da)
    da = test_add(NULL, flags);

  struct user urm = {.age = 1, .name = "user3"};

  da_remove(da, &urm);
  assert(da->len == 2, "da_rm failed setting len");
  for (size_t i = 0; i < da->len; ++i) {
    struct user *u = (struct user *)da_get_idx(da, i);
    assert(strcmp(u->name, urm.name) != 0, "da_remove failed removing element");
  }

  if (flags & FLAG_SORTED_ARR) {
    for (size_t i = 0; i < da->len - 1; ++i) {
      assert(da->cmp_func(da->arr + i * da->elsize,
                          da->arr + (i + 1) * da->elsize) < 0,
             "da_add failed adding element in sorted order");
    }
  }

  return da;
}

struct dynamic_array *test_clear(struct dynamic_array *da) {
  if (!da)
    da = test_add(NULL, 0);

  da_clear(da);
  assert(da->len == 0, "da_clear failed setting len");
  return da;
}
