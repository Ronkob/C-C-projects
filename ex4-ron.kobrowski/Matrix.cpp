//
// Created by PC on 8/27/2022.
//

#include "Matrix.h"
#include <cmath>
#define BOW_THRESHOLD 0.1
#define DEFAULT_MATRIX_SIZE 1
#define DEFAULT_FILL_VALUE 0
#define INCORRECT_NUM_DIMS "Error: Incorrect number of dims"
#define INCORRECT_MATRIX_DIMS "Error: Initialize with integer dims"
#define INVALID_COORDS "Error: Invalid coordination or out of range"
#define INVALID_FILE "Error: invalid input file stream"

/************************/
/*   aux and helpers    */
/************************/

/**
 * checking for identical dims
 *
 * @param row1
 * @param row2
 * @param col1
 * @param col2
 * @return
 */
bool check_dims(const int row1, const int row2, const int col1, const int col2)
{
    return (row1 == row2) && (col1 == col2);
}

/**
 * check validity for multiplication
 * @param rows
 * @param cols
 * @return
 */
bool check_dims(const int rows, const int cols)
{
    return (rows == cols);
}


/**
 * closed vector product to float
 * @param one
 * @param two
 * @return
 */
float Matrix::vector_dot(const float* one, const float* two) const
{
    float ret = 0.0;
    for (int i = 0; i < _cols ; i++)
    {
        ret += one[i] * two[i];
    }
    return ret;
}

void delete_values(float ***values, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] (*values)[i];
    }
    delete[] *values;
}

/************************/
/*      constructors    */
/************************/

Matrix::Matrix(const int rows, const int cols)
{
    if (rows <= 0 || cols <=0){
        throw std::length_error(INCORRECT_MATRIX_DIMS);
    }
    _rows = rows;
    _cols = cols;
    auto **values = new float *[rows];
    for (int i = 0; i < rows; i++)
    {
        values[i] = new float[cols];
        for (int j = 0; j < cols; j++)
        {
            values[i][j] = DEFAULT_FILL_VALUE;
        }
    }
    _values = values;
}

Matrix::Matrix() : Matrix(DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE)
{

}

Matrix::Matrix(const Matrix &m): Matrix()
{
    this->operator=(m); // really like it, hopefully works
}

Matrix::~Matrix()
{
    delete_values(&_values, _rows);
}


Matrix::Matrix(int rows, int cols, float **values) // private - not checking
{
    _rows = rows;
    _cols = cols;
    _values = values;
}

/************************/
/*       methods        */
/************************/


int Matrix::get_rows() const
{
    return _rows;
}

int Matrix::get_cols() const
{
    return _cols;
}

Matrix& Matrix::transpose()
{
    Matrix original = *this;
    // make sure it's not a copy
    delete_values(&_values, _rows);
    _rows = original.get_cols();
    _cols = original.get_rows();
    auto **values = new float *[_rows];
    for (int i = 0; i < _rows; i++)
    {
        values[i] = new float[_cols];
        for (int j = 0; j < _cols; j++)
        {
            values[i][j] = original.operator()(j,i);
            // passing a ref to a value
        }
    }
    _values = values;
    return *this;
}

Matrix& Matrix::vectorize()
{
    Matrix original = *this;
    int vec_size = _rows * _cols;
    delete_values(&_values, _rows);
    auto **values = new float *[vec_size];
    for (int i = 0; i < vec_size; ++i)
    {
        values[i] = new float[1];
        values[i][0] = (original.operator[](i));
        // passing pointer to a heap stored float
    }
    _rows = vec_size;
    _cols = 1;
    _values = values;
    return *this;
}

