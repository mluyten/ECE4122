#pragma once
#include <map>
#include <string>
#include <iostream>
#include <atomic>
#include <list>
#include <utility>
#include <thread>
#include <omp.h>
#include "Matrix.h"

void age(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
void ageRow(size_t r, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, std::atomic_bool* done);

int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);
int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);

std::map<std::string, size_t> parseArgs(int argc, char* argv[]);