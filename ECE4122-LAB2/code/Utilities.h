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
#include <atomic>
#include <thread>

// Thread worker struct. This bundles some atomic boolean states that allow for
// some primitive thread pooling.
struct Worker {
    Worker() {
        // Starts thread with state that allows it to be launched ASAP
        idle = true;
        done = true;
    }
    // Thread is finished and ready for tasking
    std::atomic_bool idle;
    // Thread finished execution but may not be ready for tasking
    std::atomic_bool done;
    // Thread ptr. Should be set to new thread object whenever new process is kicked off
    std::thread* thread;
};

// Parses command line arguments
std::map<std::string, size_t> parseArgs(int argc, char* argv[]);