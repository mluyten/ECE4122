/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the header for the Starship class (derrived from the Bat class from Pong)
*/
#pragma once
#include "GameUtils.h"
#include <SFML/Graphics.hpp>

// This class is the sprite that the player controls. It moves around the screen according to user inputs.
class Starship : public sf::Sprite
{
private:
	float speed_ = 550.0f;

	bool movingRight_ = false;
	bool movingLeft_ = false;
	bool movingUp_ = false;
	bool movingDown_ = false;

	// X and Y ranges for which the ship can exist
	sf::Vector2f minRange_;
	sf::Vector2f maxRange_;

public:
	Starship(float startX, float startY, sf::Texture& texture);

	Starship(sf::Vector2f start, sf::Texture& texture);

	void setRange(sf::Vector2f minRange, sf::Vector2f maxRange);

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	void update(sf::Time dt);

};
