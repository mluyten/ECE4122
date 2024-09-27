// Pong.cpp : Defines the entry point for the console application.
//

#include "Mushroom.h"
#include "Starship.h"
#include "GameUtils.h"
#include "ECE_LaserBlast.h"
#include "ECE_Centipede.h"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1200);

	// Create and open a window for the game

	RenderWindow window(vm, "Centipede", Style::Fullscreen);

	float scaleFactor = 1.5;

	int score = 0;

	sf::Texture titleScreenTexture;
	if (!titleScreenTexture.loadFromFile("graphics/TitleScreen.png")) 
	{
		std::cout << "Failed to load graphics/TitleScreen.png\n";
	}
	sf::Texture spiderTexture;
	if(!spiderTexture.loadFromFile("graphics/Spider.png")) 
	{
		std::cout << "Failed to load graphics/Spider.png\n";
	}
	sf::Texture mushroomTexture;
	if (!mushroomTexture.loadFromFile("graphics/Mushroom0.png")) 
	{
		std::cout << "Failed to load graphics/Mushroom0.png\n";
	}
	sf::Texture damagedMushroomTexture;
	if (!damagedMushroomTexture.loadFromFile("graphics/Mushroom1.png")) 
	{
		std::cout << "Failed to load graphics/Mushroom1.png\n";
	}
	sf::Texture starshipTexture;
	if (!starshipTexture.loadFromFile("graphics/StarShip.png"))
	{
		std::cout << "Failed to load graphics/StarShip.png\n";
	}
	sf::Texture laserTexture;
	if (!laserTexture.loadFromFile("graphics/laser.png"))
	{
		std::cout << "Failed to load graphics/laser.png\n";
	}
	sf::Texture headTexture;
	if (!headTexture.loadFromFile("graphics/CentipedeHead.png"))
	{
		std::cout << "Failed to load graphics/CentipedeHead.png\n";
	}
	sf::Texture bodyTexture;
	if (!bodyTexture.loadFromFile("graphics/CentipedeBody.png"))
	{
		std::cout << "Failed to load graphics/CentipedeBody.png\n";
	}

		// Create a sprite
	sf::Sprite titleScreenSprite;

	// Attach the texture to the sprite
	titleScreenSprite.setTexture(titleScreenTexture);

	// Set the spriteBackground to cover the screen
	titleScreenSprite.setPosition(0, 0);
	titleScreenSprite.setScale(
    vm.width / titleScreenSprite.getLocalBounds().width, 
    vm.height / titleScreenSprite.getLocalBounds().height);

	Grid gameGrid(27 * scaleFactor, vm.width, vm.height);
			

	// Create a Starship
	Starship starship(vm.height / 2, vm.width - 20, starshipTexture);
	starship.setScale(scaleFactor, scaleFactor);
	starship.setRange(Vector2f(0, vm.height-5*gameGrid.size), Vector2f(vm.width, vm.height));

	std::list<ECE_LaserBlast> blasts;
	bool spacePressed = false;

	Grid mushroomGrid = gameGrid;
	mushroomGrid.rows -= 10;
	mushroomGrid.yOffset += 5 * mushroomGrid.size;
	std::list<Mushroom> mushrooms;

	std::list<ECE_Centipede> centipedes;
	centipedes.emplace_back(7, gameGrid.grid2Coord(4, 7), gameGrid, headTexture, bodyTexture);

	std::list<Starship> lives;

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
	std::vector<int> generated(30, 0);

	while (window.isOpen())
	{
		if (titleScreen) 
		{
			if (Keyboard::isKeyPressed(Keyboard::Enter)) 
			{
				titleScreen = false;
					/*
					* Generate spider
					* Generate centipede
					*/
				generateMushrooms(70, mushrooms, mushroomGrid, mushroomTexture, damagedMushroomTexture);

				for (int i = 0; i < 3; i++)
				{
					lives.emplace_back(vm.width*(0.75+0.025*i), vm.height*0.06, starshipTexture);
					lives.back().setScale(1.5,1.5);
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
				blasts.emplace_back(starship.getPosition(), laserTexture);
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
		starship.update(dt);
		
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

		for(auto& centipede : centipedes)
		{
			centipede.update(dt);
			for (auto& mushroom : mushrooms) 
			{
				centipede.checkCollision(mushroom.getGlobalBounds());
			}
		}

		
		while(true) 
		{
			if (blasts.size() > 0 && blasts.front().getPosition().y < 0) 
			{
				blasts.pop_front();
			}
			else
				break;
		}
		
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