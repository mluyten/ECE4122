#pragma once
#include "cstddef"
#include <cmath>
#include <SFML/Graphics.hpp>

struct Grid {
	Grid(size_t s, size_t width, size_t height);
	Grid() {}
	size_t size;
    size_t xOffset;
    size_t yOffset;
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

float distance(sf::Vector2f a, sf::Vector2f b);

float magnitude(sf::Vector2f v);

// Function to normalize a vector
sf::Vector2f normalize(sf::Vector2f v);

sf::Vector2f coerceBounds(sf::Vector2f pos, sf::Vector2f lower, sf::Vector2f upper);