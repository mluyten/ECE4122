/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 8/29/2024
Description: Queries use for a positive integer and returns the sum of all primes <= the input. Queries indefinitely until user enters 0.
*/

#include <iostream>
#include <string>
#include <cmath>
#include <cstdint>

using std::uint64_t; // Grab c-style unsigned 64-bit int - I refuse to use a long long 
const double MAX_INPUT = std::pow(2,32); // Const for max value of input

// Checks if positive integer n is prime
// true: n is prime, false: n is not prime
bool checkPrime(uint64_t n) {
    for (uint64_t i = 2; i <= std::sqrt(n); i++) { // Checks if n is divistible by any value from 2 to sqrt(n)
        if (n % i == 0) return false;
    }
    return true;
}

// Adds all primes <= n and returns the sum
uint64_t sumPrimes(uint64_t n) {
    uint64_t sum = 0;
    
    if (n >= 2) sum += 2; // Check against the only even prime
    for (uint64_t i = 3; i <= n; i+=2) { // Iterate over all odd numbers from 3 to n
        if (checkPrime(i)) { // If this off number is prime, add it to sum
            sum += i;
        }
    }
    return sum;
}

int main() {
    uint64_t input = -1;
    std::string inputString;
    
    while (true) {
        std::cout << "Please enter a natural number (0 to quit): ";
        std::getline(std::cin, inputString);
        
        try {
            input = std::stol(inputString); // Throws if string cannot be converted to int, won't throw on string with trailing characters (i.e. 123abc)
            if (input < 0) {
                throw std::invalid_argument("Input must be positive"); // Throw if integer is negative
            }
            if (input > MAX_INPUT) {
                throw std::invalid_argument("Input must be < 2^32"); // Throw if integer is greater than max value
            }
            if (inputString.size() != std::to_string(input).size()) {
                throw std::invalid_argument("Input must be an integer"); // Throws if input is floating point or "123abc" or "1 1"
            }
        }
        catch(std::exception e) {
            std::cout << "Error! Invalid input!" << std::endl;
            continue;
        }

        if (input == 0) break; // User enters 0

        std::cout << "The sum of primes is " << sumPrimes(input) << std::endl; // Sum primes <= input
    }
    std::cout << "Program terminated." << std::endl << "Have a nice day!" << std::endl;
    return 0;
}