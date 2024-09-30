/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
	This is the header for the ECE_Centipede class and Segment class. Segment should probably be in a private namespace or part of
	ECE_Centipede's private scope. Oh bother.
*/

#pragma once
#include <list>
#include <iterator>
#include <map>
#include <iostream>
#include "GameUtils.h"
#include "ECE_LaserBlast.h"
#include "Mushroom.h"
#include <SFML/Graphics.hpp>

// This is the segment class. It is a sprite that makes up the segments of the
// centipede. It manages orientation, position, and collisions
class Segment : public sf::Sprite
{
private:
	// Vertical direction of centipede
	bool descending_ = true;

	float speed_ = 300.0f;
	Grid gameGrid_;
	int nextY_;
public:
	Segment(float startX, float startY, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);
	Segment(sf::Vector2f start, Grid gameGrid, sf::Texture& texture, sf::Vector2f dir);

	// Checks collisions
	void collide();
	// Rotates texture based on direction
	void rotate();
	// Updates postion based on time
	void update(sf::Time dt);
	// Reverses direction of travel
	void reverse();
	// Sets a segment to be a head
	void setHead(sf::Texture& texture);
	bool isHead = false;

	// Current direction unit vector
	sf::Vector2f dir;
	// Last direction unit vector
	sf::Vector2f lastDir;
};

// The ECE_Centipede class keeps track of a list of Segments, manages colisions, updates, and draws the centipede
class ECE_Centipede 
{
private:
	// Map of textures
	std::map<std::string,sf::Texture>* textures_;
	// List of segments
	std::list<Segment> segments_;
	Grid gameGrid_;
public:
	ECE_Centipede(size_t size, sf::Vector2f headPosition, Grid gameGrid, std::map<std::string,sf::Texture>& textures);
	ECE_Centipede(std::list<Segment>& segments, Grid gameGrid, std::map<std::string,sf::Texture>& textures);

	// Updates position of all segments
	void update(sf::Time dt);
	// Draws centipede
	void draw(sf::RenderWindow& window);
	// Checks collision between segments, starship, mushrooms, and lasers
	float checkCollision(sf::FloatRect ship, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms, std::list<ECE_Centipede>& Centipedes);
	// Gets the size of the list of segments
	size_t size();
};