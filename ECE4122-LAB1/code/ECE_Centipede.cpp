/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This implements the Centipede class. This was by-far the most painful part and caused me emotional pain for days on end.
It is not complete of bug-free (haha), but the time for fixing it has passed.
*/

#include "ECE_Centipede.h"

Segment::Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(startX, startY);
	gameGrid_ = gameGrid;
	this->dir = dir;
	nextY_ = gameGrid.y2Row(startY);
}

Segment::Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir)
{
	setTexture(texture);
	setOrigin(texture.getSize().x/2, texture.getSize().y/2);
	setPosition(start);
	gameGrid_ = gameGrid;
	this->dir = dir;
	nextY_ = gameGrid.y2Row(start.y);
}

void Segment::collide()
{
	lastDir = dir;
	setPosition(gameGrid_.gridSnap(getPosition()));
	if (descending_)
	{
		dir = sf::Vector2f(0,1); // DOWN
		nextY_ += 1;
	}
	else
	{
		dir = sf::Vector2f(0,-1); // UP
		nextY_ -= 1;
	}
}

void Segment::rotate()
{
	if (dir.x == 1)
	{
		setRotation(0);
	}
	else if (dir.x == -1)
	{
		setRotation(180);
	}
	else if (dir.y == 1)
	{
		setRotation(270);
	}
	else
	{
		setRotation(90);
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
			if(descending_) {
				// Check if we're at the end of the row
				if (nextY_ < gameGrid_.rows-1) {
					dir = sf::Vector2f(0,1); // DOWN
					nextY_ += 1;
				}
				else {
					dir = sf::Vector2f(0,-1); // UP
					nextY_ -= 1;
					descending_ = false;
				}
			}
			else {
				if (nextY_ > 1) {
					dir = sf::Vector2f(0,-1); // UP
					nextY_ -= 1;
				}
				else {
					dir = sf::Vector2f(0,1); // DOWN
					nextY_ += 1;
					descending_ = true;
				}
			}
			rotate();
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
			if(descending_) {
				// Check if we're at the end of the row
				if (nextY_ < gameGrid_.rows-1) {
					dir = sf::Vector2f(0,1); // DOWN
					nextY_ += 1;
				}
				else {
					dir = sf::Vector2f(0,-1); // UP
					nextY_ -= 1;
					descending_ = false;
				}
			}
			else {
				if (nextY_ > 4) {
					dir = sf::Vector2f(0,-1); //DOWN
					nextY_ -= 1;
				}
				else {
					dir = sf::Vector2f(0,1); // UP
					nextY_ += 1;
					descending_ = true;
				}
			}
			rotate();
		}
	}
	else if (dir == sf::Vector2f(0,1)) // DOWN
	{
		newPosition.y += speed_ * dt.asSeconds();
		if (newPosition.y >= gameGrid_.row2Coord(nextY_)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			sf::Vector2f newDir(lastDir.x*(-1), lastDir.y*(-1));
			lastDir = dir;
			dir = newDir;
			rotate();
		}
	}
	else if (dir == sf::Vector2f(0,-1)) // UP
	{
		newPosition.y -= speed_ * dt.asSeconds();
		
		if (newPosition.y <= gameGrid_.row2Coord(nextY_)) 
		{
			newPosition = gameGrid_.gridSnap(newPosition);
			sf::Vector2f newDir(lastDir.x*(-1), lastDir.y*(-1));
			lastDir = dir;
			dir = newDir;
			rotate();
		}
	}

	setPosition(newPosition);
}

void Segment::setHead(sf::Texture& texture) 
{
	if (dir.y == 1)
		nextY_ = ceil((getPosition().y - gameGrid_.yOffset) / gameGrid_.size) + gameGrid_.yOffset;
	else if (dir.y == -1)
		nextY_ = floor((getPosition().y - gameGrid_.yOffset) / gameGrid_.size) + gameGrid_.yOffset;
	else
		nextY_ = gameGrid_.y2Row(getPosition().y);

	setTexture(texture);
}

void Segment::reverse()
{
	dir.x *= -1;
	dir.y *= -1;
	lastDir.x *= -1;
	lastDir.y *= -1;
	rotate();
}

