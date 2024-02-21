#include "dyn-arr.h"
#include <stdlib.h>
#include <string.h>

static inline size_t da_byte_step(struct dynamic_array *da, size_t num) {
  return num * da->elsize;
}

static int da_grow(struct dynamic_array *da, size_t min_index);
static const void *da_add_sorted(struct dynamic_array *da, const void *element);
static const void *da_add_unsorted(struct dynamic_array *da,
                                   const void *element);
static size_t da_bin_search(struct dynamic_array *da, const void *element,
                            int *found);

struct dynamic_array *da_new(size_t element_size, size_t initial_cap,
                             da_free_data free_func, da_data_cmp cmp_func,
                             uint8_t flags) {
  struct dynamic_array *da = calloc(1, sizeof(struct dynamic_array));
  if (da == NULL)
    return NULL;
  da->arr = calloc(initial_cap, element_size);
  if (da->arr == NULL) {
    free(da);
    return NULL;
  }

  da->elsize = element_size;
  da->cap = initial_cap;
  da->free_func = free_func;
  da->cmp_func = cmp_func;
  da->flags = flags;

  return da;
}

void da_free(struct dynamic_array *da) {
  if (da == NULL)
      return;
  if (da->free_func) {
    for (size_t i = 0; i < da->len * da->elsize; i += da->elsize) {
      da->free_func(da->arr + i);
    }
  }
  free(da->arr);
  free(da);
}

void da_clear(struct dynamic_array *da) {
  if (da == NULL)
      return;
  memset(da->arr, 0, da->elsize * da->cap);
  da->len = 0;
}

size_t da_len(struct dynamic_array *da) {
    return da ? da->len : 0;
}

const void *da_add(struct dynamic_array *da, const void *element) {
  if (da == NULL)
      return NULL;

  return da->flags & FLAG_SORTED_ARR ? da_add_sorted(da, element)
                                     : da_add_unsorted(da, element);
}

const void *da_add_idx(struct dynamic_array *da, const void *element,
                       size_t idx) {
  if (da == NULL)
      return NULL;

  if (idx >= da->cap || da->len >= da->cap)
    da_grow(da, idx);

  for (size_t i = da->len; i > idx; --i) {
    size_t step = da_byte_step(da, i);
    memcpy(da->arr + step, da->arr + step - da->elsize, da->elsize);
  }
  memcpy(da->arr + (idx * da->elsize), element, da->elsize);
  ++da->len;
  return (da->arr + (idx * da->elsize));
}

const void *da_get(struct dynamic_array *da, const void *element) {
  if (da == NULL)
      return NULL;

  if (da->flags & FLAG_SORTED_ARR) {
    int found;
    size_t idx = da_bin_search(da, element, &found);
    if (found)
      return da->arr + (idx * da->elsize);
    return NULL;
  }

  for (size_t i = 0; i < da->len * da->elsize; i += da->elsize) {
    size_t cmp_res = da->cmp_func(element, da->arr + i);
    if (cmp_res == 0)
        return da->arr + i;
  }
  return NULL;
}

const void *da_get_idx(struct dynamic_array *da, size_t idx) {
  if (da == NULL)
      return NULL;
  return da->arr + (idx * da->elsize);
}

int da_remove(struct dynamic_array *da, const void *element) {
    if (da == NULL)
        return -2;
    void *ptr = (void *)da_get(da, element);
    if (ptr == NULL)
        return -1;
    
    return da_remove_idx(da, (ptr - da->arr) / da->elsize);
}

int da_remove_idx(struct dynamic_array *da, size_t idx) {
    if (da == NULL)
        return -2;
    if (idx >= da->len)
        return -1;
    if (da->free_func)
        da->free_func(da->arr + (idx * da->elsize));

    size_t i = idx * da->elsize;
    size_t end = (da->len - 1) * da->elsize;
    for (; i < end; i += da->elsize) {
        memcpy(da->arr + i, da->arr + i + da->elsize, da->elsize);
    }
    --da->len;
    return 0;
}

int da_iter(struct dynamic_array *da, size_t *iter, void **element) {
  if (da == NULL)
      return -2;
  if (*iter >= da->len)
    return 0;
  *element = da->arr + (*iter * da->elsize);
  ++(*iter);
  return 1;
}

static int da_grow(struct dynamic_array *da, size_t min_index) {
  size_t new_cap = (size_t)(2 * da->cap);
  if (min_index >= new_cap) {
    new_cap = min_index + 1;
  }

  da->arr = realloc(da->arr, new_cap * da->elsize);
  if (da->arr == NULL)
    return -1;
  da->cap = new_cap;
  return 0;
}

static const void *da_add_sorted(struct dynamic_array *da,
                                 const void *element) {
    size_t idx = da_bin_search(da, element, NULL);
    return da_add_idx(da, element, idx);
}

static const void *da_add_unsorted(struct dynamic_array *da,
                                   const void *element) {
    return da_add_idx(da, element, da->len);
}

static size_t da_bin_search(struct dynamic_array *da, const void *element,
                            int *found) {

  size_t l = 0, r = da->len - 1;
  size_t mid;
  if (found)
    *found = 0;
  if (da->len == 0)
    return 0;

  while (l <= r) {
    mid = l + ((r - l) / 2);
    int cmp_res = da->cmp_func(element, da->arr + (mid * da->elsize));
    // If mid == 0, wraps to max size_t
    if (cmp_res < 0 && mid != 0)
      r = mid - 1;
    else if (cmp_res > 0)
      l = mid + 1;
    else if (mid == 0)
      break;
    else {
      if (found) *found = 1;
      return mid;
    }
  }
  return l;
}
