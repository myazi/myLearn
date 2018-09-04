#include "MatrixOpe.h"

int autoNorm(Matrix x);

Matrix cdistances(Matrix test,Matrix x);

Matrix getK(Matrix oneTest,Matrix x,int K);

int classfiy(Matrix &testData,Matrix &testDatay,Matrix &x,Matrix &y,const int &K);

int KNN();
