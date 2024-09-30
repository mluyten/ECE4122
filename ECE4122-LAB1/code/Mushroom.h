/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the header for the Mushroom class and its related generation functon.
*/

#pragma once
#include <random>
#include <list>
#include "GameUtils.h"
#include <SFML/Graphics.hpp>

class Mushroom : public sf::Sprite
{
private:
	sf::Texture* textureDamaged_;

public:
	Mushroom(float startX, float startY, sf::Texture& textureFull, sf::Texture& textureDamaged);
	Mushroom(sf::Vector2f start, sf::Texture& textureFull, sf::Texture& textureDamaged);

	void damage();
	bool damaged;
};

void generateMushrooms(size_t n, std::list<Mushroom>& mushrooms, Grid grid, sf::Texture& texture, sf::Texture& textureDamaged);