/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This is the header for my utility functions/structs.
*/

#pragma once
#include <map>
#include <string>
#include <iostream>

// Parses command line arguments
std::map<std::string, size_t> parseArgs(int argc, char* argv[]);