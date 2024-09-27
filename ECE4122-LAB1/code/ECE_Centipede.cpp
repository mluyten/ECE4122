#include "ECE_Centipede.h"

#include <iostream>

float distance(sf::Vector2f a, sf::Vector2f b) 
{
	return abs(b.x - a.x) + abs(b.y - a.y);
}

float magnitude(sf::Vector2f v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// Function to normalize a vector
sf::Vector2f normalize(sf::Vector2f v) 
{
	float mag = std::sqrt(v.x * v.x + v.y * v.y);
	if (mag == 0) return sf::Vector2f(0, 0);
	return sf::Vector2f(v.x / mag, v.y / mag);
}

ECE_Centipede::ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, sf::Texture& headTexture, sf::Texture& bodyTexture)
{
	float maxTextureDimHead = float(headTexture.getSize().x > headTexture.getSize().y ? headTexture.getSize().x : headTexture.getSize().y);
	float scaleFactorHead = gameGrid.size / maxTextureDimHead;

	float maxTextureDimBody = float(bodyTexture.getSize().x > bodyTexture.getSize().y ? bodyTexture.getSize().x : bodyTexture.getSize().y);
	float scaleFactorBody = gameGrid.size / maxTextureDimBody;
	
	segments_.emplace_front(headPosition.x, headPosition.y, gameGrid, headTexture, sf::Vector2f(1,0));
	segments_.front().setScale(scaleFactorHead, scaleFactorHead);
	for (size_t i = 1; i < size; i++)
	{
		segments_.emplace_back(headPosition.x - i*gameGrid.size, headPosition.y, gameGrid, bodyTexture, sf::Vector2f(1,0));
		segments_.back().setScale(scaleFactorBody, scaleFactorBody);
	}
	
	bodyTexture_ = &bodyTexture;
	headTexture_ = &headTexture;
	gameGrid_ = gameGrid;
}

void ECE_Centipede::update(sf::Time dt)
{
	for(auto it = segments_.begin(); it != segments_.end(); ++it)
	{
		if (it == segments_.begin())
		{
			it->update(dt);
			auto& nextSeg = *std::next(it);
			//std::cout << "POS " << nextSeg.getPosition().x << " " << nextSeg.getPosition().y << "\n";
			//std::cout << "DIR " << nextSeg.dir.x << " " << nextSeg.dir.y << "\n";
			//std::cout << "LAST DIR " << nextSeg.dir.x << " " << nextSeg.dir.y << "\n";
		}
		else
		{
			auto& thisSeg = *it;
			auto& prevSeg = *std::prev(it);
			sf::Vector2f dir = prevSeg.getPosition() - thisSeg.getPosition();
			float dist = distance(prevSeg.getPosition(), thisSeg.getPosition());
			float distDelta = dist - gameGrid_.size;
			if (distDelta > 0) {
				sf::Vector2f mv(thisSeg.dir.x * dir.x, thisSeg.dir.y * dir.y);
				if (mv.x != 0 && abs(mv.x) >= abs(distDelta))
				{
					mv.x = thisSeg.dir.x * distDelta;
				}
				else if (mv.y != 0 && abs(mv.y) >= abs(distDelta))
				{
					mv.y = thisSeg.dir.y * distDelta;
				}
				thisSeg.move(mv);
				
			}
			
			dir = prevSeg.getPosition() - thisSeg.getPosition();
			if (std::prev(it)->dir != it->dir)
			{
				if (thisSeg.dir.x * dir.x + thisSeg.dir.y * dir.y == 0) {
					thisSeg.dir = normalize(dir);
				}
			}
		}
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
	if (Centipedes == nullptr) // This is not a laser collision
	{
		if (object.intersects(segments_.front().getGlobalBounds())) {
			if (segments_.front().dir == sf::Vector2f(0,-1) || // UP
					segments_.front().dir == sf::Vector2f(0,1))
			{
				// Nothing?
			}
			else 
			{
				segments_.front().collide();
			}
		}
	}
	else
	{
		for (auto segment = segments_.begin(); segment != segments_.end(); ) {
			if (object.intersects(segment->getGlobalBounds()))
			{
				// Remove segment and add new centipede
				return segment->getPosition();
			}
			else
			{
				++segment;
			}
		}
	}
	return sf::Vector2f(-1, -1);
}

// This the constructor and it is called when we create an object
Segment::Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
	gameGrid_ = gameGrid;
	this->dir = dir;
	nextPos_ = sf::Vector2u(0,4);
}

