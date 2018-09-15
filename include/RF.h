struct twoSubData_R;

struct split_R;

typedef struct bitnode_R bitnode_R,*bitree_R;
twoSubData_R binSplitDataSet(const Data &data,int axis,double value);

double mean_R(const Data &data);

double MeanErr_R(const Data &data);

split_R chooseBestSplit(const Data &data,const int &deep, double minErr,int minLen);

int createBinTree(bitree_R &t,const Data &data, const int &deep, const int &epsilon);

int preorder(bitree_R &t);//递归先序遍历二叉树

double predict(bitree_R t, const RowData &data);//bitree_R &t，不能引用，不能改变树根

Data Bootstrap(const Data &data);

int RF();

