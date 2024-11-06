/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/6/2024

Description:
This is the main loop for the GameOfLife cellular automata project.
Each generation of celular automata is drawn and displayed.
Every 100 generations, the processing time in us is printed.
Arguments:
	-n <number of threads>
	-c <size of each cell in pixels>
	-x <window width in pixels>
	-y <window height in pixels>
	-t <memory mode>
		NOTE: Memory modes are NORMAL, PINNED, and MANAGED. Any other mode will result in an error
*/

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <random>
#include "CellularAutomata.cuh"
#include "Utilities.h"
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[]) {
	// Parse arguments and stick 'em in a map
	std::map<std::string, size_t> args = parseArgs(argc, argv);
	// Quit on error
	if (args["error"] == 1)
		return 1;

	// Map of modes to printout string
	std::map<size_t, std::string> modes = { {0, std::to_string(args["nThreads"]) + " threads per block using Normal memory allocation."},
										   {1, std::to_string(args["nThreads"]) + " threads per block using Pinned memory allocation."},
										   {2, std::to_string(args["nThreads"]) + " threads per block using Managed memory allocation."} };

	// Get number of rows and cols
	const size_t rows = args["windowHeight"] / args["cellSize"];
	const size_t cols = args["windowWidth"] / args["cellSize"];

	// Set up matrices to store cell states
	LifeManager mgr(rows, cols, args["memoryMode"]);

	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 1);

	// Set up vector of SFML rectangles to display cells and randomly determine cell state
	std::vector<std::vector<sf::RectangleShape>> cells(rows, std::vector<sf::RectangleShape>(cols));
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			cells[r][c].setSize(sf::Vector2f((float) args["cellSize"], (float) args["cellSize"]));
			cells[r][c].setFillColor(sf::Color(255, 255, 255, 255));
			cells[r][c].setPosition(c * args["cellSize"], r * args["cellSize"]);
			mgr.set(r, c, distrib(gen));
		}
	}
	size_t generations = 0;
	int procTime = 0;
	mgr.init();

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

		// Update automata based on memory mode
		procTime += mgr.nextGeneration(args["nThreads"]);
		generations++;

		// Print time took for the last 100 generations
		if (generations % 100 == 0) {
			std::cout << "100 generations took " << procTime << " microseconds with " << modes[args["memoryMode"]] << std::endl;
			procTime = 0;
		}

		// Draw window
		window.clear();
		// Draw every live cell
		for (size_t r = 0; r < rows; r++) {
			for (size_t c = 0; c < cols; c++) {
				if (mgr.get(r, c) == 1)
					window.draw(cells[r][c]);
			}
		}
		window.display();
	}

	return 0;
}