#include "Mushroom.h"

// This the constructor function
Mushroom::Mushroom(float startX, float startY, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	setPosition(startX, startY);
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

Mushroom::Mushroom(sf::Vector2f start, sf::Texture& textureFull, sf::Texture& textureDamaged)
{
	setTexture(textureFull);
	setOrigin(textureFull.getSize().x/2, textureFull.getSize().y/2);
	setPosition(start);
	damaged = false;
	textureDamaged_ = &textureDamaged;
}

void Mushroom::damage()
{
	setTexture(*textureDamaged_);
	damaged = true;
}