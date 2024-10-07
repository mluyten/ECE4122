/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This is the header for cellular automata functions.
*/

#pragma once
#include <atomic>
#include <list>
#include <thread>
#include <omp.h>
#include "Matrix.h"
#include "Utilities.h"

// Checks adjacent cells to cell at lastGeneration[r][c] and sets its value at thisGeneration[r][c]
void ageCell(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
// Checks adjacent cells to cells in row at lastGeneration[r] and sets their values in row at thisGeneration[r]
void ageRow(size_t r, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, std::atomic_bool* done);

// Determines state of next cellular automata generation using sequential processing
int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
// Determines state of next cellular automata generation using parallel processing w/ std::thread
int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);
// Determines state of next cellular automata generation using parallel processing w/ OMP
int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);