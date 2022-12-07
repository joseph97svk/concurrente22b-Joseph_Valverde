#include <assert.h>
#include <stdlib.h>

#include "array.h"
#include <stdio.h>

// static array_t array_elements = NULL;
// static size_t array_capacity = 0;
// static size_t array_count = 0;

array_t* array_create(size_t capacity)
{
  array_t* array = (array_t*) calloc(1, sizeof(array_t));
	assert(capacity);
	array->array_capacity = capacity;
	array->array_count = 0;
	array->array_elements = (void**)malloc( capacity * sizeof(void*) );
  pthread_mutex_init(&array->can_access_array, NULL);
  return array;
}

void array_destroy(array_t* array)
{
  pthread_mutex_destroy(&array->can_access_array);
  free(array->array_elements);
	free(array);
}

int array_increase_capacity(array_t* array)
{
	size_t new_capacity = 10 * array->array_capacity;
	array_data_t new_elements =
      (void**)realloc( array->array_elements, new_capacity * sizeof(void*) );
	if ( new_elements == NULL )
		return -1;
	array->array_capacity = new_capacity;
	array->array_elements = new_elements;

	return 0; // Success
}

int array_decrease_capacity(array_t* array)
{
	size_t new_capacity = array->array_capacity / 10;
	if ( new_capacity < 10 )
		return 0;

	array_data_t new_elements =
      (void**)realloc( array->array_elements, new_capacity * sizeof(void*) );
	if ( new_elements == NULL )
		return -1;

	array->array_capacity = new_capacity;
	array->array_elements = new_elements;

	return 0; // Success
}

size_t array_get_count(array_t* array)
{
  size_t count = 0;
  pthread_mutex_lock(&array->can_access_array);
    count = array->array_count;
  pthread_mutex_unlock(&array->can_access_array);
	return count;
}

void* array_get_element(array_t* array, size_t index)
{
	assert( index < array_get_count(array) );
  void* element = NULL;
  pthread_mutex_lock(&array->can_access_array); {
    element = array->array_elements[index];
  } pthread_mutex_unlock(&array->can_access_array);
	return element;
}

int array_append(array_t* array, void* element)
{
  pthread_mutex_lock(&array->can_access_array); {
    //printf("locked\n");
    if ( array->array_count == array->array_capacity )
      if ( ! array_increase_capacity(array) )
        return (void)pthread_mutex_unlock(&array->can_access_array), -1;

    array->array_elements[(array->array_count)++] = element;
  } pthread_mutex_unlock(&array->can_access_array);
  //printf("unlocked\n");
	return 0; // Success
}

size_t array_find_first(array_t* array,
    const void* element, size_t start_pos)
{
  pthread_mutex_lock(&array->can_access_array);
	for ( size_t index = start_pos; index < array->array_count; ++index )
		if ( array->array_elements[index] == element )
			return (void) pthread_mutex_unlock(&array->can_access_array), index;
  pthread_mutex_unlock(&array->can_access_array);
	return array_not_found;
}

int array_remove_first(array_t* array, const void* element, size_t start_pos)
{

  pthread_mutex_lock(&array->can_access_array); {

    pthread_mutex_unlock(&array->can_access_array);
      size_t index = array_find_first(array, element, start_pos);
    pthread_mutex_lock(&array->can_access_array);

    if ( index == array_not_found ) {
      return (void)pthread_mutex_unlock(&array->can_access_array), -1;
    }

    for ( --(array->array_count); index < array->array_count; ++index )
      array->array_elements[index] = array->array_elements[index + 1];
    if ( array->array_count == array->array_capacity / 10 )
      array_decrease_capacity(array);
  } pthread_mutex_unlock(&array->can_access_array);

	return 0; // Removed
}
