#pragma once
#include <list>
#include <iterator>
#include <map>
#include <iostream>
#include "GameUtils.h"
#include "ECE_LaserBlast.h"
#include "Mushroom.h"
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
	bool descending_ = true;
	float speed_ = 400.0f;
	Grid gameGrid_;
	
	sf::Vector2f ignore_; // In case we have to clip through 
	int nextY_;
public:
	Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);

	Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);

	void collide();
	void rotate();
	void update(sf::Time dt);
	void reverse();
	void setHead(sf::Texture& texture);
	
	bool isHead = false;
	sf::Vector2f dir;
	sf::Vector2f lastDir;
};

class ECE_Centipede 
{
private:
	std::map<std::string,sf::Texture>* textures_;

	std::list<Segment> segments_;
	Grid gameGrid_;
public:
	ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, std::map<std::string,sf::Texture>& textures);
	ECE_Centipede(std::list<Segment>& segments, Grid gameGrid, std::map<std::string,sf::Texture>& textures);

	void update(sf::Time dt);
	void draw(sf::RenderWindow& window);
	float checkCollision(sf::FloatRect ship, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms, std::list<ECE_Centipede>& Centipedes);\
	size_t size();
};