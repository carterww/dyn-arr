#ifndef DYN_ARR_H
#define DYN_ARR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define FLAG_SORTED_ARR (1)
#define FLAG_ADD_NO_CPY (1 << 1)

// For freeing items before removing
typedef void (*da_free_data)(void *data);

// If user wants sorted on something
typedef int (*da_data_cmp)(const void *d1, const void *d2);

struct dynamic_array {
  void *arr;
  size_t len;
  size_t cap;
  size_t elsize;
  da_free_data free_func;
  da_data_cmp cmp_func;
  uint8_t flags;
};

/** Creates a new dynamic array.
 *  param element_size: Sizeof each element to be inserted into the array.
 *  param initial_cap: Initial capacity of the array.
 *  param free_func: Function for freeing data elements. NULL if nothing to be
 *  freed.
 *  param cmp_func: Function for comparing elements for sorting. NULL if
 *  not sorted.
 */
struct dynamic_array *da_new(size_t element_size, size_t initial_cap,
                             da_free_data free_func, da_data_cmp cmp_func,
                             uint8_t flags);

void da_free(struct dynamic_array *da);
void da_clear(struct dynamic_array *da);
size_t da_len(struct dynamic_array *da);

const void *da_add(struct dynamic_array *da, const void *element);
const void *da_add_idx(struct dynamic_array *da, const void *element,
                       size_t idx);

const void *da_get(struct dynamic_array *da, const void *element);
const void *da_get_idx(struct dynamic_array *da, size_t idx);

int da_remove(struct dynamic_array *da, const void *element);
int da_remove_idx(struct dynamic_array *da, size_t idx);

int da_iter(struct dynamic_array *da, size_t *iter, void **element);

#ifdef __cplusplus
};
#endif

#endif // DYN_ARR_H
