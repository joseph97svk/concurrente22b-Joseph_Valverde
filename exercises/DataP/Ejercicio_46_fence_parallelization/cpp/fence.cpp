#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

// Data type declarations
typedef short coord_t;
typedef long perimeter_t;
static const perimeter_t min_perimeter = std::numeric_limits<perimeter_t>::min();

struct cell_t
{
	// Distance from the cell to the closest right marsh
	coord_t x_distance;
	// Distance from the cell to the closest bottom marsh
	coord_t y_distance;
};

// Calculates the potential perimeter that may form a fence from this cell
inline perimeter_t calculate_perimeter(coord_t width, coord_t height) { return 2 * (width + height - 2); }
// Calculates the potential perimeter that may form a fence from this cell
inline perimeter_t calculate_perimeter(const cell_t& cell) { return calculate_perimeter(cell.x_distance, cell.y_distance); }


typedef std::vector<std::vector<cell_t>> terrain_t;
typedef std::multimap<perimeter_t, std::pair<size_t, size_t>, std::greater<perimeter_t>> cell_perimeters_t;

// Function declarations
bool read_terrain(terrain_t& terrain);
void update_distances(terrain_t& terrain, cell_perimeters_t& cell_perimeters);
void update_row_distances(terrain_t& terrain);
void update_column_distances(terrain_t& terrain, cell_perimeters_t& cell_perimeters);
void find_largest_fence(terrain_t& terrain, cell_perimeters_t& cell_perimeters, perimeter_t& largest_fence);
void find_largest_fence_for_cell(size_t row, size_t column, terrain_t& terrain, perimeter_t& largest_fence);
void print_largest_fence(const perimeter_t& largest_fence);

inline void ignore_new_line(std::istream& in) { in.ignore( std::numeric_limits<std::streamsize>::max(), '\n' ); }

// Main:
int main()
{
	// Read terrain dimensions
	size_t rows = 0, columns = 0;
	std::cin >> rows >> columns;
	ignore_new_line(std::cin);

	// Valid dimensions
	if ( rows <= 0 || columns <= 0 )
		return 1;

	// Create terrain as a matrix of records cell
	terrain_t terrain(rows, std::vector<cell_t>(columns));
	// Read terrain
	if ( read_terrain(terrain) )
	{
		// Create a map that associates perimeters (integers) with cells (row,col) in descending order
		// so cells that can create larger fences are processed before
		cell_perimeters_t cell_perimeters;
		// Update distances in terrain
		update_distances(terrain, cell_perimeters);

		// Create a Cell for the perimeter of the largest fence as -infinite
		perimeter_t largest_perimeter = min_perimeter;
		// Find the largest fence
		find_largest_fence(terrain, cell_perimeters, largest_perimeter);
		// Print perimeter of the largest fence
		print_largest_fence(largest_perimeter);
	}

	// Done
	return 0;
}

// Read terrain:
bool read_terrain(terrain_t& terrain)
{
	// Repeat for each row
	for ( size_t row = 0; row < terrain.size(); ++row )
	{
		// Repeat for each column
		for ( size_t column = 0; column < terrain[row].size(); ++column )
		{
			// Read token
			int token = std::cin.get();
			// If token is a dot
			if ( token == '.' )
			{
				// Set the cell as not updated (-1 values)
				terrain[row][column] = {-1, -1};
			}
			// Else if token is a X
			else if ( token == 'x' )
			{
				// Set the cell as a marsh (0 values)
				terrain[row][column] = {0, 0};
			}
			else
			{
				// Data source is corrupt
				return false;
			}
		}

		// Ignore the new-line char after the row
		ignore_new_line(std::cin);
	}

	return true;
}

// Update distances:
void update_distances(terrain_t& terrain, cell_perimeters_t& cell_perimeters)
{
	// Update row distances
	update_row_distances(terrain);
	// Update column distances
	update_column_distances(terrain, cell_perimeters);
}

// Update row distances:
void update_row_distances(terrain_t& terrain)
{
	// Repeat for each row
	for ( size_t row = 0; row < terrain.size(); ++row )
	{
		// Create distance as 0
		coord_t distance = 0;
		// Repeat for each column in descending order
		for ( size_t column = terrain[row].size(); column-- > 0; )
		{
			// If cell has a marsh (0 values)
			if ( terrain[row][column].x_distance == 0 )
			{
				// Set distance to 0
				distance = 0;
			}
			else
			{
				// Increase distance
				// Set cell's x distance to distance
				terrain[row][column].x_distance = ++distance;
			}
		}
	}
}

