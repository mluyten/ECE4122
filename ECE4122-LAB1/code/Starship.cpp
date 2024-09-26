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
	sf::Vector2f newPosition = getPosition();
	if (movingLeft_) {
		newPosition.x -= speed_ * dt.asSeconds();
	}

	if (movingRight_) {
		newPosition.x += speed_ * dt.asSeconds();
	}

	if (movingDown_) {
		newPosition.y += speed_ * dt.asSeconds();
	}

	if (movingUp_) {
		newPosition.y -= speed_ * dt.asSeconds();
	}

	if (newPosition.x > maxRange_.x)
	{
		newPosition.x = maxRange_.x;
	}
	else if (newPosition.x < minRange_.x) 
	{
		newPosition.x =- minRange_.x;
	}

	if  (newPosition.y > maxRange_.y)
	{
		newPosition.y = maxRange_.y;
	}
	else if (newPosition.y < minRange_.y)
	{
		newPosition.y = minRange_.y;
	}

	setPosition(newPosition);
}
