//
// Created by PC on 8/27/2022.
//
#include "Activation.h"

float activation::single_relu(float &x)
{
    if (x<0){
        return 0;
    }
    return x;
}

Matrix activation::relu(const Matrix &x)
{
    Matrix temp = x;
    temp.vectorize();
    for (int i = 0; i < temp.get_rows(); ++i)
    {
        temp[i] = activation::single_relu(temp[i]);
    }
    return temp;
}

Matrix activation::softmax(const Matrix &x)
{
    Matrix temp = x;
    temp.vectorize();
    float sum = 0;
    for (int i = 0; i < temp.get_rows(); ++i)
    {
        sum += std::exp(temp[i]);
    }
    float multiplier = 1/sum;
    for (int i = 0; i <temp.get_rows(); ++i)
    {
        temp[i] = (std::exp(temp[i])*multiplier);
    }
    return temp;
}