/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This file implements the Mushroom class and its related generate function.
*/

#include "Mushroom.h"

// Constructor
// (startX, startY) - Starting position for mushroom
// textureFull - Initial texture to load into sprite
// textureDamaged - Texture to be set when mushroom is damaged
Mushroom::Mushroom(float startX, float startY, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	// Set origin to center of sprite
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	setPosition(startX, startY);
	// Set damaged to false and keep track of damaged texture
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

// Constructor
// start - Starting position vector for mushroom
// textureFull - Initial texture to load into sprite
// textureDamaged - Texture to be set when mushroom is damaged
Mushroom::Mushroom(sf::Vector2f start, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	// Sets origin to center of sprite
	setPosition(start);
	// Set damaged to false and keep track of damaged texture
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

// "Damages" a mushroom
void Mushroom::damage()
{
	// Set damaged texture and flag - if hit again, it will be destroyed
	setTexture(*textureDamaged_);
	damaged = true;
	return;
}

// Generates n mushrooms on a given grid
// n - number of mushrooms to create
// mushrooms - list of mushrooms to add mushrooms to
// grid - grid object that defines the grid locations where mushrooms must be placed
// texture - full texture for initialization
// textureDamaged - damaged texture for when mushroom is hit
void generateMushrooms(size_t n, std::list<Mushroom>& mushrooms, Grid grid, sf::Texture& texture, sf::Texture& textureDamaged)
{
	std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, grid.rows*grid.cols-1);
	// Keep track of where mushrooms have been generated. Makes sure there are no duplicates
	std::vector<bool> mushMap (grid.rows*grid.cols, false);
	// Determines mushroom scale factor
	float maxTextureDim = float(texture.getSize().x > texture.getSize().y ? texture.getSize().x : texture.getSize().y);
	float scaleFactor = grid.size / maxTextureDim;

	// Create n mushrooms
	for (size_t i = 0; i < n; i++) 
	{
		size_t pos = distrib(gen);
		while (mushMap[pos]) 
		{
			pos = distrib(gen);
		}
		mushMap[pos] = true;
		// Find (x,y) coordinates for grid positions
		float posY = (floor(pos / grid.cols)) * grid.size + grid.yOffset;
		float posX = (pos % (int) grid.cols) * grid.size + grid.xOffset;
		mushrooms.emplace_back(posX, posY, texture, textureDamaged);
		mushrooms.back().setScale(scaleFactor, scaleFactor);
	}
	return;
}