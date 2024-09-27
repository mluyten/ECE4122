#pragma once
#include <list>
#include <iterator>
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
	bool descending = true;
	float speed_ = 450.0f;

	Grid gameGrid_;
	bool isHead = true;
	sf::Vector2f ignore_; // In case we have to clip through 
	sf::Vector2u nextPos_;
public:
	Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);

	Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);

	void collide();
	void update(sf::Time dt);

	sf::Vector2f dir;
	sf::Vector2f lastDir;
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