#pragma once
#include "cstddef"
#include <cmath>
#include <SFML/Graphics.hpp>

struct Grid {
	Grid(size_t s, size_t width, size_t height) : size(s) 
	{
        cols = width / size;
	    rows = height / size;
        xOffset = (width - cols * size + size) / 2;
		yOffset = (height - rows * size + size) / 2;
    }
	Grid() {}
	size_t size;
    size_t xOffset;
    size_t yOffset;
	size_t cols;
	size_t rows;

	sf::Vector2f grid2Coord(size_t r, size_t c) 
	{
		return sf::Vector2f(c * size + xOffset, r * size + yOffset);
	}

	float col2Coord(size_t c)
	{
		return c * size + xOffset;
	}

	float row2Coord(size_t r) 
	{
		return r * size + yOffset;
	}

	sf::Vector2f gridSnap(float x, float y) 
	{
		return sf::Vector2f(std::round((x - xOffset) / size) + xOffset, std::round((y - yOffset) / size) + yOffset);
	}

	sf::Vector2f gridSnap(sf::Vector2f pos) 
	{
		return sf::Vector2f(std::round((pos.x - xOffset) / size) * size + xOffset, std::round((pos.y - yOffset) / size) * size + yOffset);
	}
};