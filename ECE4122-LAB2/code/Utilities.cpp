/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This implements utility function. In this case, it only implements
the argument parser function.
*/

#include "Utilities.h"


// This argument parses all of the desired arguments and stores
// their values in a std::map
// -n <val> is stored at map["nThreads"]
// -c <val> is stored at map["cellSize"]
// -x <val> is stored at map["windowWidth"]
// -y <val> is stored at map["windowHeight"]
// -t <val> is stored at map["threadingMode"]
// NOTE: Threading mode is stored as an int value:
//      0: SEQ - Sequential (Default)
//      1: THRD - Parallel w/ std::thread
//      2: OMP - Parallel w/ OpenMP
std::map<std::string, size_t> parseArgs(int argc, char* argv[])
{
    std::map<std::string, size_t> args = {
        {"error", 0},
		{"nThreads", 8},
		{"cellSize", 5},
		{"windowWidth", 800},
		{"windowHeight", 600},
		{"threadingMode", 0}
	};

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

		if (arg == "-n") {
            // Ensure there is a value following -n
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    size_t nThreads = std::stoi(argv[i + 1]);
                    args["nThreads"] = nThreads;
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -n: " << argv[i + 1] << std::endl;
                    args["error"] = 1;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -n" << std::endl;
				args["error"] = 1;
            }
        }
		else if (arg == "-c") {
            // Ensure there is a value following -c
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    size_t cellSize = std::stoi(argv[i + 1]);
                    args["cellSize"] = cellSize;
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -c: " << argv[i + 1] << std::endl;
					args["error"] = 1;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -c" << std::endl;
				args["error"] = 1;
            }
        }
        else if (arg == "-x") {
            // Ensure there is a value following -x
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    size_t windowWidth = std::stoi(argv[i + 1]);
                    args["windowWidth"] = windowWidth;
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -x: " << argv[i + 1] << std::endl;
					args["error"] = 1;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -x" << std::endl;
				args["error"] = 1;
            }
        }
		else if (arg == "-y") {
            // Ensure there is a value following -y
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    size_t windowHeight = std::stoi(argv[i + 1]);
                    args["windowHeight"] = windowHeight;
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -y: " << argv[i + 1] << std::endl;
					args["error"] = 1;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -y" << std::endl;
				args["error"] = 1;
            }
        }
		else if (arg == "-t") {
            // Ensure there is a value following -t
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    if (std::string(argv[i + 1]) == "SEQ") {
                        args["threadingMode"] = 0;
                    }
                    else if (std::string(argv[i + 1]) == "THRD") {
                        args["threadingMode"] = 1;
                    }
                    else if (std::string(argv[i + 1]) == "OMP") {
                        args["threadingMode"] = 2;
                    }
                    else {
                        throw std::invalid_argument("");
                    }
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -t (Must be OMP, THRD, or SEQ): " << argv[i + 1] << std::endl;
					args["error"] = 1;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -t" << std::endl;
				args["error"] = 1;
            }
        }
		else {
			std::cerr << "Error: Unknown argument " << arg[i] << std::endl;
            args["error"] = 1;
		}
    }
    return args;
}