/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description

This implements the ECE_LaserBlast Class.
*/

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

void blastMushrooms(int& score, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms, sf::Time dt)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{
		bool hit = false;
		for (auto mushroom = mushrooms.begin(); mushroom != mushrooms.end(); ) 
		{
			if (mushroom->getGlobalBounds().intersects(blast->getGlobalBounds()))
			{
				if (mushroom->damaged)
				{
					mushroom = mushrooms.erase(mushroom);
					score += 4;
				}
				else
				{
					mushroom->damage();
				}
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
}

void blastSpider(int& score, std::list<ECE_LaserBlast>& blasts, Spider& spider, sf::Time dt)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{	
		if (blast->getGlobalBounds().intersects(spider.getGlobalBounds())) 
		{
			std::random_device rd;  // a seed source for the random number engine
			std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(3, 9);
			score += 100 * distrib(gen);
			blast = blasts.erase(blast);
			spider.reset(4);
		}
		else
		{
			++blast;
		}
	}
}

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
}