#include "Starship.h"

// This the constructor and it is called when we create an object
Starship::Starship(float startX, float startY, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
}

Starship::Starship(sf::Vector2f start, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
}

void Starship::setRange(sf::Vector2f minRange, sf::Vector2f maxRange)
{
	minRange_ = minRange;
	maxRange_ = maxRange;
}

void Starship::moveLeft()
{
	movingLeft_ = true;

}

void Starship::moveRight()
{
	movingRight_ = true;
}

void Starship::moveUp()
{
	movingUp_ = true;

}

void Starship::moveDown()
{
	movingDown_ = true;
}

void Starship::stopLeft()
{
	movingLeft_ = false;

}

void Starship::stopRight()
{
	movingRight_ = false;
}

void Starship::stopUp()
{
	movingUp_ = false;

}

void Starship::stopDown()
{
	movingDown_ = false;
}

void Starship::update(sf::Time dt)
{
	sf::Vector2f nextPos = getPosition();
	if (movingLeft_) {
		nextPos.x -= speed_ * dt.asSeconds();
	}

	if (movingRight_) {
		nextPos.x += speed_ * dt.asSeconds();
	}

	if (movingDown_) {
		nextPos.y += speed_ * dt.asSeconds();
	}

	if (movingUp_) {
		nextPos.y -= speed_ * dt.asSeconds();
	}

	setPosition(coerceBounds(nextPos, minRange_, maxRange_));
}
