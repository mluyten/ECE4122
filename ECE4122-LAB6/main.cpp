/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 11/26/2024

Description:
This implements the OpenMPI Monte-Carlo method for solving two definite integral.
This code is adapted from Blaise Barney's "MPI pi Calculation Example - C Version"
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <string>
#include <iostream>

#define MASTER 0 // task ID of master task 

int main(int argc, char *argv[])
{
    int mode = 1;
    int samples = 1000;
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

		if (arg == "-P") {
            // Ensure there is a value following -P
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    mode = std::stoi(argv[i + 1]);
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -P: " << argv[i + 1] << std::endl;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -P" << std::endl;
            }
        }
		else if (arg == "-N") {
            // Ensure there is a value following -N
            if (i + 1 < argc) {
                try {
                    // Convert the next argument to an integer
                    samples = std::stoi(argv[i + 1]);
                    i++; // Skip the next argument since it's already processed
                } 
				catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for -N: " << argv[i + 1] << std::endl;
                }
            } 
            else {
                std::cerr << "Error: No value provided for -N" << std::endl;
            }
        }
		else {
			std::cerr << "Error: Unknown argument " << arg[i] << std::endl;
		}
    }

    int	taskid; // task IDr
    int numtasks; // number of tasks
    int rc; // return code
    
    MPI_Status status;

    MPI_Init(&argc, &argv); 

    // Obtain number of tasks and task ID
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    
    double integralSumLocal = 0; // value of integral calculated by current task 
    double integralSum = 0;	// sum of tasks' integral values
    double integral = 0; // monte-carlo average of integral 

    int samplesPerTask = samples/numtasks;
    if (taskid == MASTER) samplesPerTask += samples % numtasks;

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> distr(0.0, 1.0);

    for (int i = 0; i < samplesPerTask; i++) {
        double x = distr(generator);
  	      
	if (mode == 1) {
            integralSumLocal += std::pow(x, 2);
        }
        else if (mode == 2) {
            integralSumLocal += std::exp(-std::pow(x, 2));
        }
    }

    rc = MPI_Reduce(&integralSumLocal, &integralSum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
 
    // Master computes average of integral values to get approximation
    if (taskid == MASTER)
    {
        integral = integralSum / samples;
        printf("The estimate for integral %i is %f\nBye!\n", mode, integral);
    }
    return 0;
    
    // Wrap it up
    MPI_Finalize();
}

