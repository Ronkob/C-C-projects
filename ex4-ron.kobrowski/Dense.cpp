//
// Created by PC on 8/27/2022.
//
#include "Dense.h"

Dense::Dense(Matrix &weights, Matrix &bias, activation::activation_func func)
{
    _weights = weights;
    _bias = bias;
    _func = func;
}

Matrix Dense::get_weights() const
{
    return _weights;
}

Matrix Dense::get_bias() const
{
    return _bias;
}

activation::activation_func Dense::get_activation() const
{
    return _func;
}

Matrix
Dense::operator()(Matrix &input) const
{
    return _func((_weights*input)+_bias);
}

