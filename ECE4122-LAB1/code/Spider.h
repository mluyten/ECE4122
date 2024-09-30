/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the header for the Spider class.
*/

#pragma once
#include <random>
#include <queue>
#include <list>
#include <utility>
#include "Mushroom.h"
#include "GameUtils.h"
#include <SFML/Graphics.hpp>

class Spider : public sf::Sprite
{
private:
	float speed_ = 350.0f;

	sf::Vector2f minRange_;
	sf::Vector2f maxRange_;

	float time_;
	std::queue<std::pair<sf::Vector2f, float>> commands_;

	float xDir_;
	int state_; 
	int nextState_;
	sf::Vector2f initialPos_;
public:
	Spider(float startX, float startY, sf::Texture& texture);
	Spider(sf::Vector2f start, sf::Texture& texture);

	void setRange(sf::Vector2f minRange, sf::Vector2f maxRange);

	void update(sf::Time dt);
	void reset(float timeout=0);

	void eatMushrooms(std::list<Mushroom>& mushrooms);
};
