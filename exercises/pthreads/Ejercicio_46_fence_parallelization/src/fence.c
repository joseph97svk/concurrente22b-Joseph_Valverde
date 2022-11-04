#include <assert.h>
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
} fence_data_t;

int read_terrain(fence_data_t* fence_data);
char** create_terrain(const size_t rows, const size_t columns);
void destroy_terrain(char** terrain, const size_t rows);
void find_maximum_perimeter(fence_data_t* fence_data);
coordinate_t find_maximum_local_perimeter(const size_t top_left_row, const size_t top_left_column, const fence_data_t* fence_data);
bool can_form_rectangle(const size_t top_left_row, const size_t top_left_column, const size_t bottom_right_row, const size_t bottom_right_column, const fence_data_t* fence_data);
size_t calculate_perimeter(const coordinate_t top_left, const coordinate_t bottom_right);
void print_maximum_perimeter(const fence_data_t* fence_data);

int main()
{
	fence_data_t* fence_data = (fence_data_t*) calloc(1, sizeof(fence_data_t));
	if ( fence_data == NULL )
		return (void)fprintf(stderr, "error: could not allocate shared memory\n"), 1;

	if ( read_terrain(fence_data) )
		return 2;

	find_maximum_perimeter(fence_data);

	// If maximum perimeter is 0
	if ( fence_data->maximum_perimeter == 0 )
	{
		// Print impossible
		puts("impossible");
	}
	// Else
	else
	{
		// Print maximum perimeter and its coordinates
		print_maximum_perimeter(fence_data);
	}

	free(fence_data);
	return 0;
}

int read_terrain(fence_data_t* fence_data)
{
	/* Input example:

	3 4
	.x..
	..x.
	x...
	*/

	// Read rows
	// Read columns
	if ( scanf("%zu %zu\n", &fence_data->rows, &fence_data->columns) != 2 )
		return 1;

	// Create terrain
	fence_data->terrain = create_terrain(fence_data->rows, fence_data->columns);
	if ( fence_data->terrain == NULL )
		return 2;

	// Read terrain
	for ( size_t row = 0; row < fence_data->rows; ++row )
	{
		for (size_t column = 0; column < fence_data->columns; ++column )
			fence_data->terrain[row][column] = getchar();

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

// Find the maximum perimeter:
void find_maximum_perimeter(fence_data_t* fence_data)
{
	// Create maximum perimeter as 0
	// Create maximum coordinates as (0,0)-(0,0)
	// Repeat top left row for each row of terrain except last one
	for ( size_t top_left_row = 0; top_left_row < fence_data->rows - 1; ++top_left_row )
	{
		// Repeat top left column for each column of terrain except last one
		for ( size_t top_left_column = 0; top_left_column < fence_data->columns - 1; ++top_left_column )
		{
			// Create local perimeter as the result of finding maximum perimeter that can be formed from row and column
			coordinate_t local_bottom_right = find_maximum_local_perimeter(top_left_row, top_left_column, fence_data);
			if ( local_bottom_right.row > 0 || local_bottom_right.column > 0 )
			{
				size_t local_perimeter = calculate_perimeter( (coordinate_t){top_left_row, top_left_column}, local_bottom_right );

				// If local perimeter is larger than the maximum perimeter
				if ( local_perimeter > fence_data->maximum_perimeter )
				{
					// Assign maximum perimeter to the local perimeter
					fence_data->maximum_perimeter = local_perimeter;
					// Assign maximum coordinates to the local rectangle's coordinates
					fence_data->maximum_top_left.row = top_left_row;
					fence_data->maximum_top_left.column = top_left_column;
					fence_data->maximum_bottom_right = local_bottom_right;
				}
			}
		}
	}
}

// Find maximum perimeter that can be formed from (top left row, top left column):
coordinate_t find_maximum_local_perimeter(const size_t top_left_row, const size_t top_left_column, const fence_data_t* fence_data)
{
	// Create local maximum perimeter as 0
	size_t local_maximum_perimeter = 0;
	// Create local maximum coordinates as (0,0)-(0,0)
	coordinate_t local_maximum_coordinates = {0, 0};

	// Repeat bottom right row from top left row + 1 to the last one
	for ( size_t bottom_right_row = top_left_row + 1; bottom_right_row < fence_data->rows; ++bottom_right_row )
	{
		// Repeat bottom right column from top left column + 1 to the last one
		for ( size_t bottom_right_column = top_left_column + 1; bottom_right_column < fence_data->columns; ++bottom_right_column )
		{
			// If can form rectangle from top left cell to bottom right cell
			if ( can_form_rectangle(top_left_row, top_left_column, bottom_right_row, bottom_right_column, fence_data) )
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
bool can_form_rectangle(const size_t top_left_row, const size_t top_left_column, const size_t bottom_right_row, const size_t bottom_right_column, const fence_data_t* fence_data)
{
	// Repeat column from top left column to bottom right column
	for ( size_t column = top_left_column; column <= bottom_right_column; ++column )
	{
		// If cell at top row is x or cell at bottom row is x then
		if ( fence_data->terrain[top_left_row][column] == 'x' || fence_data->terrain[bottom_right_row][column] == 'x' )
		{
			// Return false
			return false;
		}
	}
	// Repeat row from top left row + 1 to bottom right row - 1
	for ( size_t row = top_left_row + 1; row <= bottom_right_row - 1; ++row )
	{
		// If cell at left column is x or cell at right column is x then
		if ( fence_data->terrain[row][top_left_column] == 'x' ||  fence_data->terrain[row][bottom_right_column] == 'x' )
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

void print_maximum_perimeter(const fence_data_t* fence_data)
{
	printf("%zu (%zu,%zu)-(%zu,%zu)\n", fence_data->maximum_perimeter
		, fence_data->maximum_top_left.row + 1, fence_data->maximum_top_left.column + 1
		, fence_data->maximum_bottom_right.row + 1, fence_data->maximum_bottom_right.column + 1);
}