ECE_Centipede::ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, std::map<std::string,sf::Texture>& textures)
{
	sf::Vector2u headSize = textures["CentipedeHead.png"].getSize();
	float maxTextureDimHead = float(headSize.x > headSize.y ? headSize.x : headSize.y);
	float scaleFactorHead = gameGrid.size / maxTextureDimHead;
	sf::Vector2u bodySize = textures["CentipedeBody.png"].getSize();
	float maxTextureDimBody = float(textures["CentipedeBody.png"].getSize().x > bodySize.y ? bodySize.x : bodySize.y);
	float scaleFactorBody = gameGrid.size / maxTextureDimBody;
	
	segments_.emplace_front(headPosition.x, headPosition.y, gameGrid, textures["CentipedeHead.png"], sf::Vector2f(1,0));
	segments_.front().setScale(scaleFactorHead, scaleFactorHead);
	segments_.front().isHead = true;
	for (size_t i = 1; i < size; i++)
	{
		segments_.emplace_back(headPosition.x - i*gameGrid.size, headPosition.y, gameGrid, textures["CentipedeBody.png"], sf::Vector2f(1,0));
		segments_.back().setScale(scaleFactorBody, scaleFactorBody);
	}
	
	this->textures_ = &textures;
	gameGrid_ = gameGrid;
}

ECE_Centipede::ECE_Centipede(std::list<Segment>& segments, Grid gameGrid, std::map<std::string,sf::Texture>& textures)
{
	sf::Vector2u headSize = textures["CentipedeHead.png"].getSize();
	float maxTextureDimHead = float(headSize.x > headSize.y ? headSize.x : headSize.y);
	float scaleFactorHead = gameGrid.size / maxTextureDimHead;
	sf::Vector2u bodySize = textures["CentipedeBody.png"].getSize();
	float maxTextureDimBody = float(textures["CentipedeBody.png"].getSize().x > bodySize.y ? bodySize.x : bodySize.y);
	float scaleFactorBody = gameGrid.size / maxTextureDimBody;
	
	for (auto& segment : segments)
	{
		segments_.emplace_back(segment);
	}
	this->textures_ = &textures;
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

float ECE_Centipede::checkCollision(sf::FloatRect ship, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms, std::list<ECE_Centipede>& Centipedes)
{
	float score = 0;
	for (auto& mushroom : mushrooms)
	{
		if (mushroom.getGlobalBounds().intersects(segments_.front().getGlobalBounds())) {
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
	for (auto segment = segments_.begin(); segment != segments_.end(); ) {
		bool hit = false;
		if (ship.intersects(segment->getGlobalBounds()))
		{
			return -1;
		}
		for (auto blast = blasts.begin(); blast != blasts.end();)
		{
			if (blast->getGlobalBounds().intersects(segment->getGlobalBounds()))
			{
				hit = true;
				blast = blasts.erase(blast);
				if (segment->isHead)
					score += 100;
				else
					score += 10;

				// Place mushroom at location of bounce
				sf::Vector2u mushroomSize = (*textures_)["Mushroom0.png"].getSize();
				float scaleFactor = gameGrid_.size / float(mushroomSize.x > mushroomSize.y ? mushroomSize.x : mushroomSize.y);
				mushrooms.emplace_back(gameGrid_.gridSnap(segment->getPosition()), (*textures_)["Mushroom0.png"], (*textures_)["Mushroom1.png"]);
				mushrooms.back().setScale(scaleFactor, scaleFactor);

				segment = segments_.erase(segment);
				if (segment != segments_.end())
				{
					std::list<Segment> newCentipede;
					while (segment != segments_.end())
					{
						newCentipede.emplace_front(*segment);
						newCentipede.front().reverse();
						segment = segments_.erase(segment);
					}
					newCentipede.front().setHead((*textures_)["CentipedeHead.png"]);
					Centipedes.emplace_back(newCentipede, gameGrid_, *textures_);
				}
				return score;
			}
			else {
				++blast;
			}
		}
		if (!hit) 
		{
			++segment;
		}
	}
	
	return score;
}

size_t ECE_Centipede::size()
{
	return segments_.size();
}