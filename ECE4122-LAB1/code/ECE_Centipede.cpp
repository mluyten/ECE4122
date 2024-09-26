#include "ECE_Centipede.h"

#include <iostream>

ECE_Centipede::ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, sf::Texture& headTexture, sf::Texture& bodyTexture)
{
	float maxTextureDimHead = float(headTexture.getSize().x > headTexture.getSize().y ? headTexture.getSize().x : headTexture.getSize().y);
	float scaleFactorHead = gameGrid.size / maxTextureDimHead;

	float maxTextureDimBody = float(bodyTexture.getSize().x > bodyTexture.getSize().y ? bodyTexture.getSize().x : bodyTexture.getSize().y);
	float scaleFactorBody = gameGrid.size / maxTextureDimBody;
	
	segments_.emplace_front(headPosition.x, headPosition.y, gameGrid, headTexture, Segment::Direction::RIGHT);
	segments_.front().setScale(scaleFactorHead, scaleFactorHead);
	for (size_t i = 1; i < size; i++)
	{
		segments_.emplace_back(headPosition.x - i*gameGrid.size, headPosition.y, gameGrid, bodyTexture, Segment::Direction::RIGHT);
		segments_.back().setScale(scaleFactorBody, scaleFactorBody);
	}
	
	bodyTexture_ = &bodyTexture;
	headTexture_ = &headTexture;
	gameGrid_ = gameGrid;
}

void ECE_Centipede::update(sf::Time dt)
{
	for(auto& segment : segments_)
	{
		segment.update(dt);
	}
}

void ECE_Centipede::draw(sf::RenderWindow& window)
{
	for(auto& segment : segments_)
	{
		window.draw(segment);
	}
}

sf::Vector2f ECE_Centipede::checkCollision(sf::FloatRect object, std::list<ECE_Centipede>* Centipedes)
{
	for (auto segment = segments_.begin(); segment != segments_.end(); ) {
		if (object.intersects(segment->getGlobalBounds())) {
			if (Centipedes == nullptr)
			{
				if (segment->getDirection() == Segment::Direction::UP ||
						segment->getDirection() == Segment::Direction::DOWN)
				{
					// Nothing?
				}
				else 
				{
					segment->collide();
					return segment->getPosition();
				}
				++segment;
			}
			else
			{
				// Delete segment and make new centipede
			}
		}
		else
		{
			++segment;
		}
	}
	return sf::Vector2f(-1, -1);
}

// This the constructor and it is called when we create an object
Segment::Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, Direction dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
	gameGrid_ = gameGrid;
	dir_ = dir;
	nextPos_ = sf::Vector2u(0,4);
}

Segment::Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, Direction dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
	gameGrid_ = gameGrid;
	dir_ = dir;
}

void Segment::setDirection(Direction dir)
{
	dir_ = dir;
}

Segment::Direction Segment::getDirection()
{
	return dir_;
}

void Segment::collide()
{
	lastDir_ = dir_;
	setPosition(gameGrid_.gridSnap(getPosition()));
	if (descending)
	{
		dir_ = Direction::DOWN;
		nextPos_.y += 1;
	}
	else
	{
		dir_ = Direction::UP;
		nextPos_.y -= 1;
	}
}

void Segment::update(sf::Time dt)
{
	sf::Vector2f newPosition = getPosition();
	if (dir_ == Direction::LEFT) {
		newPosition.x -= speed_ * dt.asSeconds();
		// Check if we've arrived at beginning of row
		if (newPosition.x < gameGrid_.col2Coord(0))
		{
			// Snap position to the grid
			newPosition = gameGrid_.gridSnap(newPosition);
			// Check if we're descending the play area
			if(descending) {
				// Check if we're at the end of the row
				if (nextPos_.y < gameGrid_.rows-1) {
					dir_ = Direction::DOWN;
					nextPos_.y += 1;
				}
				else {
					dir_ = Direction::UP;
					nextPos_.y -= 1;
					descending = false;
				}
			}
			else {
				if (nextPos_.y > 1) {
					dir_ = Direction::UP;
					nextPos_.y -= 1;
				}
				else {
					dir_ = Direction::DOWN;
					nextPos_.y += 1;
					descending = true;
				}
			}
		}
	}
    else if (dir_ == Direction::RIGHT) {
		newPosition.x += speed_ * dt.asSeconds();
		// Check if we've arrived at end of row
		if (newPosition.x >= gameGrid_.col2Coord(gameGrid_.cols-1))
		{
			// Snap position to the grid
			newPosition = gameGrid_.gridSnap(newPosition);
			// Check if we're descending the play area
			if(descending) {
				// Check if we're at the end of the row
				if (nextPos_.y < gameGrid_.rows-1) {
					dir_ = Direction::DOWN;
					nextPos_.y += 1;
				}
				else {
					dir_ = Direction::UP;
					nextPos_.y -= 1;
					descending = false;
				}
			}
			else {
				if (nextPos_.y > 4) {
					dir_ = Direction::UP;
					nextPos_.y -= 1;
				}
				else {
					dir_ = Direction::DOWN;
					nextPos_.y += 1;
					descending = true;
				}
			}
		}
	}
	else if (dir_ == Direction::DOWN) 
	{
		newPosition.y += speed_ * dt.asSeconds();
		if (newPosition.y >= gameGrid_.row2Coord(nextPos_.y)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			if (newPosition.x > gameGrid_.col2Coord(gameGrid_.cols-1)/2)
			{
				dir_ = Direction::LEFT;
			}
			else 
			{
				dir_ = Direction::RIGHT;
			}
		}
	}
	else if (dir_ == Direction::UP) 
	{
		newPosition.y -= speed_ * dt.asSeconds();
		
		if (newPosition.y <= gameGrid_.row2Coord(nextPos_.y)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			if (newPosition.x > gameGrid_.col2Coord(gameGrid_.cols-1)/2)
			{
				dir_ = Direction::LEFT;
			}
			else 
			{
				dir_ = Direction::RIGHT;
			}
		}
	}

	setPosition(newPosition);
}
