#pragma once
#include "GameUtils.h"
#include "ECE_LaserBlast.h"
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