#include "Spider.h"
#include <iostream>

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

void Spider::setRange(sf::Vector2f minRange, sf::Vector2f maxRange)
{
    minRange_ = minRange;
    maxRange_ = maxRange;
}

void Spider::update(sf::Time dt)
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(25, 100);
    
    if (state_ == 0) 
    {
        sf::Vector2f nextPos = getPosition() + commands_.front().first * speed_ * dt.asSeconds();
        sf::Vector2f boundedPos = coerceBounds(nextPos, minRange_, maxRange_);
        if (time_ >= commands_.front().second || nextPos != boundedPos) 
        {
            commands_.pop();
            time_ = 0;
            if (commands_.size() == 0)
            {
                if (boundedPos.x == maxRange_.x || boundedPos.x == minRange_.x)
                    xDir_ *= -1;
                
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
        sf::Vector2f dir(1/sqrt(2), 1/sqrt(2));
        std::uniform_int_distribution<> zigDistrib(2, 5);
        dir.x = dir.x * xDir_;
        for (int i = 0; i < zigDistrib(gen); i++)
        {
            dir.y = dir.y * -1;
            commands_.push(std::pair<sf::Vector2f, float>(dir, (float) distrib(gen)/150));
        }
        state_ = 0;
        nextState_ = 2;
        time_ = 0;
    }
    else if (state_ == 2) // Prepare to move vertically
    {
        sf::Vector2f up(0, -1);
        sf::Vector2f down(0, 1);
        commands_.push(std::pair<sf::Vector2f, float>(up, (float) distrib(gen)/150));
        commands_.push(std::pair<sf::Vector2f, float>(down, (float) distrib(gen)/150));
        state_ = 0;
        nextState_ = 1;
    }
    else if (state_ == 3) // Random delay before next state
    {
        if (time_ > commands_.front().second)
        {
            state_ = nextState_;
            commands_.pop();
            time_ = 0;
        }
        else
            time_ += dt.asSeconds();
    }
}

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
}