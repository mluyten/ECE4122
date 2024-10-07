/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/7/2024

Description:
This is the header for the matrix class. This class wraps a 1D std::vector and allows it to be accessed like a 2D matrix.
This ensures all elements are sequential in memory and access/copy opperations are quick. It's templated, so it will work
with nearly any type and size.
*/

#pragma once
#include <vector>
#include <cstring>

template <typename T>
class Matrix {
public:
    // Constructor
    Matrix(size_t rows, size_t cols, T fill=0) : d(rows*cols, fill), r(rows), c(cols) {}
    
    // Col class that allows us to do dual access operators. i.e. matrix[row][col] = val
    class Col {
        friend class Matrix;
    public:
        // Accesses a single value based on column
        T& operator[](size_t row) {
            return parent.d[col*parent.c + row];
        }
    private:
        Col(Matrix &parent_, int col_) :
            parent(parent_), col(col_) {}
        Matrix& parent;
        size_t col;
    };

    // Accesses a column
    Col operator[](size_t col) {
        return Col(*this, col);
    }

    // Returns number of rows in matrix
    size_t rows() {
        return r;
    }

    // Returns number of cols in matrix
    size_t cols() {
        return c;
    }

    // Returns copy of the vector that 
    std::vector<T> vector() {
        return d;
    }

    // Returns pointer to start of array in memory
    T* data() {
        return d.data();
    }

    // Coppies contents of one matrix into another
    void copy(Matrix src) {
        std::memcpy(d.data(), src.data(), sizeof(T) * r * c);
        return;
    }

private:
    size_t r, c;
    std::vector<T> d;
};