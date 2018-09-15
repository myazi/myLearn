/***

采用CART构成森林

**/

#include "RF.h"
#define MAX 10000000
#define MIN -1
using namespace std;
struct twoSubData_R
{
    Data left;
    Data right;
};
struct split_R
{
    int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
    double value;//若为分裂节点，则表示分裂阈值，否则为叶子节点，用来记录叶子节点的均值
};
typedef struct bitnode_R
{
    struct bitnode_R *left;//小于等于阈值的左子树
    struct bitnode_R *right;//大于阈值的右子树
    int feature;//只有非叶子节点才有分裂属性，叶子节点feature=-1
    double meanValue;//如果是叶子节点为值型,非叶子节点为阈值
    Data data;//每颗树都存储了该树下的数据
} bitnode_R,*bitree_R;
twoSubData_R binSplitDataSet(const Data &data,int axis,double value)
{
    twoSubData_R twosubdata;
    int i,j,k;
    for(i=0; i<data.size(); i++)
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
double mean_R(const Data &data)
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
double MeanErr_R(const Data &data)
{
    if(data.size()==0)
        return 0;
    double meanvalue=mean_R(data);
    double meanErr=0;
    int label_index=data[0].size()-1;
    for(size_t i=0; i<data.size(); i++)
    {
        meanErr+=(data[i][label_index]-meanvalue)*(data[i][label_index]-meanvalue);
    }
    meanErr=sqrt(meanErr/data.size());
    return meanErr;
}
split_R chooseBestSplit(const Data &data,const int &deep, double minErr,int minLen)
{
    int signvalue=1;
    twoSubData_R twosubdata;
    split_R sp;
    int i,j;
    double oldMeanErr=MeanErr_R(data);
    double bestMeanErr=MAX;
    double newMeanErr;
    double value=data[0][data[0].size()-1];
    for(i=0; i<data.size(); i++)
    {
        signvalue=1;
        if(data[i][data[0].size()-1]!=value)
        {
            signvalue=0;
            break;
        }
    }
    if(signvalue)
    {
        cout<<"signvalue"<<endl;
        sp.bestIndex=-1;
        sp.value=mean_R(data);
        return sp;
    }
    for(i=0; i<deep && i<data[0].size()-1; i++)
    {
        for(j=0; j<data.size(); j++)
        {
            twosubdata=binSplitDataSet(data,i,data[j][i]);
            if(twosubdata.left.size()==0 || twosubdata.right.size()==0)
                continue;
            newMeanErr=MeanErr_R(twosubdata.left)+MeanErr_R(twosubdata.right);
            //cout<<"id="<<j<<"  newMeanErr="<<newMeanErr<<endl;
            if(newMeanErr<bestMeanErr)
            {
                sp.bestIndex=i;
                sp.value=data[j][i];
                bestMeanErr=newMeanErr;
            }
        }
    }
    //cout<<"value="<<sp.value<<"  index="<<sp.bestIndex<<endl;
    if(oldMeanErr-bestMeanErr<minErr||oldMeanErr-bestMeanErr<MIN)
    {
        sp.bestIndex=-1;
        sp.value=mean_R(data);
        //cout<<"minErr"<<endl;
        return sp;
    }
    //cout<<sp.bestIndex<<"&"<<sp.value<<"  ";
    //cout<<oldMeanErr<<"&"<<bestMeanErr<<endl;
    return sp;
}
int createBinTree(bitree_R &t,const Data &data, const int &deep, const int &epsilon)
{
    if(!(t=(bitnode_R *)malloc(sizeof(bitnode_R)))) exit(-1);
    split_R sp=chooseBestSplit(data,deep,epsilon,10);
    cout<<"index="<<sp.bestIndex<<endl;
    t->feature=sp.bestIndex;
    t->meanValue=sp.value;
    //t->data=data;
    if(t->feature==-1)
    {
        t->left=NULL;
        t->right=NULL;
        //t->data=data;
        cout<<"feat-1"<<endl;
        return 0;
    }
    else
    {
        cout<<"feature="<<t->feature<<"  value="<<t->meanValue<<endl;
        twoSubData_R twosubdata=binSplitDataSet(data,sp.bestIndex,sp.value);
        createBinTree((t->left),twosubdata.left,deep,epsilon);
        createBinTree((t->right),twosubdata.right,deep,epsilon);
    }
    return 0;
}
int preorder(bitree_R &t)//递归先序遍历二叉树
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
double predict(bitree_R t, const RowData &data)//bitree_R &t，不能引用，不能改变树根
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
Data Bootstrap(const Data &data)
{
    Data bdata;
    int smp;
    for(size_t i=0;i<data.size();i++)
    {
        smp=rand()%data.size();
        bdata.push_back(data[smp]);
    }
    return bdata;
}
int RF()
{
    const int numIt=50;
    const int deep=5;
    const double epsilon=0.01;
    Data data;
    Data bdata;
    LoadDataNum(data,"data\\rf.txt");
    bitree_R *t;
    if(!(t=(bitree_R*)malloc(sizeof(bitree_R)*numIt))) exit(-1);
    for(int i=0;i<numIt;i++)
    {
        cout<<"It="<<i+1<<"---------------------"<<endl;
        bdata = Bootstrap(data);
        createBinTree(t[i],bdata,deep,epsilon);
        preorder(t[i]);
    }
    Data testdata;
    LoadDataNum(testdata,"data\\rftest.txt");
    vector<double> labels(testdata.size(),0);
    vector<double> bs(numIt,0);
    for(size_t i =0; i<testdata.size(); i++)
    {
        for(size_t it=0;it<numIt;it++)
        {
            bs[it]=predict(t[it],testdata[i]);
            labels[i]+=bs[it];
        }
        labels[i]/=double(numIt);
        cout<<labels[i]<<"&&"<<testdata[i][testdata[0].size()-1]<<endl;
    }
    return 0;
}

