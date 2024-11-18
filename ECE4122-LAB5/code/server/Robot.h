/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/16/2024

Description:
This is the header for the Robot class
*/
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

/**
 * Extends a sprite to implement a robot that has variable speed and direction
 */
class Robot : public sf::Sprite
{
private:
	float _speed = 3.0f;

	sf::Vector2f _direction = sf::Vector2f(0, -1);

	// X and Y ranges for which the robot can exist
	sf::Vector2f _minRange;
	sf::Vector2f _maxRange;

public:
	/**
	 * Default constructor
	 */
	Robot() {}

	/**
	 * Constructor with starting position, and window dimensions
	 * 
	 * @param start the x component of the starting position
	 * @param texture the SFML texture to be loaded as the sprite
	 * @param windowDims the window dimensions - sets the bounds for the robot
	 */
	Robot(sf::Vector2f start, sf::Texture& texture, sf::Vector2f windowDims);

	/**
	 * Changes the direction of the robot and rotates the robot to face direction of travel
	 * 
	 * @param dir the desired direction of travel
	 */
	void changeDirection(sf::Vector2f dir);

	/**
	 * Increments speed by reuested amount
	 * 
	 * @param increment the amount by which to increment speed
	 */
	void incrementSpeed(double increment);

	/**
	 * Updates current position of robot based on change in time
	 * 
	 * @param dt the difference in time beteween now and last update
	 */
	void update(sf::Time dt);
};
