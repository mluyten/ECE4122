/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified:

Description:

*/

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Utilities.h"
#include "Matrix.h"
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[]) {
	std::map<std::string, size_t> args = parseArgs(argc, argv);
	if (args["error"] == 1)
		return 1;

	const size_t rows = args["windowHeight"] / args["cellSize"];
	const size_t cols = args["windowWidth"] / args["cellSize"];

	Matrix<int> thisGeneration(rows, cols);
	Matrix<int> lastGeneration(rows, cols);

	std::vector<std::vector<sf::RectangleShape>> cells(rows, std::vector<sf::RectangleShape>(cols));
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			cells[r][c].setSize(sf::Vector2f(args["cellSize"], args["cellSize"]));
			cells[r][c].setFillColor(sf::Color(255,255,255,255));
			cells[r][c].setPosition(c*args["cellSize"], r*args["cellSize"]);
		}
	}

	// Spawn a glider
	lastGeneration[0][1] = 1;
	lastGeneration[1][2] = 1;
	lastGeneration[2][0] = 1;
	lastGeneration[2][1] = 1;
	lastGeneration[2][2] = 1;

	float time = 0;

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

		if (time > 0.05) {
			for (size_t r = 0; r < rows; r++) {
				for (size_t c = 0; c < cols; c++) {
					age(r, c, thisGeneration, lastGeneration);
				}
			}
			time = 0;
			lastGeneration = thisGeneration;
		}
		else {
			time += dt.asSeconds();
		}
		
		// Draw automata
		window.clear();

		for (size_t r = 0; r < rows; r++) {
			for (size_t c = 0; c < cols; c++) {
				if (lastGeneration[r][c] == 1)
					window.draw(cells[r][c]);
			}
		}

		window.display();
	}

	return 0;
}