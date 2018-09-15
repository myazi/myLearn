/***

回归树 CART

考虑

1最小均方差 ,拟合函数采用均值函数， 拟合函数采用线性模型
2特征阈值的选择是从逐个样本中选择，不是按步长选择
2属性类型（string）连续值
3预剪枝，后剪枝
**/
struct twoSubData_C;

struct split_C;

typedef struct bitnode bitnode,*bitree;
twoSubData_C binSplitDataSet(const Data &data,const int &axis,const double &value);

double mean_C(const Data &data);

double MeanErr_C(const Data &data);

split_C chooseBestSplit(const Data &data,const double &minErr);

int createBinTree(bitree &t,Data &data);

int preorder(bitree &t);//递归先序遍历二叉树

int prune(bitree &t,const Data &testData);

double predict(bitree t, const RowData &data);//bitree &t，不能引用，不能改变树根

int CART();
