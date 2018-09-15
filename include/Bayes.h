#ifndef Bayes_H
#define Bayes_H
#include "MatrixOpe.h"

struct Par;

typedef vector<string> DIC;

DIC createVocabList(vector<DataStr> dataClass);//生成单词字典

Matrix createFectVec(vector<DataStr> dataClass,DIC dic);//生成训练样本矩阵

int trainNB(Matrix X,Matrix Y);//训练函数的实现，注意对参数进行平滑处理

int classifyNB(Matrix testVecX);//分类决策函数

void getAllFiles(string path, vector <string>&files);

void Bayes();

#endif

