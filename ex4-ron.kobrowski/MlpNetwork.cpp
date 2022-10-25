//
// Created by PC on 8/27/2022.
//
#include "MlpNetwork.h"

static digit max(Matrix &output){
    digit ret;
    float max = 0;
    for (int i = 0; i < output.get_rows(); ++i)
    {
        if(output[i]>max){
            ret = {(unsigned)i, output[i]};
            max = output[(int)i];
        }
    }
    return ret;
}

MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases)
{
    for (int i = 0; i < MLP_SIZE; ++i)
    {
        _weights[i] = *(weights+i);
        _biases[i] = *(biases+i);
    }
}

digit MlpNetwork::operator()(Matrix &img)
{
    Dense layer1(_weights[0], _biases[0], activation::relu);
    Matrix r1 = layer1(img);
    Dense layer2(_weights[1], _biases[1], activation::relu);
    Matrix r2 = layer2(r1);
    Dense layer3(_weights[2], _biases[2], activation::relu);
    Matrix r3 = layer3(r2);
    Dense layer4(_weights[3], _biases[3], activation::softmax);
    Matrix r4 = layer4(r3);
    digit selected = max(r4);
    return selected;
}