void Matrix::plain_print()
{
    for (int i = 0; i < _rows; ++i)
    {
        for (int j = 0; j < _cols; ++j)
        {
            std::cout << _values[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::dot(const Matrix &m) const
{
    if (!check_dims(_rows, m.get_rows(),_cols, m.get_cols()))
    {
        throw std::length_error(INCORRECT_NUM_DIMS);
    }
    auto **values = new float *[_rows];
    for (int i = 0; i < _rows; ++i)
    {
        values[i] = new float[_cols];
        for (int j = 0; j < _cols; ++j)
        {
            values[i][j] = _values[i][j] * m.operator()(i, j);
        }
    }
    return {_rows, _cols, values};
}

float Matrix::norm() const
{
    float norm = 0.0;
    for (int i = 0; i < _rows; ++i)
    {
        for (int j = 0; j < _cols; ++j)
        {
            norm += (float)pow(_values[i][j], 2);
        }
    }
    return sqrtf(norm);

}


/************************/
/*      operations      */
/************************/

Matrix &Matrix::operator=(const Matrix &other)
{
    if(&other == this){
        return *this;
    }
    delete_values(&_values, _rows);
    auto **values = new float *[other.get_rows()];
    for (int i = 0; i < other.get_rows(); i++)
    {
        values[i] = new float[other.get_cols()];
        for (int j = 0; j < other.get_cols(); j++)
        {
            values[i][j] = other.operator()(i, j); // passing a ref to a value
        }
    }
    _values = values;
    _rows = other.get_rows();
    _cols = other.get_cols();
    return *this;
}

Matrix operator+(const Matrix &original, const Matrix &other)
{
    if (!check_dims(original._rows, other._rows, original._cols, other._cols))
    {
        throw std::length_error(INCORRECT_NUM_DIMS);
    }
    auto **values = new float *[original._rows];
    for (int i = 0; i < original._rows; i++)
    {
        values[i] = new float[other._cols];
        for (int j = 0; j < other._cols; j++)
        {
            values[i][j] = other.operator()(i, j) + original.operator()(i,
                                                                        j); // passing a ref to a value
        }
    }
    return {original.get_rows(), other.get_cols(), values};
}


Matrix operator*(const Matrix &original, const Matrix &other)
{
    if (!check_dims(original._cols, other._rows))
    {
        throw std::length_error(INCORRECT_NUM_DIMS);
    }
    Matrix temp = other;
    temp.transpose();

    auto **values = new float *[original._rows];
    for (int i = 0; i < original._rows; ++i)
    {
        values[i] = new float[other._cols];
        for (int j = 0; j < other._cols; ++j)
        {
            values[i][j] = original.vector_dot(original._values[i], temp._values[j]);
        }
    }
    return {original._rows, other._cols, values};
}

Matrix Matrix::operator*(const float c) const
{
    Matrix m = Matrix(_rows, _cols);
    for (int i = 0; i < _rows * _cols; ++i)
    {
        m.operator[](i) = (this->operator[](i))*c;
    }
    return m;
}

Matrix operator*(float c, const Matrix &m)
{
    Matrix new_matrix = Matrix(m.get_rows(), m.get_cols());
    for (int i = 0; i < m.get_rows() * m.get_cols(); ++i)
    {
        new_matrix.operator[](i) = (m.operator[](i))*c;
    }
    return new_matrix;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (!check_dims(_rows, other.get_rows(), _cols, other.get_cols()))
    {
        throw std::length_error(INCORRECT_NUM_DIMS);
    }
    auto **values = new float *[_rows];
    for (int i = 0; i < _rows; ++i)
    {
        values[i] = new float[_cols];
        for (int j = 0; j < _cols; ++j)
        {
            values[i][j] = _values[i][j] + other.operator()(i, j);
        }
    }
    delete_values(&_values, _rows);
    _values = values;
    return *this;

}

float Matrix::operator()(int i, const int j) const
{
    if (i<0 || j<0 || i>=_rows || j>=_cols){
        throw std::out_of_range(INVALID_COORDS);
    }
    return _values[i][j];
}

float& Matrix::operator()(int i, const int j)
{
    if (i<0 || j<0 || i>=_rows || j>=_cols){
        throw std::out_of_range(INVALID_COORDS);
    }
    return _values[i][j];
}

float Matrix::operator[](const int i) const
{
    if (i<0 ||  i>=_rows*_cols){
        throw std::out_of_range(INVALID_COORDS);
    }
    return _values[(i / (_cols))][i % _cols];

}
float& Matrix::operator[](const int i)
{
    if (i<0 || i>=_rows*_cols){
        throw std::out_of_range(INVALID_COORDS);
    }
    return _values[(i / (_cols))][i % _cols];
}

std::ostream& operator<<(std::ostream &out, const Matrix &m)
{
    for (int i = 0; i < m.get_rows(); ++i)
    {
        for (int j = 0; j < m.get_cols(); ++j)
        {
            if (m.operator()(i, j) > BOW_THRESHOLD)
            {
                out << "**";
            } else
            {
                out << "  ";
            }
        }
        out << std::endl;
    }
    return out;

}

std::ifstream& operator>>(std::ifstream &in, Matrix &m)
{
    if(in){
        // get length of file
        in.seekg(0, std::ifstream::end);
        int length = (int) in.tellg();
        if(length < (int) (m.get_cols()*m.get_rows()*sizeof(float))){
            throw std::runtime_error(INVALID_FILE);
        }
        in.seekg (0, std::istream::beg);


        float buffer;
        for(int i= 0; i<(m.get_rows())*(m.get_cols()) && in; i++){
            in.read(reinterpret_cast<char*>(&buffer), sizeof(float));
            m.operator[](i) = buffer;
        }

        if(!in){
            throw std::runtime_error(INVALID_FILE);
        }
        in.close();

    }
    else{
        throw std::runtime_error(INVALID_FILE);
    }
    return in;

}


