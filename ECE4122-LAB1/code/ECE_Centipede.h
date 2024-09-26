#pragma once
#include <list>
#include "GameUtils.h"
#include <SFML/Graphics.hpp>

class Segment : public sf::Sprite
{
public:
	enum Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

private:
	Direction dir_;
	Direction lastDir_;
	bool descending = true;
	float speed_ = 450.0f;

	Grid gameGrid_;
	bool isHead = true;

	bool movingRight_ = false;
	bool movingLeft_ = false;
	bool movingUp_ = false;
	bool movingDown_ = false;

	sf::Vector2u nextPos_;
public:
	Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, Direction dir);

	Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, Direction dir);

	void setDirection(Direction dir);
	Direction getDirection();
	void collide();
	void update(sf::Time dt);

};

class ECE_Centipede 
{
private:
	sf::Texture* headTexture_;
	sf::Texture* bodyTexture_;

	std::list<Segment> segments_;
	Grid gameGrid_;
public:
	ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, sf::Texture& headTexture, sf::Texture& bodyTexture);

	void update(sf::Time dt);
	void draw(sf::RenderWindow& window);
	sf::Vector2f checkCollision(sf::FloatRect object, std::list<ECE_Centipede>* Centipedes=nullptr);
};