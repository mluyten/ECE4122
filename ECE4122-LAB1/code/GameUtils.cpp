#include "GameUtils.h"

Grid::Grid(size_t s, size_t width, size_t height)
{
	size = s;
	cols = width / size;
	rows = height / size;
	xOffset = (width - cols * size + size) / 2;
	yOffset = (height - rows * size + size) / 2;
}

sf::Vector2f Grid::grid2Coord(size_t r, size_t c) 
{
	return sf::Vector2f(c * size + xOffset, r * size + yOffset);
}

float Grid::col2Coord(size_t c)
{
	return c * size + xOffset;
}

float Grid::row2Coord(size_t r) 
{
	return r * size + yOffset;
}

size_t Grid::x2Col (float x)
{
	return std::round((x - xOffset) / size);
}

size_t Grid::y2Row (float y)
{
	return std::round((y - yOffset) / size);
}

sf::Vector2f Grid::gridSnap(float x, float y) 
{
	return sf::Vector2f(std::round((x - xOffset) / size) + xOffset, std::round((y - yOffset) / size) + yOffset);
}

sf::Vector2f Grid::gridSnap(sf::Vector2f pos) 
{
	return sf::Vector2f(std::round((pos.x - xOffset) / size) * size + xOffset, std::round((pos.y - yOffset) / size) * size + yOffset);
}

float distance(sf::Vector2f a, sf::Vector2f b) 
{
	return abs(b.x - a.x) + abs(b.y - a.y);
}

float magnitude(sf::Vector2f v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// Function to normalize a vector
sf::Vector2f normalize(sf::Vector2f v) 
{
	float mag = std::sqrt(v.x * v.x + v.y * v.y);
	if (mag == 0) return sf::Vector2f(0, 0);
	return sf::Vector2f(v.x / mag, v.y / mag);
}

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