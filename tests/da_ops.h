#ifndef TEST_DA_OPS_H
#define TEST_DA_OPS_H

#include "../dyn-arr.h"

struct dynamic_array *test_new(int flags);
struct dynamic_array *test_add(struct dynamic_array *da, int flags);
struct dynamic_array *test_get(struct dynamic_array *da, int flags);
struct dynamic_array *test_rm(struct dynamic_array *da, int flags);
struct dynamic_array *test_free(struct dynamic_array *da);
struct dynamic_array *test_clear(struct dynamic_array *da);

#endif // TEST_DA_OPS_H
