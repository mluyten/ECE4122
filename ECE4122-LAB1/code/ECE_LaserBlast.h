/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the header for the ECE_LaserBlast Class and its related functions.
*/
#pragma once
#include <list>
#include "Spider.h"
#include "Mushroom.h"
#include <SFML/Graphics.hpp>

// The laser blast is a simple sprite that moves horizontally when created
class ECE_LaserBlast : public sf::Sprite
{
private:
	float speed = 1000.0f;

public:
	ECE_LaserBlast(float startX, float startY, sf::Texture& texture);
	ECE_LaserBlast(sf::Vector2f start, sf::Texture& texture);

	void update(sf::Time dt);

};

// Checks collisitions between blasts and mushrooms and updates score/mushrooms if needed
void blastMushrooms(int& score, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms);

// Checks collisions between blasts and spider and updates score/spider if needed
void blastSpider(int& score, std::list<ECE_LaserBlast>& blasts, Spider& spider);

// Remove blasts that are out of bounds
void cullBlasts(std::list<ECE_LaserBlast>& blasts);