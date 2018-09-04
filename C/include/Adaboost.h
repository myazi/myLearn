/***

**/

#include "MatrixOpe.h"

struct twoSubData_A;

struct Stump;

double* rangeSize(const Data &data,int axis);

vector<int> Classify(const Data data, int axis, double threshVal, string threshIneq);

Stump buildStump(const Data &data,vector<double> weight);

int adaBoostTrainDS(const Data &data,const int &numIt);

int Adaboost(const int &numIt);
