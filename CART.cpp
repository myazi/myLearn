/***

回归树 CART

考虑

1最小均方差 ,拟合函数采用均值函数， 拟合函数采用线性模型
2特征阈值的选择是从逐个样本中选择，不是按步长选择
2属性类型（string）连续值
3预剪枝，后剪枝

**/

#include "CART.h"
#define MAX 1000000000

struct twoSubData_C
{
    Data left;
    Data right;
};
struct split_C
{
    int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
    double value;//若为分裂节点，则表示分裂阈值，否则为叶子节点，用来记录叶子节点的均值
};
typedef struct bitnode
{
    struct bitnode *left;//小于等于阈值的左子树
    struct bitnode *right;//大于阈值的右子树
    int feature;//只有非叶子节点才有分裂属性，叶子节点feature=-1
    double meanValue;//如果是叶子节点为值型,非叶子节点为阈值
    Data data;//每颗树都存储了该树下的数据
    bitnode():left(NULL),right(NULL),feature(-1),meanValue(0),data() {};
} bitnode,*bitree;

twoSubData_C binSplitDataSet(const Data &data,const int &axis,const double &value)
{
    twoSubData_C twosubdata;
    for(size_t i=0; i<data.size(); i++)
    {
        if(data[i][axis]<=value)
        {
            twosubdata.left.push_back(data[i]);
        }
        else
        {
            twosubdata.right.push_back(data[i]);
        }
    }
    return twosubdata;
}
double mean_C(const Data &data)
{
    if(data.size()==0)
        return 0;
    double meanvalue=0;
    int label_index=data[0].size()-1;
    for(size_t i=0; i<data.size(); i++)
    {
        meanvalue+=data[i][label_index];
    }
    meanvalue/=data.size();
    return meanvalue;
}
double MeanErr_C(const Data &data)
{
    if(data.size()==0)
        return 0;
    double meanvalue=mean_C(data);
    double meanErr=0;
    int label_index=data[0].size()-1;
    for(size_t i=0; i<data.size(); i++)
    {
        meanErr+=(data[i][label_index]-meanvalue)*(data[i][label_index]-meanvalue);
    }
    meanErr=sqrt(meanErr/data.size());
    return meanErr;
}
split_C chooseBestSplit(const Data &data,const double &minErr)
{
    size_t i,j;
    double oldMeanErr=MeanErr_C(data);
    double bestMeanErr=MAX;
    double newMeanErr;
    int signvalue=1;
    twoSubData_C twosubdata;
    split_C sp;
    int label_index=data[0].size()-1;
    double value=data[0][label_index];
    for(i=0; i<data.size(); i++)
    {
        if(data[i][label_index]!=value)
        {
            signvalue=0;
            break;
        }
    }
    if(signvalue)
    {
        cout<<"signvalue"<<endl;
        sp.bestIndex=-1;
        sp.value=mean_C(data);
        return sp;
    }
    for(i=0; i<data[0].size()-1; i++)
    {
        for(j=0; j<data.size(); j++)
        {
            twosubdata=binSplitDataSet(data,i,data[j][i]);
            if(twosubdata.left.size()==0 || twosubdata.right.size()==0)
                continue;
            newMeanErr=MeanErr_C(twosubdata.left)+MeanErr_C(twosubdata.right);
            if(newMeanErr<bestMeanErr)
            {
                sp.bestIndex=i;
                sp.value=data[j][i];
                bestMeanErr=newMeanErr;
            }
        }
    }
    if(oldMeanErr-bestMeanErr<minErr)
    {
        sp.bestIndex=-1;
        sp.value=mean_C(data);
    }
    return sp;
}
int createBinTree(bitree &t,Data &data)
{
    if(data.size()==0)
        return 0;
    if(!(t=(bitree)malloc(sizeof(bitnode)))) exit(-1);
    split_C sp=chooseBestSplit(data,0.001);
    t->feature=sp.bestIndex;
    t->meanValue=sp.value;
    //t->data=data;//为什么结构体data不能赋值
    if(t->feature==-1)
    {
        t->left=NULL;
        t->right=NULL;
        cout<<"feat-1"<<endl;
        return 0;
    }
    //cout<<"feature="<<t->feature<<"  value="<<t->meanValue<<endl;
    twoSubData_C twosubdata=binSplitDataSet(data,sp.bestIndex,sp.value);
    createBinTree((t->left),twosubdata.left);
    createBinTree((t->right),twosubdata.right);
    return 0;
}
int preorder(bitree &t)//递归先序遍历二叉树
{
    if(t!=NULL)
    {
        if(t->feature==-1)
            cout<<t->feature<<"  "<<t->meanValue<<"  "<<endl;//<<t->data.size()<<endl;//只输出叶子节点
        if(t->left!=NULL)
            preorder(t->left);
        if(t->right!=NULL)
            preorder(t->right);
    }
    return 0;
}
int prune(bitree &t,const Data &testData)
{
    if(t==NULL||testData.size()==0)//检测子树是否为NULL,不然后面的操作执行不了
        return 0;
    if(t->left->feature!=-1||t->right->feature!=-1)
    {
        twoSubData_C twosubdata=binSplitDataSet(testData,t->feature,t->meanValue);
        if(t->left->feature!=-1)
        {
            prune(t->left,twosubdata.left);
        }
        if(t->right->feature!=-1)
        {
            prune(t->right,twosubdata.right);
        }
        if(t->right->feature==-1 && t->left->feature==-1)
        {
            prune(t,testData);
        }
    }
    else
    {
        twoSubData_C twosubdata=binSplitDataSet(testData,t->feature,t->meanValue);
        double errortwo=MeanErr_C(twosubdata.left) + MeanErr_C(twosubdata.right);
        double errorone=MeanErr_C(testData);
        if(errorone<errortwo)
        {
            cout<<"merge"<<endl;
            t->left=NULL;
            t->right=NULL;
            t->feature=-1;
            t->meanValue=mean_C(testData);
        }
        return 0;
    }
    return 0;
}
double predict(bitree t, const RowData &data)//bitree &t，不能引用，不能改变树根
{
    while(t)
    {
        if(t->feature==-1)
            return t->meanValue;
        else
        {
            if(data[t->feature] <= t->meanValue)
                t=t->left;
            else
                t=t->right;
        }
    }
    return 0;
}
int CART()
{
    Data data;
    LoadDataNum(data,"data\\cart.txt");
    bitree t;
    createBinTree(t,data);
    preorder(t);
    Data testdata;
    LoadDataNum(testdata,"data\\carttest.txt");
    prune(t,testdata);
    preorder(t);
    vector<double> labels(testdata.size(),0);
    for(size_t i =0; i<testdata.size(); i++)
    {
        labels[i]=predict(t,testdata[i]);
        cout<<labels[i]<<"&&"<<testdata[i][testdata[0].size()-1]<<endl;
    }
    return 0;
}

