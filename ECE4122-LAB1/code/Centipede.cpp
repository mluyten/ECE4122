// Centipede.cpp : Defines the entry point for the console application.
//

// TODO:
// 2. CMAKE
// 3. Odd Race Case
// 4. Pace-ICE Compile
// 5. Comments
// 6. Odd race case
// 7. Enter from top

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

void eatMushrooms(Spider spider, std::list<Mushroom>& mushrooms)
{
	for (auto mushroom = mushrooms.begin(); mushroom != mushrooms.end(); ) 
	{
		if (mushroom->getGlobalBounds().intersects(spider.getGlobalBounds()))
		{
			mushroom = mushrooms.erase(mushroom);
			break;
		}
		else {
			++mushroom;
		}
	}
}

void blastMushrooms(int& score, std::list<ECE_LaserBlast>& blasts, std::list<Mushroom>& mushrooms, sf::Time dt)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{
		bool hit = false;
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

void blastSpider(int& score, std::list<ECE_LaserBlast>& blasts, Spider& spider, sf::Time dt)
{
	for (auto blast = blasts.begin(); blast != blasts.end(); ) 
	{	
		if (blast->getGlobalBounds().intersects(spider.getGlobalBounds())) 
		{
			std::random_device rd;  // a seed source for the random number engine
			std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(3, 9);
			score += 100 * distrib(gen);
			blast = blasts.erase(blast);
			spider.reset(4);
		}
		else
		{
			++blast;
		}
	}
}

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

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1200);

	// Create and open a window for the game

	RenderWindow window(vm, "Centipede", Style::Fullscreen);
	bool titleScreen = true;
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

	float scaleFactor = 1.5;
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
	starship.setRange(Vector2f(0, vm.height-5.5*gameGrid.size), Vector2f(vm.width, vm.height-0.5*gameGrid.size));

	std::list<ECE_LaserBlast> blasts;
	bool spacePressed = false;

	Grid mushroomGrid = gameGrid;
	mushroomGrid.rows -= 10;
	mushroomGrid.yOffset += 5 * mushroomGrid.size;
	std::list<Mushroom> mushrooms;

	std::list<ECE_Centipede> centipedes;
	centipedes.emplace_back(11, gameGrid.grid2Coord(24, 11), gameGrid, textures);

	std::list<Starship> lives;

	sf::Vector2f spiderStart(-1.0 * gameGrid.size, vm.height - 5 * gameGrid.size);
	Spider spider(spiderStart, textures["Spider.png"]);
	spider.setRange(Vector2f(-1.0 * gameGrid.size, vm.height - 10 * gameGrid.size), Vector2f(vm.width + gameGrid.size, vm.height - 0.5 * gameGrid.size));

	// Create a Text object called HUD
	Text hud;
	Font font;
	font.loadFromFile("fonts/DS-DIGI.ttf");
	hud.setFont(font);
	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(1920/2, 20);
	int score = 0;

	// Here is our clock for timing everything
	Clock clock;

	while (window.isOpen())
	{
		if (titleScreen) 
		{
			if (Keyboard::isKeyPressed(Keyboard::Enter)) 
			{
				titleScreen = false;
				generateMushrooms(30, mushrooms, mushroomGrid, textures["Mushroom0.png"], textures["Mushroom1.png"]);

				for (int i = 0; i < 3; i++)
				{
					lives.emplace_back(vm.width*(0.75+0.025*i), vm.height*0.06, textures["StarShip.png"]);
					lives.back().setScale(scaleFactor, scaleFactor);
				}
			}
			clock.restart();
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
			starship.moveLeft();
		else
			starship.stopLeft();

		if (Keyboard::isKeyPressed(Keyboard::Right))
			starship.moveRight();
		else
			starship.stopRight();

		if (Keyboard::isKeyPressed(Keyboard::Up))
			starship.moveUp();
		else
			starship.stopUp();

		if (Keyboard::isKeyPressed(Keyboard::Down))
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

		for (auto centipede = centipedes.begin(); centipede != centipedes.end(); )
		{
			float points = centipede->checkCollision(starship.getGlobalBounds(), blasts, mushrooms, centipedes);
			dead |= (points == -1);
			if (dead)
				break;
			else
				score += points;
			if (centipede->size() > 0)
			{
				centipede->update(dt);
				++centipede;
			}
			else
				centipede = centipedes.erase(centipede);

			
		}

		titleScreen = (centipedes.size() == 0);
		dead |= (spider.getGlobalBounds().intersects(starship.getGlobalBounds()));
		
		if (dead | titleScreen)
		{
			centipedes.clear();
			blasts.clear();
			lives.pop_back();
			starship.setPosition(vm.height / 2, vm.width - 20);
			spider.reset();
			centipedes.emplace_back(11, gameGrid.grid2Coord(4, 1), gameGrid, textures);
			titleScreen |= (lives.empty());
			if (titleScreen)
			{
				lives.clear();
				mushrooms.clear();
				score = 0;
				
			}
			continue;
		}

		
		eatMushrooms(spider, mushrooms);

		blastMushrooms(score, blasts, mushrooms, dt);

		blastSpider(score, blasts, spider, dt);
		
		cullBlasts(blasts); // Remove blasts that are out of frame

		spider.update(dt);
		starship.update(dt);
		for (auto& blast : blasts) blast.update(dt);

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