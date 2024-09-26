#pragma once
#include <SFML/Graphics.hpp>

class ECE_LaserBlast : public sf::Sprite
{
private:
	float speed = 1000.0f;
	float directionY = .2f;

public:
	ECE_LaserBlast(float startX, float startY, sf::Texture& texture);
	ECE_LaserBlast(sf::Vector2f start, sf::Texture& texture);

	void update(sf::Time dt);

};