// Update column distances:
void update_column_distances(terrain_t& terrain, cell_perimeters_t& cell_perimeters)
{
	// Repeat for each column
	for ( size_t column = 0; column < terrain[0].size(); ++column )
	{
		// Create distance as 0
		coord_t distance = 0;
		// Repeat for each row in descending order
		for ( size_t row = terrain.size(); row-- > 0; )
		{
			// If cell has a marsh (0 values)
			if ( terrain[row][column].y_distance == 0 )
			{
				// Set distance to 0
				distance = 0;
			}
			else
			{
				// Increase distance
				// Set cell's y distance to distance
				terrain[row][column].y_distance = ++distance;
				assert(terrain[row][column].x_distance >= 0);
				assert(terrain[row][column].y_distance >= 0);

				// Do not insert into the tree, cells that are unable to form a fence
				if ( terrain[row][column].x_distance > 1 && terrain[row][column].y_distance > 1 )
				{
					// Calculate the maximum perimeter for the cell (x distance by y distance)
					perimeter_t perimeter = calculate_perimeter(terrain[row][column]);
					// Append the cell to the queue of that perimeter in the map of cell perimeters
					cell_perimeters.insert( std::make_pair(perimeter, std::make_pair(row, column)) );
				}
			}
		}
	}
}

// Find the largest fence:
void find_largest_fence(terrain_t& terrain, cell_perimeters_t& cell_perimeters, perimeter_t& largest_fence)
{
	// Repeat for each perimeter in the map of cell perimeters (starting with the largest one)
	for ( cell_perimeters_t::const_iterator itr = cell_perimeters.begin(); itr != cell_perimeters.end(); ++itr )
	{
		// Get the pointed cell
		const size_t row = itr->second.first;
		const size_t column = itr->second.second;
		const cell_t& cell = terrain[row][column];
		// If the potential cell fence (x distance by y distance) is greater than max fence
		if ( calculate_perimeter(cell) > largest_fence )
		{
			// Find the largest fence for the cell
			find_largest_fence_for_cell(row, column, terrain, largest_fence);
		}
		else
		{
			// This and remaining cells cannot create a largest fence than the already known
			// Stop the search
			return ;
		}
	}
}

// Find the largest fence for cell:
void find_largest_fence_for_cell(size_t row, size_t column, terrain_t& terrain, perimeter_t& largest_fence)
{
	// Get the cel at row/column
	const cell_t& cell = terrain[row][column];

	// Repeat bottom-left-row starting at cell's y-distance to 1
	for ( size_t bottom_left_row = row + static_cast<size_t>(cell.y_distance) - 1; bottom_left_row > row; --bottom_left_row )
	{
		// Try to form a fence with cell and bottom-left-cell
		const cell_t& bottom_left_cell = terrain[bottom_left_row][column];
		// If bottom-left-cell's x-distance is 1
		if ( bottom_left_cell.x_distance <= 1 )
		{
			// We cannot build a fence with perimeter 0. Continue with the cell at North of bottom-left-cell
			continue;
		}

		// Calculate the max potetnial fence that might be created between cell and bottom_left_cell
		const coord_t potential_width = std::min(cell.x_distance, bottom_left_cell.x_distance);
		const coord_t height = static_cast<coord_t>(bottom_left_row - row + 1);
		// If potential bottom-left-cell's fence is less than max fence that we already know
		if ( calculate_perimeter(potential_width, height) <= largest_fence )
		{
			// We cannot improve our current max perimeter. Continue with the cell at North
			continue;
		}

		// Find a third cell (top-right or bottom-right) to form a fence larger than our max
		// Repeat top-right-col starting at minimum of cell's x-distance and bottom-left-cell's x distance
		for ( size_t top_right_col = column + static_cast<size_t>(potential_width) - 1; top_right_col > column; --top_right_col )
		{
			// Try to form a fence with cell, bottom_left_cell, and top_right_cell
			const cell_t& top_right_cell = terrain[row][top_right_col];
			// If top-right-col-cell's y-distance is greater or equals than potential fence's height
			if ( top_right_cell.y_distance >= height )
			{
				// We found a new valid fence. Calculate its perimeter
				const coord_t width = static_cast<coord_t>(top_right_col - column + 1);
				const perimeter_t new_perimeter = calculate_perimeter(width, height);
				//  If new fence's perimeter is greather than our max fence
				if ( new_perimeter > largest_fence )
				{
					// Set max fence to the perimeter found (may store coordinates also)
					largest_fence = new_perimeter;
				  #ifdef VERBOSE
					std::cerr << largest_fence << " (" << row + 1 << "," << column + 1 << ")-(" << bottom_left_row + 1
						 << "," << top_right_col + 1 << ") [" << width << "," << height << "]\n";
				  #endif
				}
			}
		}
	}
}

// Print perimeter of the largest fence:
void print_largest_fence(const perimeter_t& largest_fence)
{
	// If perimeter is -infinite
	if ( largest_fence == min_perimeter )
	{
		// Print impossible
		std::cout << "impossible" << std::endl;
	}
	else
	{
		// Print perimeter
		std::cout << largest_fence << std::endl;
	}
}
