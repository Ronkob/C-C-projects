#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

// Insert Dense class here...
class Dense{
public:
    Dense(Matrix &weights, Matrix &bias, activation::activation_func func);

    Matrix get_weights() const ;

    Matrix get_bias() const;

    activation::activation_func get_activation() const;

    Matrix operator()(Matrix &input) const;

private:
    Matrix _weights;
    Matrix _bias;
    activation::activation_func _func;


};

#endif //DENSE_H
