/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/6/2024

Description:
This defines the LifeManager class. This class manages all the memory for the game of life as well as the 
gpu-based computations for each iteration. On initialization, it allocates the necessary memory as
defined by the desired memory mode. It then updates the initial condition with set(), copys the state
to the GPU with init(), and can progress each iteration using nextGeneration().
*/

#ifndef CELULARAUTONOMA_H
#define CELULARAUTONOMA_H

//This is the LifeManager class. It manages all of the memory and GPU function calls to implement Conway's
// Game of Life.
class LifeManager {
public:
	LifeManager(int rows, int cols, int mode);
	~LifeManager();

	// Gets value of current generation at (r,c)
	int get(int r, int c);
	// Sets value of last generation at (r,c) [for initialization purposes]
	void set(int r, int c, int val);
	// Progresses simulation by one generation using nThreads threads/block on the GPU
	int nextGeneration(int nThreads);
	// Copys initial state to GPU
	void init();
private:
	// Pointers to host and device buffers
	int* _lastGeneration;
	int* _thisGeneration;
	int* _d_lastGeneration;
	int* _d_thisGeneration;

	// Parameters for this simulation
	int _rows;
	int _cols;
	int _mode;
};

#endif