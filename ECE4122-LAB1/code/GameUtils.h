/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the header for the Grid struct and the other utility functions that didn't derserve their own files.
*/

#pragma once
#include "cstddef"
#include <cmath>
#include <SFML/Graphics.hpp>

// The grid struct maintains an integer grid within the play window
// This allows for transformation of positions in the integer grid to the float positions in game
struct Grid {
	Grid(size_t s, size_t width, size_t height);
	Grid() {}
	// Size in pixels of each grid square
	size_t size;

	// Grid is centered, so each position has an x and a y offset
    size_t xOffset;
    size_t yOffset;

	// Number of rows and cols in game grid
	size_t cols;
	size_t rows;

	sf::Vector2f grid2Coord(size_t r, size_t c);

	float col2Coord(size_t c);

	float row2Coord(size_t r);

	size_t x2Col (float x);

	size_t y2Row (float y);

	sf::Vector2f gridSnap(float x, float y);

	sf::Vector2f gridSnap(sf::Vector2f pos);
};

// Gets the l1 distance between two vectors
float distance(sf::Vector2f a, sf::Vector2f b);

// Gets magnitude of a vector
float magnitude(sf::Vector2f v);

// Function to normalize a vector
sf::Vector2f normalize(sf::Vector2f v);

// Coerces a position within an upper and lower boundary
sf::Vector2f coerceBounds(sf::Vector2f pos, sf::Vector2f lower, sf::Vector2f upper);