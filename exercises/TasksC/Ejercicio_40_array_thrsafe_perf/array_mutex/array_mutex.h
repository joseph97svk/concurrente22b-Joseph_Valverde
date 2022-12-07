#ifndef ARRAYMUTEX_H
#define ARRAYMUTEX_H

#include <stddef.h>
#include <pthread.h>

typedef void** array_data_t;

typedef struct array_mutex {
  array_data_t array_elements;
  size_t array_capacity;
  size_t array_count;
  pthread_mutex_t can_access_array;
} array_t;

typedef array_t* array_mutex_t;

static const size_t array_mutex_not_found = (size_t)-1;

array_t* array_mutex_create(size_t capacity);
void array_mutex_destroy(array_t* array);
int array_mutex_append(array_t* array, void* element);
size_t array_mutex_get_count(array_t* array);
void* array_mutex_get_element(array_t* array, size_t index);
size_t array_mutex_find_first(array_t* array, const void* element, size_t start_pos);
int array_mutex_remove_first(array_t* array, const void* element, size_t start_pos);

#endif // ARRAY_H
