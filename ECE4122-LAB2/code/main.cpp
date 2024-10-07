/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:

*/

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <random>
#include "CellularAutomata.h"
#include "Utilities.h"
#include "Matrix.h"
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[]) {
	std::map<std::string, size_t> args = parseArgs(argc, argv);
	if (args["error"] == 1)
		return 1;

	std::map<size_t, std::string> modes = {{0, "single thread."}, 
										   {1, std::to_string(args["nThreads"]) + " std::threads."},
										   {2, std::to_string(args["nThreads"]) + " OMP threads."}};

	const size_t rows = args["windowHeight"] / args["cellSize"];
	const size_t cols = args["windowWidth"] / args["cellSize"];

	Matrix<int> thisGeneration(rows, cols);
	Matrix<int> lastGeneration(rows, cols);

	std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 1);

	std::vector<std::vector<sf::RectangleShape>> cells(rows, std::vector<sf::RectangleShape>(cols));
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++) {
			cells[r][c].setSize(sf::Vector2f(args["cellSize"], args["cellSize"]));
			cells[r][c].setFillColor(sf::Color(255,255,255,255));
			cells[r][c].setPosition(c*args["cellSize"], r*args["cellSize"]);
			lastGeneration[r][c] = distrib(gen);
		}
	}
	size_t generations = 0;
	int procTime = 0;

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
		
		// Update automata based on threading mode
		if (args["threadingMode"] == 0) {
			procTime += nextGenerationSeq(thisGeneration, lastGeneration);
		}
		else if (args["threadingMode"] == 1) {
			procTime += nextGenerationThrd(thisGeneration, lastGeneration, args["nThreads"]);
		}
		else {
			procTime += nextGenerationOMP(thisGeneration, lastGeneration, args["nThreads"]);
		}
		generations++;

		// Print time took for the last 100 generations
		if (generations % 100 == 0) {
			std::cout << "100 generations took " << procTime << " microseconds with " << modes[args["threadingMode"]] << std::endl;
			procTime = 0;
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