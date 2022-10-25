#include "Matrix.h"
#include <cmath>

#ifndef ACTIVATION_H
#define ACTIVATION_H

// Insert activation class here...
namespace activation
{
    typedef Matrix (*activation_func)(const Matrix &x);

    Matrix relu(const Matrix &x);

    Matrix softmax(const Matrix &x);

    float single_relu(float &x);




};


#endif //ACTIVATION_H