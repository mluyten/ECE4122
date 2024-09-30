/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 9/30/2024

Description:
This is the main processing loop for my centipede game. All gameplay and graphics handling happens here.
This is derrived from the Pong game example.
*/

// TODO:
// 1. There is a case where the centipede siezes up and the rear segments get blasted into oblivion. I do not have time to fix this bug.
// 2. Have centipede enter from the top. The centipede enters from the top of the screen, but it does not slink down like the centipede
//	  from the original atari game. I dont have time for this.

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
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

int main()
{
	// Create a video mode object
	sf::VideoMode vm(1280, 720);

	// Create and open a window for the game
	sf::RenderWindow window(vm, "Centipede", sf::Style::Titlebar);

	// Display splash screen and do not start until enter is pressed
	bool titleScreen = true;

	// Load all textures into a map
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

	// Set scale factor for sprites. 1.1 works well
	float scaleFactor = 1.1;
	// Create a sprite
	sf::Sprite titleScreenSprite;
	// Attach the texture to the sprite
	titleScreenSprite.setTexture(textures["TitleScreen.png"]);
	// Set the spriteBackground to cover the screen
	titleScreenSprite.setPosition(0, 0);
	// Scale splash screen to window size
	titleScreenSprite.setScale(
    vm.width / titleScreenSprite.getLocalBounds().width, 
    vm.height / titleScreenSprite.getLocalBounds().height);

	// Set up game grid according to parameters window and scale parameters
	Grid gameGrid(27 * scaleFactor, vm.width, vm.height);
			

	// Create a Starship
	Starship starship(vm.height / 2, vm.width - 20, textures["StarShip.png"]);
	starship.setScale(scaleFactor, scaleFactor);
	starship.setRange(sf::Vector2f(0, vm.height-5*gameGrid.size), sf::Vector2f(vm.width, vm.height-0.5*gameGrid.size));

	// Create blasts list
	std::list<ECE_LaserBlast> blasts;
	bool spacePressed = false;

	// Create mushrooms and place them in smaller 
	Grid mushroomGrid = gameGrid;
	mushroomGrid.rows -= 10;
	mushroomGrid.yOffset += 5 * mushroomGrid.size;
	std::list<Mushroom> mushrooms;

	// Creeate list of centipedes and add our first one at the top of the play area.
	std::list<ECE_Centipede> centipedes;
	centipedes.emplace_back(11, gameGrid.grid2Coord(4, 1), gameGrid, textures);

	// Create lives vector. This just displays the sprites and keeps track of the number of lives remaining
	std::list<Starship> lives;

	// Create the spider and place it just out of view
	sf::Vector2f spiderStart(-1.0 * gameGrid.size, vm.height - 5 * gameGrid.size);
	Spider spider(spiderStart, textures["Spider.png"]);
	spider.setRange(sf::Vector2f(-1.0 * gameGrid.size, vm.height - 10 * gameGrid.size), sf::Vector2f(vm.width + gameGrid.size, vm.height - 0.5 * gameGrid.size));

	// Create a Text object called HUD - this displays the score
	sf::Text hud;
	sf::Font font;
	font.loadFromFile("fonts/DS-DIGI.ttf");
	hud.setFont(font);
	hud.setCharacterSize(55);
	hud.setFillColor(sf::Color::White);
	hud.setPosition(vm.width/2, 20);
	int score = 0;

	// Here is our clock for timing everything
	sf::Clock clock;

	// Game loop
	while (window.isOpen())
	{
		// Display splash screen and do not start until enter is pressed
		if (titleScreen) 
		{
			// Check if enter pressed to start game
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) 
			{
				// Toggle flag and generate 30 mushrooms in game area
				titleScreen = false;
				generateMushrooms(30, mushrooms, mushroomGrid, textures["Mushroom0.png"], textures["Mushroom1.png"]);

				// Place three "starships" in lives vector
				for (int i = 0; i < 3; i++)
				{
					lives.emplace_back(vm.width*(0.75+0.025*i), vm.height*0.06, textures["StarShip.png"]);
					lives.back().setScale(scaleFactor, scaleFactor);
				}
			}
			// Handle the player quitting
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}

			clock.restart();
			window.clear();
			window.draw(titleScreenSprite); // Draw splash screen
			window.display();
			continue;
		}

		/*
		Handle the player input
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				// Quit the game when the window is closed
				window.close();

		}
		// Handle the player quitting
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		
		// Handle the player quitting
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
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

		// Handle the pressing and releasing of the arrow keys to move starship
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			starship.moveLeft();
		else
			starship.stopLeft();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			starship.moveRight();
		else
			starship.stopRight();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			starship.moveUp();
		else
			starship.stopUp();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			starship.moveDown();
		else
			starship.stopDown();
		/*
		Update the Starship, the ECE_LaserBlast and the HUD
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		// Update the delta time
		sf::Time dt = clock.restart();

		bool dead = false;

		// Check collisions with mushrooms, blasts, and the starship for all centipedes
		for (auto centipede = centipedes.begin(); centipede != centipedes.end(); )
		{
			// Returns the score accumulated if blast hits centipede. If -1, the centipede hit the starship and a life is lost
			float points = centipede->checkCollision(starship.getGlobalBounds(), blasts, mushrooms, centipedes);
			dead |= (points == -1);
			if (dead) 
				break; // We're hit, exit the loop
			else
				score += points;
			if (centipede->size() > 0) // Remove any empty or "dead" centipedes
			{
				centipede->update(dt);
				++centipede;
			}
			else
				centipede = centipedes.erase(centipede);

			
		}

		// Restart game if every centipede is dead
		titleScreen = (centipedes.size() == 0);
		// Lose a life if spider hits starship
		dead |= (spider.getGlobalBounds().intersects(starship.getGlobalBounds()));
		
		if (dead | titleScreen)
		{
			// If dead or restarting, we need ot clear centipedes, blasts, and remove a life
			centipedes.clear();
			blasts.clear();
			lives.pop_back();
			starship.setPosition(vm.height / 2, vm.width - 20);
			spider.reset();
			centipedes.emplace_back(11, gameGrid.grid2Coord(4, 1), gameGrid, textures);

			// If we last life is gone, restart game
			titleScreen |= (lives.empty());
			if (titleScreen)
			{
				// Reset game
				lives.clear();
				mushrooms.clear();
				score = 0;
				
			}
			continue;
		}

		// Removes mushrooms intersecting the spider
		spider.eatMushrooms(mushrooms);

		// Checks blast/mushroom collisions and changes mushrooms as needed. Updates score
		blastMushrooms(score, blasts, mushrooms, dt);

		// Checks if spider is hit and "kills" is if so. Updates score
		blastSpider(score, blasts, spider, dt);
		
		cullBlasts(blasts); // Remove blasts that are out of frame

		// Update our moving sprites
		spider.update(dt);
		starship.update(dt);
		for (auto& blast : blasts) blast.update(dt);

		// Update the HUD text
		std::stringstream ss;
		ss << score;
		hud.setString(ss.str()); 

		/*
		Draw the all of our sprites + the hud
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