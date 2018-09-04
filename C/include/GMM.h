#include "MatrixOpe.h"

struct Par;

double COV2(Matrix Sigma);

Matrix E_step(Matrix &x, Par &par);

int M_step(const Matrix w, Matrix &x, Par &par);

int GMM(const int &K, const int &Iter);
