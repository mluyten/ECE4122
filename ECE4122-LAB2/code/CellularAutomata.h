/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This is the header for my utility functions.
*/

#pragma once
#include <atomic>
#include <list>
#include <thread>
#include <omp.h>
#include "Matrix.h"
#include "Utilities.h"

void ageCell(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
void ageRow(size_t r, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, std::atomic_bool* done);

int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);
int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);