#pragma once
#include <vector>

// This is my fancy templated matrix class
// Since 2D vectors in C++ are not sequential in memory,
// I wanted something that was. Thus: Matrix<T>
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