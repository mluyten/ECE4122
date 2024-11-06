/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/6/2024

Description:
This implements the LifeManager class and all cellular automata functions for use on the GPU.
*/

#include "CellularAutomata.cuh"
#include <cuda_runtime.h>
#include <iostream>
#include <chrono>

// Checks adjacent cells to cell at lastGeneration[r][c] and sets its value at thisGeneration[r][c]
// This function uses the cuda thread id to determine its position (row, col)
// thisGeneration - pointer to most recent generation of automata - this is set by this function
// lastGeneration - pointer to last generation of automata - this is read by the function and determines nextGeneration
// rows - number of rows in the simulation
// cols - number of cols in the simulation
__global__ void ageCell (int* thisGeneration, const int* lastGeneration, int rows, int cols) {
    // Get cuda thread index
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    // Make sure we're not out of bounds
    if (i < rows * cols) {
        // Determine (row, col) postion
        int r = i / cols;
        int c = i - r * cols;
        int sum = 0;
        // Iterates through all cells adjacent to desired position and sums number of live cells
        for (int i(r > 0 ? -1 : 0); i <= int(r < rows - 1 ? 1 : 0); i++) {
            for (int j(c > 0 ? -1 : 0); j <= int(c < cols - 1 ? 1 : 0); j++) {
                // Ignores desired cell
                if (j != 0 || i != 0)
                    sum += lastGeneration[(r + i) * cols + c + j];
            }
        }

        // Game of life logic
        if (lastGeneration[r * cols + c] && (sum < 2 || sum > 3))
            thisGeneration[r * cols + c] = 0;
        else if (!lastGeneration[r * cols + c] && sum == 3)
            thisGeneration[r * cols + c] = 1;
        else
            thisGeneration[r * cols + c] = lastGeneration[r * cols + c];
    }
    return;
}

// This function progresses the simulation by one iteration. It offloads that computation to the GPU
// nThreads - number of threads per block to be used on the GPU
int LifeManager::nextGeneration(int nThreads) {
    // Start wall timer
    auto tick = std::chrono::high_resolution_clock::now();
    // Get total size of array
    size_t size = _rows * _cols * sizeof(int);

    // Set up thread blocks based on nThreads value
    int threadsPerBlock = nThreads;
    int blocksPerGrid = (_rows * _cols + threadsPerBlock - 1) / threadsPerBlock;

    // Make sure any async coppies are done
    cudaDeviceSynchronize();
    
    // Run ageCell on the GPU
    ageCell << <blocksPerGrid, threadsPerBlock >> > (_d_thisGeneration, _d_lastGeneration, _rows, _cols);

    // Make sure all computations are finished
    cudaDeviceSynchronize();
    
    if (_mode != 2) {
        // If not using managed memory, async copy next generation for next iteration and copy result to host
        cudaMemcpyAsync(_d_lastGeneration, _d_thisGeneration, size, cudaMemcpyDeviceToDevice);
        cudaMemcpy(_thisGeneration, _d_thisGeneration, size, cudaMemcpyDeviceToHost);
    }
    else {
        // Copy result to last generation for next iteration
        cudaMemcpy(_d_lastGeneration, _d_thisGeneration, size, cudaMemcpyDeviceToDevice);
    }

    // Stop wall timer
    auto tock = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(tock - tick).count();
}

// Constructor for LifeManager class. Allocates memory based on GPU memory mode.
// rows - number of rows in simulation
// cols - number of columns in simulation
// mode - memory mode for the simulation (0 - NORMAL, 1 - PINNED, 2 - MANAGED)
LifeManager::LifeManager(int rows, int cols, int mode) : _rows(rows), _cols(cols), _mode(mode) {
    if (_mode == 0) {
        // Alloc buffers on host
        _thisGeneration = new int[rows * cols];
        _lastGeneration = new int[rows * cols];
        // Alloc buffers on device
        cudaMalloc(&_d_thisGeneration, rows * cols * sizeof(int));
        cudaMalloc(&_d_lastGeneration, rows * cols * sizeof(int));
    }
    else if (_mode == 1) {
        // Alloc pinned buffers on host
        cudaMallocHost(&_thisGeneration, rows * cols * sizeof(int));
        cudaMallocHost(&_lastGeneration, rows * cols * sizeof(int));
        // Alloc buffers on device
        cudaMalloc(&_d_thisGeneration, rows * cols * sizeof(int));
        cudaMalloc(&_d_lastGeneration, rows * cols * sizeof(int));
    }
    else {
        // Alloc managed buffers
        cudaMallocManaged(&_thisGeneration, rows * cols * sizeof(int));
        cudaMallocManaged(&_lastGeneration, rows * cols * sizeof(int));
        // Set host/device pointers to same value since they are the same buffers
        _d_thisGeneration = _thisGeneration;
        _d_lastGeneration = _lastGeneration;
    }
}

// Destructor for LifeManager class. Frees up any buffers made on init.
LifeManager::~LifeManager() {
    if (_mode == 0) {
        free(_thisGeneration);
        free(_lastGeneration);
        cudaFree(_d_thisGeneration);
        cudaFree(_d_lastGeneration);
    }
    else if (_mode == 1) {
        cudaFreeHost(_thisGeneration);
        cudaFreeHost(_lastGeneration);
        cudaFree(_d_thisGeneration);
        cudaFree(_d_lastGeneration);
    }
    else {
        cudaFree(_thisGeneration);
        cudaFree(_lastGeneration);
    }
}

// This function gets the value at position (r, c) in the must recent generation of life.
// r - Row position
// c - Column position
int LifeManager::get(int r, int c) {
    return _thisGeneration[r * _cols + c];
}

// Sets the value at position (r, c) in the last generation of life. This is used to initialize the values
// at the start of the simulation.
// r - Row position
// c - Column position
void LifeManager::set(int r, int c, int val) {
    _lastGeneration[r * _cols + c] = val;
}

// Initializes the simulation by copying the lastGeneration buffer onto the GPU (if not using managed memory).
// This is functionally a noop if using MANAGED memory mode.
// r - Row position
// c - Column position
void LifeManager::init() {
    if (_mode != 2)
        cudaMemcpy(_d_lastGeneration, _lastGeneration, _rows * _cols * sizeof(int), cudaMemcpyHostToDevice);
    return;
}