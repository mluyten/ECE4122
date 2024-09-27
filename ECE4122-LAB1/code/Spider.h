#pragma once
#include <SFML/Graphics.hpp>

class Spider : public sf::Sprite
{
private:
	float speed_ = 750.0f;

	sf::Vector2f dir_; // Unit vector for direction

	sf::Vector2f minRange_;
	sf::Vector2f maxRange_;

	int state = 0; // 0 - Offscreen, 1 - moving right, 2 - moving left, 3 - moving up, 4 - moving down 

public:
	Spider(float startX, float startY, sf::Texture& texture);

	Spider(sf::Vector2f start, sf::Texture& texture);

	void update(sf::Time dt);

};
