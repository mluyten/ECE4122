/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This implements Grid and some utilities that did not warrant their own files.
*/

#include "GameUtils.h"

// Constructor
// s - size in pixels of each grid square
// width - width of the window in pixels
// height - height of the window in pixels;
Grid::Grid(size_t s, size_t width, size_t height)
{
	size = s;
	cols = width / size; // # of columns in the grid
	rows = height / size; // # of rows in the grid
	xOffset = (width - cols * size + size) / 2; // X offset required to center the grd
	yOffset = (height - rows * size + size) / 2; // Y offset required to center the grid
}

// Converts from grid coordinates to window coordinates
sf::Vector2f Grid::grid2Coord(size_t r, size_t c) 
{
	return sf::Vector2f(c * size + xOffset, r * size + yOffset);
}

// Converts column posiiton to x position
// c - column position
// returns - x position
float Grid::col2Coord(size_t c)
{
	return c * size + xOffset;
}

// Converts row posiiton to y position
// r - row position
// returns - y position
float Grid::row2Coord(size_t r) 
{
	return r * size + yOffset;
}

// Converts x position to nearest column
// x - x window position
// returns - nearest column
size_t Grid::x2Col (float x)
{
	return std::round((x - xOffset) / size);
}

// Converts y position to nearest row
// y - y window position
// returns - nearest row
size_t Grid::y2Row (float y)
{
	return std::round((y - yOffset) / size);
}

// Returns a position nearest to grid positon
// x, y - x and y window position
// returns - vector position of nearest grid point
sf::Vector2f Grid::gridSnap(float x, float y) 
{
	return sf::Vector2f(std::round((x - xOffset) / size) + xOffset, std::round((y - yOffset) / size) + yOffset);
}

// Snaps a position nearest to grid positon
// pos - vector window position
// returns - vector window position of nearest grid point
sf::Vector2f Grid::gridSnap(sf::Vector2f pos) 
{
	return sf::Vector2f(std::round((pos.x - xOffset) / size) * size + xOffset, std::round((pos.y - yOffset) / size) * size + yOffset);
}


// Gets L1 distance between two vectors
// a - position 1
// b - potition 2
// returns - L1 (city-block) distance between a and b
float distance(sf::Vector2f a, sf::Vector2f b) 
{
	return abs(b.x - a.x) + abs(b.y - a.y);
}

// Gets magnitured of a vector
// v - input vector
// returns - magnitude of v
float magnitude(sf::Vector2f v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// Function to normalize a vector
// v - input vector
// returns - L2 norm of v
sf::Vector2f normalize(sf::Vector2f v) 
{
	float mag = std::sqrt(v.x * v.x + v.y * v.y);
	if (mag == 0) return sf::Vector2f(0, 0);
	return sf::Vector2f(v.x / mag, v.y / mag);
}

// Coerces a position to lay within a set of upper and lower bounds
// pos - position to coerce
// lower - lower x and y bounds
// upper - upper x and y bounds
// returns - coerced position
sf::Vector2f coerceBounds(sf::Vector2f pos, sf::Vector2f lower, sf::Vector2f upper)
{
	// Check if out of bounds and coerce into position
	if (pos.x < lower.x)
		pos.x = lower.x;
	else if (pos.x > upper.x)
		pos.x = upper.x;

	if (pos.y < lower.y)
		pos.y = lower.y;
	else if (pos.y > upper.y)
		pos.y = upper.y;

	return pos;
}