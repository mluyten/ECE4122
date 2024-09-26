#include "ECE_LaserBlast.h"

// This the constructor function
ECE_LaserBlast::ECE_LaserBlast(float startX, float startY, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
}

ECE_LaserBlast::ECE_LaserBlast(sf::Vector2f start, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
}

void ECE_LaserBlast::update(sf::Time dt)
{
	// Update the ECE_LaserBlast position variables
	sf::Vector2f deltaPosition;
	deltaPosition.y = -1 * speed * dt.asSeconds();

	// Move the ECE_LaserBlast and the bat
	move(deltaPosition);
}