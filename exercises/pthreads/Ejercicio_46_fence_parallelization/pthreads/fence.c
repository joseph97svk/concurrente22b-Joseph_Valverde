#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
	size_t row;
	size_t column;
} coordinate_t;

typedef struct
{
	size_t thread_count;
	size_t rows;
	size_t columns;
	char** terrain;
	size_t maximum_perimeter;
	coordinate_t maximum_top_left;
	coordinate_t maximum_bottom_right;
	pthread_mutex_t mutex;
} shared_data_t;

typedef struct
{
	size_t thread_num;
	shared_data_t* shared_data;
} private_data_t;

int create_threads(shared_data_t* shared_data);
void* run(void* data);

int read_terrain(shared_data_t* shared_data);
char** create_terrain(const size_t rows, const size_t columns);
void destroy_terrain(char** terrain, const size_t rows);
int create_threads(shared_data_t* shared_data);
void find_maximum_perimeter(const size_t thread_num, shared_data_t* shared_data);
coordinate_t find_maximum_local_perimeter(const size_t top_left_row, const size_t top_left_column, const shared_data_t* shared_data);
bool can_form_rectangle(const size_t top_left_row, const size_t top_left_column, const size_t bottom_right_row, const size_t bottom_right_column, const shared_data_t* shared_data);
size_t calculate_perimeter(const coordinate_t top_left, const coordinate_t bottom_right);
void print_maximum_perimeter(const shared_data_t* shared_data);

int main(int argc, char* argv[])
{
	shared_data_t* shared_data = (shared_data_t*) calloc(1, sizeof(shared_data_t));
	if ( shared_data == NULL )
		return (void)fprintf(stderr, "error: could not allocate shared memory\n"), 1;

	pthread_mutex_init( &shared_data->mutex, NULL );

	shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
	if ( argc >= 2 )
		shared_data->thread_count = strtoull(argv[1], NULL, 10);

	if ( read_terrain(shared_data) )
		return 2;

	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	// Find the maximum perimeter
	int error = create_threads(shared_data);
	if ( error )
		return error;

	struct timespec finish_time;
	clock_gettime(CLOCK_MONOTONIC, &finish_time);

	double elapsed_seconds = finish_time.tv_sec - start_time.tv_sec
		+ 1e-9 * (finish_time.tv_nsec - start_time.tv_nsec);

	// If maximum perimeter is 0
	if ( shared_data->maximum_perimeter == 0 )
	{
		// Print impossible
		puts("impossible");
	}
	// Else
	else
	{
		// Print maximum perimeter and its coordinates
		print_maximum_perimeter(shared_data);
	}

	fprintf(stderr, "Hello execution time %.9lfs\n", elapsed_seconds);

	pthread_mutex_destroy( &shared_data->mutex );
	free(shared_data);
	return 0;
}

int read_terrain(shared_data_t* shared_data)
{
	/* Input example:

	3 4
	.x..
	..x.
	x...
	*/

	// Read rows
	// Read columns
	if ( scanf("%zu %zu\n", &shared_data->rows, &shared_data->columns) != 2 )
		return 1;

	// Create terrain
	shared_data->terrain = create_terrain(shared_data->rows, shared_data->columns);
	if ( shared_data->terrain == NULL )
		return 2;

	// Read terrain
	for ( size_t row = 0; row < shared_data->rows; ++row )
	{
		for (size_t column = 0; column < shared_data->columns; ++column )
			shared_data->terrain[row][column] = getchar();

		// Skip the new-line char
		getchar();
	}

	return 0;
}

char** create_terrain(const size_t rows, const size_t columns)
{
	char** terrain = calloc(rows, sizeof(char*));
	if ( terrain == NULL )
		return NULL;

	for ( size_t row = 0; row < rows; ++row )
		if ( (terrain[row] = calloc(columns, sizeof(char))) == NULL )
			return destroy_terrain(terrain, rows), NULL;

	return terrain;
}

void destroy_terrain(char** terrain, const size_t rows)
{
	assert(terrain);
	for ( size_t row = 0; row < rows; ++row )
		free( terrain[row] );
	free(terrain);
}


int create_threads(shared_data_t* shared_data)
{
	pthread_t* threads = (pthread_t*) malloc(shared_data->thread_count * sizeof(pthread_t));
	if ( threads == NULL )
		return (void)fprintf(stderr, "error: could not allocate memory for %zu threads\n", shared_data->thread_count), 2;

	private_data_t* private_data = (private_data_t*) calloc(shared_data->thread_count, sizeof(private_data_t));
	if ( private_data == NULL )
		return (void)fprintf(stderr, "error: could not allocate private memory for %zu threads\n", shared_data->thread_count), 3;

	for ( size_t index = 0; index < shared_data->thread_count; ++index )
	{
		private_data[index].thread_num = index;
		private_data[index].shared_data = shared_data;
		pthread_create(&threads[index], NULL, run, &private_data[index]);
	}

	for ( size_t index = 0; index < shared_data->thread_count; ++index )
		pthread_join(threads[index], NULL);

	free(private_data);
	free(threads);
	return 0;
}

// Find the maximum perimeter
void* run(void* data)
{
	private_data_t* private_data = (private_data_t*)data;
	shared_data_t* shared_data = private_data->shared_data;

	find_maximum_perimeter(private_data->thread_num, shared_data);

	return NULL;
}

