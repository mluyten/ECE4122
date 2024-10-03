#pragma once
#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include "Matrix.h"

void age(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);

int nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);
int nextGenerationThrd(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);
int nextGenerationOMP(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration, size_t nThreads);

std::map<std::string, size_t> parseArgs(int argc, char* argv[]);