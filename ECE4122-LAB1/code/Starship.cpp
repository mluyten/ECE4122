/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This implements the Starship class (derrived from the Bat class from Pong).
*/
#include "Starship.h"

// Constructor
// (startX, startY) - Starting position for starship
// texture - Texture to load into sprite
Starship::Starship(float startX, float startY, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
}

// Constructor
// start - Vector of starting position for starship
// texture - Texture to load into sprite
Starship::Starship(sf::Vector2f start, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
}

// Sets the range for which the starship can move around.
// minRange - Minimum x and y where starship can be located
// maxRange - Maximum x and y where starship can be located
void Starship::setRange(sf::Vector2f minRange, sf::Vector2f maxRange)
{
	minRange_ = minRange;
	maxRange_ = maxRange;
	return;
}

// Allows starship to move left
void Starship::moveLeft()
{
	movingLeft_ = true;
	return;
}

// Allows starship to move right
void Starship::moveRight()
{
	movingRight_ = true;
	return;
}

// Allows starship to move up
void Starship::moveUp()
{
	movingUp_ = true;
	return;

}

// Allows starship to move down
void Starship::moveDown()
{
	movingDown_ = true;
	return;
}

// Stops starship from moving left
void Starship::stopLeft()
{
	movingLeft_ = false;
	return;

}

// Stops starship from moving right
void Starship::stopRight()
{
	movingRight_ = false;
	return;
}

// Stops starship from moving up
void Starship::stopUp()
{
	movingUp_ = false;
	return;

}

// Stops starship from moving down
void Starship::stopDown()
{
	movingDown_ = false;
	return;
}


// Updates starship's position based on speed, direction, and time since last update
// dt - change in time from last update
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
	return;
}
