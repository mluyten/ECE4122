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

template <typename T>
class Matrix {
public:
    Matrix(size_t rows, size_t cols, T fill=0) : d(rows*cols, fill), r(rows), c(cols) {}

    class Col {
        friend class Matrix;
    public:
        T& operator[](size_t row) {
            return parent.d[col*parent.c + row];
        }
    private:
        Col(Matrix &parent_, int col_) :
            parent(parent_), col(col_) {}
        Matrix& parent;
        size_t col;
    };

    Col operator[](size_t col) {
        return Col(*this, col);
    }

    size_t rows() {
        return r;
    }

    size_t cols() {
        return c;
    }

    std::vector<T> data() {
        return d;
    }

private:
    size_t r, c;
    std::vector<T> d;
};