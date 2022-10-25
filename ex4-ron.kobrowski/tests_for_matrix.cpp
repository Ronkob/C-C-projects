//
// Created by PC on 8/28/2022.
//
#include "Matrix.h"

bool readFileToMatrix (const std::string &filePath, Matrix &mat)
{
    std::ifstream fin(filePath, std::ios::binary);
    fin>>mat;
    return true;
}

int main(int argc, char* argv[]){
//    Matrix mat = Matrix(28,28);
//    readFileToMatrix(argv[1], mat);
//    Matrix mat2 = mat;
//    mat2.transpose();
//    mat = mat2;
//

    Matrix mat(2, 3);
    Matrix mat2(2, 3);
    mat2.transpose();
    mat2.vectorize();
}
