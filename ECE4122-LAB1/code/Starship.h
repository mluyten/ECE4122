#pragma once
#include <SFML/Graphics.hpp>

class Starship : public sf::Sprite
{
private:
	float speed_ = 750.0f;

	bool movingRight_ = false;
	bool movingLeft_ = false;
	bool movingUp_ = false;
	bool movingDown_ = false;

	sf::Vector2f minRange_;
	sf::Vector2f maxRange_;

public:
	Starship(float startX, float startY, sf::Texture& texture);

	Starship(sf::Vector2f start, sf::Texture& texture);

	void setRange(sf::Vector2f minRange, sf::Vector2f maxRange);

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	void update(sf::Time dt);

};
