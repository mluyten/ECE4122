/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified:

Description:
*/
#include "Robot.h"

#include <iostream>

/**
 * Constructor with starting position, and window dimensions.
 * Sprite origin is changed to middle of the texture and ranges are set so that
 * the edges of the sprite cannot leave the defined window dimensins.
 */
Robot::Robot(sf::Vector2f start, sf::Texture& texture, sf::Vector2f windowDims)
{
	setTexture(texture);
	setPosition(start);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	_minRange.x = texture.getSize().x/2;
	_minRange.y = texture.getSize().y/2;
	_maxRange.x = windowDims.x - texture.getSize().x/2;
	_maxRange.y = windowDims.y - texture.getSize().y/2;
}

/**
 * Changes direction of robot and rotates it to face direction of travel. This is done using
 * atan2. If new direction is the same as the old direction, do nothing.
 */
void Robot::changeDirection(sf::Vector2f dir) {
	if (_direction != dir) {
		double dot = _direction.x*dir.x + _direction.y*dir.y;
		double det = _direction.x*dir.y-_direction.y*dir.x;
		rotate(180 * atan2(det, dot) / M_PI);
		_direction = dir;
	}
	return;
}

/**
 * Increments the speed by given value. Speed cannot be less than 0.
 */
void Robot::incrementSpeed(double increment) {
	_speed += increment;
	if (_speed < 0) _speed = 0;
}

/**
 * Updates the robot position based on the time delta between now and the last update.
 * This coerces the robot position to fall within the defined window dimensions.
 */
void Robot::update(sf::Time dt)
{
	sf::Vector2f position = getPosition();
	position += _direction * _speed * dt.asSeconds();
	
	// Ensure robot stays in view
	if (position.x > _maxRange.x) position.x = _maxRange.x;
	if (position.x < _minRange.x) position.x = _minRange.x;
	if (position.y > _maxRange.y) position.y = _maxRange.y;
	if (position.y < _minRange.y) position.y = _minRange.y;

	setPosition(position);
	return;
}
