/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified:

Description:

*/

#include <string>
#include <map>
#include <iostream>
#include "Utilities.h"
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[]) {
	std::map<std::string, size_t> args = parseArgs(argc, argv);
	if (args["error"] == 1)
		return 1;

	// Create a video mode object
	sf::VideoMode vm(args["windowWidth"], args["windowHeight"]);

	// Create and open a window for the game
	sf::RenderWindow window(vm, "Game of Life", sf::Style::Titlebar);

	// Here is our clock for timing everything
	sf::Clock clock;

	// Game loop
	while (window.isOpen())
	{
		// Handle Input

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
		
		// Update automata

		sf::Time dt = clock.restart(); 
		
		// Draw automata

		window.clear();

		window.display();
	}

	return 0;
}