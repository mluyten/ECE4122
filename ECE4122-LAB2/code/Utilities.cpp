#include "Utilities.h"
#include <iostream>

void age(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    int sum = 0;
    for (int i(r > 0 ? -1 : 0); i <= int(r < thisGeneration.rows()-1 ? 1 : 0); i++) {
        for (int j(c > 0 ? -1 : 0); j <= int(c < thisGeneration.cols()-1 ? 1 : 0); j++) {
            if (j != 0 || i != 0)
                sum += lastGeneration[r+i][c+j];
        }
    }

    if (lastGeneration[r][c] && (sum < 2 || sum > 3))
        thisGeneration[r][c] = 0;
    else if (!lastGeneration[r][c] && sum == 3)
        thisGeneration[r][c] = 1;
    else
        thisGeneration[r][c] = lastGeneration[r][c];
    return;
}

int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    auto tick = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < thisGeneration.rows() * thisGeneration.cols(); i++) {
        age(i / thisGeneration.cols(), i % thisGeneration.cols(), thisGeneration, lastGeneration);
    }
    auto tock = std::chrono::high_resolution_clock::now();
    lastGeneration = thisGeneration;
    
    return std::chrono::duration_cast<std::chrono::microseconds>(tock - tick).count();
}

int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    auto tick = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < thisGeneration.rows() * thisGeneration.cols(); i++) {
        age(i / thisGeneration.cols(), i % thisGeneration.cols(), thisGeneration, lastGeneration);
    }
    lastGeneration = thisGeneration;
    auto tock = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(tock - tick).count();
}

int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    auto tick = std::chrono::high_resolution_clock::now();
    #pragma openmp parallel for num_threads(nThreads) nowait
    for (size_t i = 0; i < thisGeneration.rows() * thisGeneration.cols(); i++) {
        age(i / thisGeneration.cols(), i % thisGeneration.cols(), thisGeneration, lastGeneration);
    }
    auto tock = std::chrono::high_resolution_clock::now();
    lastGeneration = thisGeneration;
    
    return std::chrono::duration_cast<std::chrono::microseconds>(tock - tick).count();
}

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
            // Ensure there is a value following -x
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
            // Ensure there is a value following -x
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
            // Ensure there is a value following -x
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