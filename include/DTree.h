/***
决策树，ID3,C4.5,CART

考虑

1信息增益,信息增益率,基尼指数
2属性类型（string）
3预剪枝
**/

#include "MatrixOpe.h"
#define MAX_SIZE_OF_TRAINING_SET 1000
using namespace std;

struct Tree;

vector<string> getkindattr(const DataStr &data,int axis);//获得axis属性的所有不同取值

double calcShannonEntOrGini(const DataStr &data, const string &type);

DataStr splitDataSet(const DataStr &data,int axis,string value);

int chooseBestFectureTosplit(const DataStr &data, const string &type, double epsilon, int minLen);

Tree dataToTree(const DataStr &data,const string &type, int bbestFet);

int createTree();

int DTree();
