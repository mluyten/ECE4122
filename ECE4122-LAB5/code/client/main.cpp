/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/16/2024

Description: This is the robot UDP client. Use --port <port> to set the server port to connect to.
*/

#include <iostream>

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

	// Connect to local server w/ requested port
    sf::IpAddress server = "127.0.0.1";

    // Create a socket for communicating with the server
    sf::UdpSocket socket;
	socket.setBlocking(false);

	// Send connect command to server
	char command = 'c';
	if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done) {
		std::cout << "Unable to connect to server. Quitting...\n";
		return -1;
	}

	std::size_t received = 0;
    sf::IpAddress sender;
    uint16_t senderPort = 0;

	bool keyPressed = false;

	// Command loop
	while (true)
	{
		sf::Event event;

		// Check if server sent us quiting command
		if (socket.receive(&command, sizeof(char), received, sender, senderPort) == sf::Socket::Status::Done) {
			if (command == 'q') break; // Exit program if server is shutting down
			else std::cout << "Unknown command from server" << std::endl;
		}
		// Handle quitting
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			command = 'q';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
			break;
		}
		// Handle the pressing and releasing of WASD-GH keys to move Robot
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			// Left
			if (!keyPressed) {
				// Send command to the server
				command = 'a';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			// Right
			if (!keyPressed) {
				// Send command to the server
				command = 'd';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			// Up
			if (!keyPressed) {
				// Send command to the server
				command = 'w';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			// Down
			if (!keyPressed) {
				// Send command to the server
				command = 's';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
			// Speed up
			if (!keyPressed) {
				// Send command to the server
				command = 'g';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			// Slow down
			if (!keyPressed) {
				// Send command to the server
				command = 'h';
				if (socket.send(&command, sizeof(char), server, port) != sf::Socket::Status::Done)
					std::cout << "Failed to send message to server. Press " << std::endl;
				keyPressed = true;
			}
		}
		else {
			keyPressed = false;
		}
	}

	return 0;
}