// Find the maximum perimeter:
void find_maximum_perimeter(const size_t thread_num, shared_data_t* shared_data)
{
	// Create maximum perimeter as 0
	// Create maximum coordinates as (0,0)-(0,0)
	// Repeat concurrently|in parallel (ciclically) top left row for each row of terrain except last one
	for ( size_t top_left_row = thread_num; top_left_row < shared_data->rows - 1; top_left_row += shared_data->thread_count )
	{
		// Repeat top left column for each column of terrain except last one
		for ( size_t top_left_column = 0; top_left_column < shared_data->columns - 1; ++top_left_column )
		{
			// Create local perimeter as the result of finding maximum perimeter that can be formed from row and column
			coordinate_t local_bottom_right = find_maximum_local_perimeter(top_left_row, top_left_column, shared_data);
			if ( local_bottom_right.row > 0 || local_bottom_right.column > 0 )
			{
				size_t local_perimeter = calculate_perimeter( (coordinate_t){top_left_row, top_left_column}, local_bottom_right );

				// Critic region:
				pthread_mutex_lock( &shared_data->mutex );
				// If local perimeter is larger than the maximum perimeter
				if ( local_perimeter > shared_data->maximum_perimeter )
				{
					// Assign maximum perimeter to the local perimeter
					shared_data->maximum_perimeter = local_perimeter;
					// Assign maximum coordinates to the local rectangle's coordinates
					shared_data->maximum_top_left.row = top_left_row;
					shared_data->maximum_top_left.column = top_left_column;
					shared_data->maximum_bottom_right = local_bottom_right;
				}
				pthread_mutex_unlock( &shared_data->mutex );
			}
		}
	}
}

// Find maximum perimeter that can be formed from (top left row, top left column):
coordinate_t find_maximum_local_perimeter(const size_t top_left_row, const size_t top_left_column, const shared_data_t* shared_data)
{
	// Create local maximum perimeter as 0
	size_t local_maximum_perimeter = 0;
	// Create local maximum coordinates as (0,0)-(0,0)
	coordinate_t local_maximum_coordinates = {0, 0};

	// Repeat bottom right row from top left row + 1 to the last one
	for ( size_t bottom_right_row = top_left_row + 1; bottom_right_row < shared_data->rows; ++bottom_right_row )
	{
		// Repeat bottom right column from top left column + 1 to the last one
		for ( size_t bottom_right_column = top_left_column + 1; bottom_right_column < shared_data->columns; ++bottom_right_column )
		{
			// If can form rectangle from top left cell to bottom right cell
			if ( can_form_rectangle(top_left_row, top_left_column, bottom_right_row, bottom_right_column, shared_data) )
			{
				// Create local perimeter as result of calculate perimeter
				size_t perimeter = calculate_perimeter((coordinate_t){top_left_row, top_left_column}, (coordinate_t){bottom_right_row, bottom_right_column});
				// If local perimeter is larger than the maximum perimeter
				if ( perimeter > local_maximum_perimeter )
				{
					// Assign maximum perimeter to the local perimeter
					local_maximum_perimeter = perimeter;
					// Assign maximum coordinates to the local rectangle's coordinates
					local_maximum_coordinates = (coordinate_t){bottom_right_row, bottom_right_column};
				}
			}
		}
	}

	// Return the maximum perimeter as a record of perimeter and coordinates
	return local_maximum_coordinates;
}

// Can form rectangle from top left cell to bottom right cell:
bool can_form_rectangle(const size_t top_left_row, const size_t top_left_column, const size_t bottom_right_row, const size_t bottom_right_column, const shared_data_t* shared_data)
{
	// Repeat column from top left column to bottom right column
	for ( size_t column = top_left_column; column <= bottom_right_column; ++column )
	{
		// If cell at top row is x or cell at bottom row is x then
		if ( shared_data->terrain[top_left_row][column] == 'x' || shared_data->terrain[bottom_right_row][column] == 'x' )
		{
			// Return false
			return false;
		}
	}
	// Repeat row from top left row + 1 to bottom right row - 1
	for ( size_t row = top_left_row + 1; row <= bottom_right_row - 1; ++row )
	{
		// If cell at left column is x or cell at right column is x then
		if ( shared_data->terrain[row][top_left_column] == 'x' ||  shared_data->terrain[row][bottom_right_column] == 'x' )
		{
			// Return false
			return false;
		}
	}
	// Return true
	return true;
}

// Calculate perimeter:
size_t calculate_perimeter(const coordinate_t top_left, const coordinate_t bottom_right)
{
	// Return 2 * (bottom right column - top left column) + 2 * (bottom right row - top left row)
	return 2 * (bottom_right.column - top_left.column) + 2 * (bottom_right.row - top_left.row);
}

void print_maximum_perimeter(const shared_data_t* shared_data)
{
	printf("%zu (%zu,%zu)-(%zu,%zu)\n", shared_data->maximum_perimeter
		, shared_data->maximum_top_left.row + 1, shared_data->maximum_top_left.column + 1
		, shared_data->maximum_bottom_right.row + 1, shared_data->maximum_bottom_right.column + 1);
}
