/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/16/2024

Description: This is the robot UDP server. Listens for commands on port set with -p/--port <port>.
Adds a new robot for every new connection and controls it with command sent from the client.
*/

#include "Robot.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

int main(int argc, char* argv[])
{
	uint16_t port = 0;
	if (argc == 3) {
		if (std::string(argv[1]) == "-p" || std::string(argv[1]) == "--port") {
			try {
				// Convert the next argument to an integer
				port = std::stoi(argv[2]);
			} 
			catch (const std::invalid_argument& e) {
				std::cerr << "Error: Invalid value for -p/--port: " << argv[2] << std::endl;
				return -1;
			}
        }
		else {
			std::cerr << "Error: Unknown argument. Please specify port with -p/--port" << std::endl;
			return -1;
		}
	}
	else {
		std::cerr << "Error: Not enough arguments. Please specify port with -p/--port" << std::endl;
			return -1;
	}

	// Create a socket to receive a message from anyone
    sf::UdpSocket socket;
	socket.setBlocking(false);

    // Listen to messages on the specified port
    if (socket.bind(port) != sf::Socket::Status::Done)
        return -1;
    std::cout << "Server is listening on port " << port << std::endl;

    std::size_t received = 0;
    sf::IpAddress sender;
    uint16_t senderPort = 0;

	// Create a video mode object
	sf::VideoMode vm(800, 600);
	sf::Vector2f windowDims(vm.width, vm.height);
	sf::Vector2f center(vm.width / 2, vm.height / 2);

	// Create and open a window for the game
	sf::RenderWindow window(vm, "Robot", sf::Style::Titlebar);

	// Load in robot texture
	sf::Texture robotTexture;
	if (!robotTexture.loadFromFile("graphics/robot.png")) 
	{
		std::cout << "Failed to load graphics/robot.png\n";
	}

	std::map<std::pair<uint32_t, uint16_t>, Robot> bots;

	// Here is our clock for timing everything
	sf::Clock clock;

	bool keyPressed = false;

	// Game loop
	while (window.isOpen())
	{
		/*
		*********************************************************************
		Handle the player input
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
		
		char command;
		if (socket.receive(&command, sizeof(char), received, sender, senderPort) == sf::Socket::Status::Done) {
			std::pair<uint32_t, int16_t> senderID(sender.toInteger(), senderPort);
			switch(command) {
				case 'c': // c-onnect
					// Client connect, create a robot to control
					// Keep bot in map w/ pair of client IP + port
					bots.emplace(std::piecewise_construct,
							std::forward_as_tuple(senderID),
							std::forward_as_tuple(center, robotTexture, windowDims));
					break;
				case 'q':
					// Quit, delete client robot
					bots.erase(senderID);
					break;
				case 'a':
					// Move left, change robot direction to -x
					bots[senderID].changeDirection(sf::Vector2f(-1, 0));
					break;
				case 'd':
					// Move right, change robot direction to +x
					bots[senderID].changeDirection(sf::Vector2f(1, 0));
					break;
				case 'w':
					// Move up, change robot direction to -y
					bots[senderID].changeDirection(sf::Vector2f(0, -1));
					break;
				case 's':
					// Move down, change robot direction to +y
					bots[senderID].changeDirection(sf::Vector2f(0, 1));
					break;
				case 'g':
					// Speed up, increment speed by 10 pixels/s
					bots[senderID].incrementSpeed(10);
					break;
				case 'h':
					// Slow down, decrement speed by 10 pixels/s
					bots[senderID].incrementSpeed(-10);
					break;
				default:
					break;
			}
		}
		/*
		*********************************************************************
		Update the bots
		*********************************************************************
		*/
	
		// Update the delta time
		sf::Time dt = clock.restart();

		// Update position of all bots
		for (auto& bot : bots) bot.second.update(dt);

		/*
		*********************************************************************
		Draw all bots
		*********************************************************************
		*/
		window.clear(); 

		for (auto& bot : bots) window.draw(bot.second);

		window.display();
	}

	// If shutting down, alert all clients that shutdown is happening
	for (auto& bot : bots) {
		// Get client IP
		sf::IpAddress client(bot.first.first);
		// Send 'q' command
		char msg = 'q';
		// Fire off message over UDP
		if (socket.send(&msg, sizeof(char), client, bot.first.second) != sf::Socket::Status::Done)
			std::cout << "Failed to send message to client." << std::endl;
	}

	return 0;
}