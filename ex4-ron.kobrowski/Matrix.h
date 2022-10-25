// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...

class Matrix
{
    public:

    /************************/
    /*      constructors    */
    /************************/

    Matrix(int rows, int cols); // constructor

    Matrix(); // default ctr

    Matrix(const Matrix &m); // copy-ctr

    ~Matrix(); // destructor

    /************************/
    /*       methods        */
    /************************/

    int get_rows() const;

    int get_cols() const;

    /**
     * Transposes the matrix
     */
    Matrix& transpose();

    /**
     * like flatten() in python
     * transforms a matrix to a single column vector
     */
    Matrix& vectorize();

    /**
     * prints matrix elements
     * space after each element, including the last
     * newline after each row, including the last
     */
    void plain_print();

    /**
     * elementwise multiplication
     * @param m other matrix object
     * @return a matrix object of the dot product
     */
    Matrix dot(const Matrix &m) const;

    /**
     * @return the Frobenius of the matrix
     * sqrt(sum(Matrix^2))
     */
    float norm() const;

    /************************/
    /*      operations      */
    /************************/
    /**
     * return a matrix that is the sum of the two
     * it's a friend so there will be direct access to other matrix's members.
     * @param other
     * @return
     */
    friend Matrix operator+(const Matrix &original, const Matrix &other);

    /**
     * assigment from other matrix
     * should be working no matter of the old dims
     * @param other
     * @return
     */
    Matrix& operator=(const Matrix &other);

    /**
     * returns the matrix-multiplication with other matrix
     * @param other
     * @return
     */
    friend Matrix operator*(const Matrix &original, const Matrix &other);

    /**
     * Scalar multiplication on the right
     * @param c a float constant
     * @return
     */
    Matrix operator*(float c) const;

    /**
     * Scalar multiplication on the left
     * @param c a float constant
     * @param m a matrix constant
     * @return
     */
    friend Matrix operator*(float c, const Matrix &m);

    /**
     * matrix addition a+=b
     * @param other const matrix
     * @return a ref to the original
     */
    Matrix& operator+=(const Matrix &other);

    /**
     * return the i,j element
     * @param i element's row
     * @param j element's col
     * @return
     */
    float operator()(int i, int j) const;

    float& operator()(int i, int j);

    /**
     * return the i'th element of the flatten array
     * @param i
     * @return
     */
    float operator[](int i) const;

    float& operator[](int i);

    /**
     * pretty export of the matrix to ostream
     * @param m
     * @return
     */
    friend std::ostream& operator<<(std::ostream &out, const Matrix &m);

    /**
     * fills matrix elements.
     * has to read input stream fully, otherwise it's an error
     * @param m
     * @return
     */
    friend std::ifstream& operator>>(std::ifstream &in, Matrix &m);


private:
    int _rows;
    int _cols;
    float **_values;

    Matrix(int rows, int cols, float** values);
    float vector_dot(const float* one, const float* two) const;
};


#endif //MATRIX_H