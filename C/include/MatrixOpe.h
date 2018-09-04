#ifndef MatrixOpe_H
#define MatrixOpe_H
#include "Matrix.h"

Matrix operator+(Matrix matrix1,Matrix matrix2);

Matrix operator-(Matrix matrix1,Matrix matrix2);

Matrix operator*(Matrix matrix1,Matrix matrix2);

Matrix operator*(double alpha,Matrix matrix1);

Matrix operator/(Matrix matrix1 ,double alpha);

float sigmoid(float z);

Matrix sigmoid(Matrix z);

Matrix one_hot(Matrix Y, int C);
#endif // MatrixOpe_H
