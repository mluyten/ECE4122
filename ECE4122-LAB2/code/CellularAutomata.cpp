/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This implements all cellular automata functions.
*/

#include "CellularAutomata.h"

// Checks adjacent cells to cell at lastGeneration[r][c] and sets its value at thisGeneration[r][c]
// r, c - position of cell to age in matrix
// thisGeneration - most recent generation of automata - this is set by this function
// lastGeneration - last generation of automata - this is read by the function and determines nextGeneration
void ageCell(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    int sum = 0;
    // Iterates through all cells adjacent to desired position and sums number of live cells
    for (int i(r > 0 ? -1 : 0); i <= int(r < thisGeneration.rows()-1 ? 1 : 0); i++) {
        for (int j(c > 0 ? -1 : 0); j <= int(c < thisGeneration.cols()-1 ? 1 : 0); j++) {
            // Ignores desired cell
            if (j != 0 || i != 0)
                sum += lastGeneration[r+i][c+j];
        }
    }

    // Game of life logic
    if (lastGeneration[r][c] && (sum < 2 || sum > 3))
        thisGeneration[r][c] = 0;
    else if (!lastGeneration[r][c] && sum == 3)
        thisGeneration[r][c] = 1;
    else
        thisGeneration[r][c] = lastGeneration[r][c];
    return;
}

// Checks adjacent cells to cells in row at lastGeneration[r] and sets their values in row at thisGeneration[r]
// r - position of cell row to age in matrix
// thisGeneration - most recent generation of automata - this is set by this function
// lastGeneration - last generation of automata - this is read by the function and determines nextGeneration
void ageRow(size_t r, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, std::atomic_bool* done) {
    // Iterates through all columns in given row and determines state of next generation
    for (size_t c = 0; c < thisGeneration.cols(); c++) {
        ageCell(r, c, thisGeneration, lastGeneration);
    }
    // Indicates processing is done
    *done = true;
    return;
}

// Determines state of next cellular automata generation using sequential processing
// thisGeneration - most recent generation of automata - this is set by this function
// lastGeneration - last generation of automata - this is read by the function and determines nextGeneration
// returns time spent processing in microseconds
int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration) {
    double tick = omp_get_wtime();
    // Iterate through all rows and cols to find state of next generation
    for (size_t r = 0; r < thisGeneration.rows(); r++) {
        for (size_t c = 0; c < thisGeneration.cols(); c++) {
            ageCell(r, c, thisGeneration, lastGeneration);
        }
    }
    // Set last generation to the generation we just found
    lastGeneration.copy(thisGeneration);
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}

// Determines state of next cellular automata generation using parallel processing w/ std::thread
// thisGeneration - most recent generation of automata - this is set by this function
// lastGeneration - last generation of automata - this is read by the function and determines nextGeneration
// returns time spent processing in microseconds
int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    double tick = omp_get_wtime();
    std::vector<Worker> threads(nThreads);
    size_t row = 0;
    
    // Iterates through every row and kick off a thread to determine state of that row
    // Does this with a limited number of threads determine by cmd line args.
    while (true) {
        bool allDone = true;
        // Run through all workers and manage them
        for (size_t j = 0; j < threads.size(); j++) {
            // If thread is idle and matrix has not been traversed, kick off a new thread
            if (threads[j].idle && row < thisGeneration.rows()) {
                threads[j].idle = false;
                threads[j].done = false;
                // Allocate new thread on the heap (not super safe, sorry)
                threads[j].thread = new std::thread(ageRow, row, std::ref(thisGeneration), std::ref(lastGeneration), &threads[j].done);
                threads[j].thread->detach();
                row++;
            }
            else {
                // Check if thread is done and not idle
                if (threads[j].done && !threads[j].idle) {
                    // Free thread from heap
                    delete threads[j].thread;
                    // Set idle so it can be used again
                    threads[j].idle = true;
                }
            }
            // Check if all threads are done
            allDone = allDone && threads[j].idle;
        }

        // Exits if matrix is traversed
        if (row == thisGeneration.rows() && allDone)
            break;
    }
    // Set last generation to the generation we just found
    lastGeneration.copy(thisGeneration);
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}

// Determines state of next cellular automata generation using parallel processing w/ OMP
// thisGeneration - most recent generation of automata - this is set by this function
// lastGeneration - last generation of automata - this is read by the function and determines nextGeneration
// returns time spent processing in microseconds
int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads) {
    double tick = omp_get_wtime();
    // Iterate through all rows and cols to find state of next generation w/ OMP magic
    #pragma omp parallel for  collapse(2) num_threads(nThreads)
    for (size_t r = 0; r < thisGeneration.rows(); r++) {
        for (size_t c = 0; c < thisGeneration.cols(); c++) {
            ageCell(r, c, thisGeneration, lastGeneration);
        }
    }
    // Set last generation to the generation we just found
    lastGeneration.copy(thisGeneration);
    double tock = omp_get_wtime();
    return 1000000*(tock-tick);
}
