#ifndef ARRAYRWLOCK_H
#define ARRAYRWLOCK_H

#include <stddef.h>
#include <pthread.h>

typedef void** array_data_t;

typedef struct array_rw {
  array_data_t array_elements;
  size_t array_capacity;
  size_t array_count;
  pthread_rwlock_t can_access_array;
} array_rw_t;

typedef array_rw_t* array_rwlock_t;

static const size_t array_rwlock_not_found = (size_t)-1;

array_rw_t* array_rwlock_create(size_t capacity);
void array_rwlock_destroy(array_rw_t* array);
int array_rwlock_append(array_rw_t* array, void* element);
size_t array_rwlock_get_count(array_rw_t* array);
void* array_rwlock_get_element(array_rw_t* array, size_t index);
size_t array_rwlock_find_first(array_rw_t* array, const void* element, size_t start_pos);
int array_rwlock_remove_first(array_rw_t* array, const void* element, size_t start_pos);

#endif // ARRAY_H
