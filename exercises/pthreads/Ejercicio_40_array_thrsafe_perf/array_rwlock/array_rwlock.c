#include <assert.h>
#include <stdlib.h>

#include "array_rwlock.h"
#include <stdio.h>

// static array_rw_t array_elements = NULL;
// static size_t array_capacity = 0;
// static size_t array_count = 0;

size_t find_first_aux(array_rw_t* array,
    const void* element, size_t start_pos);

array_rw_t* array_rwlock_create(size_t capacity) {
  assert(capacity);
  array_rw_t* array = calloc(1, sizeof(array_rw_t));
  if (array == NULL) return NULL;
  array->array_capacity = capacity;
  array->array_count = 0;

  pthread_rwlock_init(&array->can_access_array, NULL);

  array->array_elements = (void**)malloc(capacity * sizeof(void*));
  if (array->array_elements == NULL)
    return (void)free(array), NULL;  
  return array;
}

void array_rwlock_destroy(array_rw_t* array) {
  pthread_rwlock_destroy(&array->can_access_array);

  free(array->array_elements);
  free(array);
}

int array_rwlock_increase_capacity(array_rw_t* array) {
  size_t new_capacity = 10 * array->array_capacity;
  void** new_elements =
      (void**)realloc(array->array_elements, new_capacity*sizeof(void*));
  if (new_elements == NULL)
    return -1;

  array->array_capacity = new_capacity;
  array->array_elements = new_elements;

  return 0; // Success
}

int array_rwlock_decrease_capacity(array_rw_t* array) {
  size_t new_capacity = array->array_capacity / 10;
  if (new_capacity < 10)
    return 0;

  void** new_elements =
      (void**)realloc(array->array_elements, new_capacity * sizeof(void*));
  if (new_elements == NULL)
    return -1;

  array->array_capacity = new_capacity;
  array->array_elements = new_elements;

  return 0; // Success
}

size_t array_rwlock_get_count(array_rw_t* array) {
  pthread_rwlock_rdlock(&array->can_access_array);
    size_t result = array->array_count;
  pthread_rwlock_unlock(&array->can_access_array);
  return result;
}

void* array_rwlock_get_element(array_rw_t* array, size_t index) {
  assert(index < array_rwlock_get_count(array));
  pthread_rwlock_rdlock(&array->can_access_array);
    void* result = array->array_elements[index];
  pthread_rwlock_unlock(&array->can_access_array);
  return result;
}

int array_rwlock_append(array_rw_t* array, void* element) {
  pthread_rwlock_wrlock(&array->can_access_array);
    if (array->array_count == array->array_capacity)
      if (array_rwlock_increase_capacity(array))
        return (void)pthread_rwlock_unlock(&array->can_access_array), -1;

    array->array_elements[array->array_count++] = element;
  pthread_rwlock_unlock(&array->can_access_array);
  return 0; // Success
}

size_t array_rwlock_find_first(array_rw_t* array,
    const void* element, size_t start_pos) {
  pthread_rwlock_rdlock(&array->can_access_array);
    size_t result = find_first_aux(array, element, start_pos);
  pthread_rwlock_unlock(&array->can_access_array);
  return result;
}

size_t find_first_aux(array_rw_t* array,
    const void* element, size_t start_pos) {
  for (size_t index = start_pos; index < array->array_count; ++index)
    if (array->array_elements[index] == element)
      return index;

  return array_rwlock_not_found;
}

int array_rwlock_remove_first(array_rw_t* array, const void* element, size_t start_pos) {
  pthread_rwlock_wrlock(&array->can_access_array);
    size_t index = find_first_aux(array, element, start_pos);
    if (index == array_rwlock_not_found)
      return (void)pthread_rwlock_unlock(&array->can_access_array), -1;

    for (--array->array_count; index < array->array_count; ++index)
      array->array_elements[index] = array->array_elements[index + 1];

    if (array->array_count == array->array_capacity / 10)
      array_rwlock_decrease_capacity(array);
  pthread_rwlock_unlock(&array->can_access_array);
  return 0; // Removed
}
