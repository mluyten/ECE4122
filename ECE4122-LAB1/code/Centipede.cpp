// Centipede.cpp : Defines the entry point for the console application.
//

#include "Mushroom.h"
#include "Starship.h"
#include "Spider.h"
#include "ECE_LaserBlast.h"
#include "ECE_Centipede.h"
#include "GameUtils.h"

#include <sstream>
#include <cstdlib>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

using namespace sf;

void cullBlasts(std::list<ECE_LaserBlast>& blasts)
{
	while(true) 
	{
		if (blasts.size() > 0 && blasts.front().getPosition().y < 0) 
		{
			blasts.pop_front();
		}
		else
			break;
	}
}

void generateMushrooms(size_t n, std::list<Mushroom>& mushrooms, Grid grid, sf::Texture& texture, sf::Texture& textureDamaged)
{
	std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, grid.rows*grid.cols-1);
	std::vector<bool> mushMap (grid.rows*grid.cols, false);
	float maxTextureDim = float(texture.getSize().x > texture.getSize().y ? texture.getSize().x : texture.getSize().y);
	float scaleFactor = grid.size / maxTextureDim;
	for (size_t i = 0; i < n; i++) 
	{
		size_t pos = distrib(gen);
		while (mushMap[pos]) 
		{
			pos = distrib(gen);
		}
		mushMap[pos] = 1;
		float posY = (floor(pos / grid.cols)) * grid.size + grid.yOffset;
		float posX = (pos % (int) grid.cols) * grid.size + grid.xOffset;
		mushrooms.emplace_back(posX, posY, texture, textureDamaged);
		mushrooms.back().setScale(scaleFactor, scaleFactor);
	}
	return;
}

void collideMushrooms(int& score, std::list<Mushroom>& mushrooms, std::list<ECE_LaserBlast>& blasts, sf::Time dt)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{
		bool hit = false;
		blast->update(dt);
		for (auto mushroom = mushrooms.begin(); mushroom != mushrooms.end(); ) 
		{
			if (mushroom->getGlobalBounds().intersects(blast->getGlobalBounds()))
			{
				if (mushroom->damaged)
				{
					mushroom = mushrooms.erase(mushroom);
					score += 4;
				}
				else
				{
					mushroom->damage();
				}
				blast = blasts.erase(blast);
				hit = true;
				
				break;
			}
			else {
				++mushroom;
			}
		}
		if (!hit)
		{
			++blast;
		}
	}
}


