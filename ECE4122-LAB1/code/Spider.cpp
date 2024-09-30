/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This file implements the Spider class.
*/

#include "Spider.h"
#include <iostream>

// Constructor
// (startX, startY) - Starting position for spider
// texture - Texture to load into sprite
Spider::Spider(float startX, float startY, sf::Texture& texture)
{
    setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
    initialPos_ = getPosition();
    xDir_ = 1;
	state_ = 1; 
	nextState_ = 0;
}

// start - Starting position vector for spider
// texture - Texture to load into sprite
Spider::Spider(sf::Vector2f start, sf::Texture& texture)
{
    setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
    initialPos_ = start;
    xDir_ = 1;
	state_ = 1; 
	nextState_ = 0;
}

// Sets the range for which the spider can move around.
// minRange - Minimum x and y where spider can be located
// maxRange - Maximum x and y where spider can be located
void Spider::setRange(sf::Vector2f minRange, sf::Vector2f maxRange)
{
    minRange_ = minRange;
    maxRange_ = maxRange;
	return;
}


// Updates position of the spider
void Spider::update(sf::Time dt)
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(25, 100);
    
    if (state_ == 0) // Move state - executing a command
    {
        // Figure out next position based on command direction
        sf::Vector2f nextPos = getPosition() + commands_.front().first * speed_ * dt.asSeconds();
        // If out of the bounding box, get coerced position inside bounding box
        sf::Vector2f boundedPos = coerceBounds(nextPos, minRange_, maxRange_);

        // Check if command time has elapsed or spider attempted to leave bounding box
        if (time_ >= commands_.front().second || nextPos != boundedPos) 
        {
            // Remove last command and reset time
            commands_.pop();
            time_ = 0;

            // If out of commands, 
            if (commands_.size() == 0)
            {
                // If we're at the horizontal boundaries, change x direction
                if (boundedPos.x == maxRange_.x || boundedPos.x == minRange_.x)
                    xDir_ *= -1;

                // Go to wait state for a random amount of time
                commands_.push(std::pair<sf::Vector2f, float>(getPosition(), (float) distrib(gen)/100));
                state_ = 3;
            }
        }
        else
        {                     
            setPosition(nextPos);
            time_ += dt.asSeconds();
        }
    }
    else if (state_ == 1) // Load up zig-zag commands
    {
        // Sets diagonal direction unit vector
        sf::Vector2f dir(1/sqrt(2), 1/sqrt(2));
        std::uniform_int_distribution<> zigDistrib(2, 5);
        // Sets unit vector horizontal sign based on overall direction spider is moving
        dir.x = dir.x * xDir_;
        // Randomly generations 2-5 diagonal movement commands of random durration
        for (int i = 0; i < zigDistrib(gen); i++)
        {
            dir.y = dir.y * -1; // induces zig-zag motion
            commands_.push(std::pair<sf::Vector2f, float>(dir, (float) distrib(gen)/150));
        }
        // Go to move state next, then go to vertical movement after that
        state_ = 0;
        nextState_ = 2;
        time_ = 0;
    }
    else if (state_ == 2) // Prepare to move vertically
    {
        // Set up and down unit vectors
        sf::Vector2f up(0, -1);
        sf::Vector2f down(0, 1);
        commands_.push(std::pair<sf::Vector2f, float>(up, (float) distrib(gen)/150)); // Move up for a random duration
        commands_.push(std::pair<sf::Vector2f, float>(down, (float) distrib(gen)/150)); // Move down for a random dutation
        // Go to move state, then go to zig zag movement
        state_ = 0;
        nextState_ = 1;
    }
    else if (state_ == 3) // Random delay before next state
    {
        // Check if waited long enough
        if (time_ > commands_.front().second)
        {
            // Move on to next state
            state_ = nextState_;
            commands_.pop();
            time_ = 0;
        }
        else
            time_ += dt.asSeconds();
    }
	return;
}

// Resets spider if killed
// timeout - Time to wait before spider starts crawling around again
void Spider::reset(float timeout)
{
    std::queue<std::pair<sf::Vector2f, float>> empty;
    std::swap(commands_, empty); // Empty commands queue
    commands_.push(std::pair<sf::Vector2f, float>(getPosition(), timeout));
    state_ = 3; // Wait timeout;
    nextState_ = 1; // Zig zag after timeout
    xDir_ = 1;
    time_ = 0;
    setPosition(initialPos_);
	return;
}

// Checks collisions with mushroom list and removes any that collide
// mushrooms - list of mushrooms in play area
void Spider::eatMushrooms(std::list<Mushroom>& mushrooms)
{
	for (auto mushroom = mushrooms.begin(); mushroom != mushrooms.end(); ) 
	{
		if (mushroom->getGlobalBounds().intersects(getGlobalBounds()))
		{
			mushroom = mushrooms.erase(mushroom);
			break;
		}
		else {
			++mushroom;
		}
	}
	return;
}