#pragma once
#include <map>
#include <string>
#include <iostream>
#include "Matrix.h"

void age(size_t r, size_t c, Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);

void nextGenerationSeq(Matrix<int>& thisGeneration, Matrix<int>& lastGeneration);

std::map<std::string, size_t> parseArgs(int argc, char* argv[]);