int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1200);

	// Create and open a window for the game

	RenderWindow window(vm, "Centipede", Style::Fullscreen);

	float scaleFactor = 1.5;

	int score = 0;

	std::map<std::string, sf::Texture> textures;

	std::vector<std::string> assets = {"TitleScreen.png", "Spider.png", "Mushroom0.png", "Mushroom1.png", "StarShip.png", "Laser.png", "CentipedeHead.png", "CentipedeBody.png"};

	for(auto& asset : assets)
	{
		textures.emplace(asset, sf::Texture());
		if (!textures[asset].loadFromFile("graphics/" + asset)) 
		{
			std::cout << "Failed to load graphics/" << asset << "\n";
		}
	}
		// Create a sprite
	sf::Sprite titleScreenSprite;

	// Attach the texture to the sprite
	titleScreenSprite.setTexture(textures["TitleScreen.png"]);

	// Set the spriteBackground to cover the screen
	titleScreenSprite.setPosition(0, 0);
	titleScreenSprite.setScale(
    vm.width / titleScreenSprite.getLocalBounds().width, 
    vm.height / titleScreenSprite.getLocalBounds().height);

	Grid gameGrid(27 * scaleFactor, vm.width, vm.height);
			

	// Create a Starship
	Starship starship(vm.height / 2, vm.width - 20, textures["StarShip.png"]);
	starship.setScale(scaleFactor, scaleFactor);
	starship.setRange(Vector2f(0, vm.height-5*gameGrid.size), Vector2f(vm.width, vm.height));

	std::list<ECE_LaserBlast> blasts;
	bool spacePressed = false;

	Grid mushroomGrid = gameGrid;
	mushroomGrid.rows -= 10;
	mushroomGrid.yOffset += 5 * mushroomGrid.size;
	std::list<Mushroom> mushrooms;

	std::list<ECE_Centipede> centipedes;

	std::list<Starship> lives;

	Spider spider(-gameGrid.size, vm.height-5*gameGrid.size, textures["Spider.png"]);
	spider.setRange(Vector2f(-gameGrid.size, vm.height-10*gameGrid.size), Vector2f(vm.width+gameGrid.size, vm.height+gameGrid.size/2));

	// Create a Text object called HUD
	Text hud;

	// A cool retro-style font
	Font font;
	font.loadFromFile("fonts/DS-DIGI.ttf");

	// Set the font to our retro-style
	hud.setFont(font);

	// Make it nice and big
	hud.setCharacterSize(75);

	// Choose a color
	hud.setFillColor(Color::White);

	hud.setPosition(1920/2, 20);

	// Here is our clock for timing everything
	Clock clock;

	bool titleScreen = true;

	while (window.isOpen())
	{
		if (titleScreen) 
		{
			if (Keyboard::isKeyPressed(Keyboard::Enter)) 
			{
				titleScreen = false;
					/*
					* Generate spider
					*/
				centipedes.emplace_back(7, gameGrid.grid2Coord(4, 7), gameGrid, textures);
				generateMushrooms(30, mushrooms, mushroomGrid, textures["Mushroom0.png"], textures["Mushroom1.png"]);

				for (int i = 0; i < 3; i++)
				{
					lives.emplace_back(vm.width*(0.75+0.025*i), vm.height*0.06, textures["StarShip.png"]);
					lives.back().setScale(scaleFactor, scaleFactor);
				}
			}
			window.clear();
			window.draw(titleScreenSprite);
			window.display();
			continue;
		}

		/*
		Handle the player input
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				// Quit the game when the window is closed
				window.close();

		}
		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (!spacePressed) {
				blasts.emplace_back(starship.getPosition(), textures["Laser.png"]);
				blasts.back().setScale(scaleFactor, scaleFactor);
				spacePressed = true;
			}
		}
		else {
			spacePressed = false;
		}

		// Handle the pressing and releasing of the arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			starship.moveLeft();
		}
		else
		{
			starship.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			starship.moveRight();
		}
		else
		{
			starship.stopRight();
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			starship.moveUp();
		}
		else
		{
			starship.stopUp();
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			starship.moveDown();
		}
		else
		{
			starship.stopDown();
		}
		/*
		Update the Starship, the ECE_LaserBlast and the HUD
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		// Update the delta time
		sf::Time dt = clock.restart();

		for (auto& centipede : centipedes)
		{
			float points = centipede.checkCollision(starship.getGlobalBounds(), blasts, mushrooms, centipedes);
			if (points == -1)
			{
				centipedes.clear();
				blasts.clear();
				lives.pop_back();
				starship.setPosition(vm.height / 2, vm.width - 20);

				if (lives.size() == 0)
				{
					mushrooms.clear();
					score = 0;
					titleScreen = true;
				}
				else
				{
					centipedes.emplace_back(7, gameGrid.grid2Coord(4, 7), gameGrid, textures);
				}
				break;
			}
			else
			{
				score += points;
			}

			centipede.update(dt);
		}
		if (titleScreen)
		{
			continue;
		}
		
		collideMushrooms(score, mushrooms, blasts, dt);
		
		cullBlasts(blasts); // Remove blasts that are out of frame

		spider.update(dt);
		starship.update(dt);

		// Update the HUD text
		std::stringstream ss;
		ss << score;
		hud.setString(ss.str()); 

		/*
		Draw the Starship, the ECE_LaserBlast and the HUD
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		window.clear(); 
		window.draw(hud);
		window.draw(starship);
		window.draw(spider);
		
		for (auto& blast : blasts) 
		{
			window.draw(blast);
		}
		for (auto& life : lives) 
		{
			window.draw(life);
		}
		for (auto& mushroom : mushrooms)
		{
			window.draw(mushroom);
		}
		for (auto& centipde : centipedes)
		{
			centipde.draw(window);
		}
		window.display();
	}

	return 0;
}