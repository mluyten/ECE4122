/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This is the header for my utility functions.
*/

#pragma once
#include <map>
#include <string>
#include <iostream>
#include <atomic>
#include <thread>

struct Worker {
    Worker() {
        idle = true;
        done = true;
    }
    std::atomic_bool idle;
    std::atomic_bool done;
    std::thread* thread;
};

std::map<std::string, size_t> parseArgs(int argc, char* argv[]);