Segment::Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
	gameGrid_ = gameGrid;
	this->dir = dir;
}

void Segment::collide()
{
	lastDir = dir;
	setPosition(gameGrid_.gridSnap(getPosition()));
	if (descending)
	{
		dir = sf::Vector2f(0,1); // DOWN
		nextPos_.y += 1;
	}
	else
	{
		dir = sf::Vector2f(0,-1); // UP
		nextPos_.y -= 1;
	}
}

void Segment::update(sf::Time dt)
{
	sf::Vector2f newPosition = getPosition();
	if (dir == sf::Vector2f(-1,0)) {
		newPosition.x -= speed_ * dt.asSeconds();
		// Check if we've arrived at beginning of row
		if (newPosition.x < gameGrid_.col2Coord(0))
		{
			// Snap position to the grid
			newPosition = gameGrid_.gridSnap(newPosition);
			lastDir = dir;
			// Check if we're descending the play area
			if(descending) {
				// Check if we're at the end of the row
				if (nextPos_.y < gameGrid_.rows-1) {
					dir = sf::Vector2f(0,1); // DOWN
					nextPos_.y += 1;
				}
				else {
					dir = sf::Vector2f(0,-1); // UP
					nextPos_.y -= 1;
					descending = false;
				}
			}
			else {
				if (nextPos_.y > 1) {
					dir = sf::Vector2f(0,-1); // UP
					nextPos_.y -= 1;
				}
				else {
					dir = sf::Vector2f(0,1); // DOWN
					nextPos_.y += 1;
					descending = true;
				}
			}
		}
	}
    else if (dir == sf::Vector2f(1,0)) {
		newPosition.x += speed_ * dt.asSeconds();
		// Check if we've arrived at end of row
		if (newPosition.x >= gameGrid_.col2Coord(gameGrid_.cols-1))
		{
			// Snap position to the grid
			newPosition = gameGrid_.gridSnap(newPosition);
			lastDir = dir;
			// Check if we're descending the play area
			if(descending) {
				// Check if we're at the end of the row
				if (nextPos_.y < gameGrid_.rows-1) {
					dir = sf::Vector2f(0,1); // DOWN
					nextPos_.y += 1;
				}
				else {
					dir = sf::Vector2f(0,-1); // UP
					nextPos_.y -= 1;
					descending = false;
				}
			}
			else {
				if (nextPos_.y > 4) {
					dir = sf::Vector2f(0,-1); //DOWN
					nextPos_.y -= 1;
				}
				else {
					dir = sf::Vector2f(0,1); // UP
					nextPos_.y += 1;
					descending = true;
				}
			}
		}
	}
	else if (dir == sf::Vector2f(0,1)) // DOWN
	{
		newPosition.y += speed_ * dt.asSeconds();
		if (newPosition.y >= gameGrid_.row2Coord(nextPos_.y)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			sf::Vector2f newDir(lastDir.x*(-1), lastDir.y*(-1));
			lastDir = dir;
			dir = newDir;
			//if (newPosition.x > gameGrid_.col2Coord(gameGrid_.cols-1)/2)
			//{
			//	dir = lastDir * (-1);
			//}
			//else 
			//{
			//	dir = sf::Vector2f(1,0);
			//}
		}
	}
	else if (dir == sf::Vector2f(0,-1)) // UP
	{
		newPosition.y -= speed_ * dt.asSeconds();
		
		if (newPosition.y <= gameGrid_.row2Coord(nextPos_.y)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			sf::Vector2f newDir(lastDir.x*(-1), lastDir.y*(-1));
			lastDir = dir;
			dir = newDir;
			//if (newPosition.x > gameGrid_.col2Coord(gameGrid_.cols-1)/2)
			//{
			//	dir = sf::Vector2f(-1,0);
			//}
			//else 
			//{
			//	dir = sf::Vector2f(1,0);
			//}
		}
	}

	setPosition(newPosition);
}
