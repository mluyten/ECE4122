#include "Mushroom.h"

// This the constructor function
Mushroom::Mushroom(float startX, float startY, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	setPosition(startX, startY);
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

Mushroom::Mushroom(sf::Vector2f start, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	setPosition(start);
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

void Mushroom::damage()
{
	setTexture(*textureDamaged_);
	damaged = true;
}

void generateMushrooms(size_t n, std::list<Mushroom>& mushrooms, Grid grid, sf::Texture& texture, sf::Texture& textureDamaged)
{
	std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, grid.rows*grid.cols-1);
	std::vector<bool> mushMap (grid.rows*grid.cols, false);
	float maxTextureDim = float(texture.getSize().x > texture.getSize().y ? texture.getSize().x : texture.getSize().y);
	float scaleFactor = grid.size / maxTextureDim;
	for (size_t i = 0; i < n; i++) 
	{
		size_t pos = distrib(gen);
		while (mushMap[pos]) 
		{
			pos = distrib(gen);
		}
		mushMap[pos] = 1;
		float posY = (floor(pos / grid.cols)) * grid.size + grid.yOffset;
		float posX = (pos % grid.cols) * grid.size + grid.xOffset;
		mushrooms.emplace_back(posX, posY, texture, textureDamaged);
		mushrooms.back().setScale(scaleFactor, scaleFactor);
	}
	return;
}