/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description

This implements the ECE_LaserBlast Class.
*/

#include "ECE_LaserBlast.h"

// Constructor
// (startX, startY) - Starting position for laser
// texture - Texture to load into sprite
ECE_LaserBlast::ECE_LaserBlast(float startX, float startY, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
}

// Constructor
// (startX, startY) - Starting position for laser
// texture - Texture to load into sprite
ECE_LaserBlast::ECE_LaserBlast(sf::Vector2f start, sf::Texture& texture)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
}

// Updates position of laser
// dt - change in time from last update
void ECE_LaserBlast::update(sf::Time dt)
{
	// Update the blast position
	sf::Vector2f deltaPosition;
	deltaPosition.y = -1 * speed * dt.asSeconds();

	// Move the blast
	move(deltaPosition);
	return;
}

// Checks collisitions between blasts and mushrooms and updates score/mushrooms if needed
// score - score value to be updated if mushroom destroyed
// blasts - list of blasts in play
// mushrooms - list of mushrooms in play
void blastMushrooms(int& score, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{
		bool hit = false;
		for (auto mushroom = mushrooms.begin(); mushroom != mushrooms.end(); ) 
		{
			if (mushroom->getGlobalBounds().intersects(blast->getGlobalBounds()))
			{
				// Check if mushroom has already been damaged
				if (mushroom->damaged)
				{
					// Remove mushrooms and update score
					mushroom = mushrooms.erase(mushroom);
					score += 4;
				}
				else
				{
					// Damage mushroom
					mushroom->damage();
				}
				// Delete blast
				blast = blasts.erase(blast);
				hit = true;
				
				break;
			}
			else {
				++mushroom;
			}
		}
		if (!hit)
		{
			++blast;
		}
	}
	return;
}

// Checks collisions between blasts and spider and updates score/spider if needed
// score - score value to be updated if mushroom destroyed
// blasts - list of blasts in play
// spider - spider sprite that may be hit by a blast
void blastSpider(int& score, std::list<ECE_LaserBlast>& blasts, Spider& spider)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{	
		// check if spider is hit by a blast
		if (blast->getGlobalBounds().intersects(spider.getGlobalBounds())) 
		{
			// generate random value from 300-900
			std::random_device rd;  // a seed source for the random number engine
			std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(3, 9);
			score += 100 * distrib(gen);
			blast = blasts.erase(blast);
			// Reset the spider with timeout 4 seconds
			spider.reset(4);
		}
		else
		{
			++blast;
		}
	}
	return;
}

// Checks if any blasts have left the window view area
// blasts - list of blasts in play
void cullBlasts(std::list<ECE_LaserBlast>& blasts)
{
	while(true)
	{
		if (blasts.size() > 0 && blasts.front().getPosition().y < 0) 
		{
			blasts.pop_front();
		}
		else
			break;
	}
	return;
}