
/***
决策树，ID3,C4.5,树回归

考虑

1信息增益，信息增益率，最小均方差(树回归中又有值回归和模型回归)
2属性类型（double，string）连续值和缺损值
3剪枝
**/

/**
注：由于一开始采用指针记录子集数据时不是修改指针指向原始数据指向，
而是另外申请空间从新操作，这样会极大浪费内存，以及会带来不必要的复制操作
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#define MAX_SIZE_OF_TRAINING_SET 100
#define MAX_NUMIT 100
#define ATTR_NUM 22
#define MAX 1000000
#define MIN -100000
using namespace std;
struct data
{
    int id;
    string attr[ATTR_NUM];//用于字符串属性
    double attr_double[ATTR_NUM];//用于数值型属性
    double weight;
    data *next;
};
struct twoSubData
{
    data *left;
    data *right;
};
struct Stump
{
    double minErr;
    int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
    string ltOrgt;//用于表示是小于等于为-1，还是大于等于为-1
    double threshVal;
    struct twoSubData twosubdata;//可用于记录data的指针头
    int predict[MAX_SIZE_OF_TRAINING_SET];
    double alpha;
};
//matrixTree matrixtree[MAX_SIZE_OF_TRAINING_SET];//用作存储后序遍历生成树的序列
int node=0;//用作存储后序遍历生成树的序列数组的下标
int fetflag[ATTR_NUM]= {0,0,0,0,0}; //初始化为0,最后一个用于统计还剩多少特征未使用
// 计算划分后子集的信息熵，该信息熵由子集中决策属性出现的概率决定

void loadData(data *dataSet)
{
    ifstream infile;
    string tmpstrline;
    data *p;
    p=dataSet;
    data *datatmp;
    infile.open("data\\adaboost.txt",ios::in);
    int i=0,j=0,yblen=0,fetlen=0;
    while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
    {
        j=0;
        getline(infile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
        stringstream input(tmpstrline);
        if(tmpstrline!="\0")////由于读取文件结束符同样会继续该操作
        {
            datatmp=new data;
            datatmp->id=i;
            datatmp->next=NULL;
            while(input>>datatmp->attr_double[j])
            {
                j++;
                fetlen=j;
            }
            i++;
            p->next=datatmp;
            p=p->next;
        }
    }
    //检测数据加载是否正确
    yblen=i;
    p=dataSet->next;
    for(i=0; p!=NULL; i++)
    {
        for(j=0; j<fetlen; j++)
        {
            cout<<p->attr_double[j]<<"  ";
        }
        p=p->next;
        cout<<endl;
    }
}
double* rangeSize(data *trainData,int axis)
{
    data *p;
    double rangemin=MAX;
    double rangemax=MIN;
    double *range=(double *)malloc(sizeof(double)*2);
    p=trainData->next;
    while(p!=NULL)
    {
        if(p->attr_double[axis]>rangemax)
            rangemax=p->attr_double[axis];
        if(p->attr_double[axis]<rangemin)
            rangemin=p->attr_double[axis];
        p=p->next;
    }
    range[0]=rangemin;
    range[1]=rangemax;
    return range;
}
int* stumpClassify(data *trainData,int axis,double threshVal,string threshIneq)
{
    data *p;
    p=trainData->next;
    int *predict=new int[MAX_SIZE_OF_TRAINING_SET];
    int i=0;
    while(p!=NULL)
    {
        predict[i]=1;
        if(threshIneq=="lt")
            {if(p->attr_double[axis]<=threshVal)
            predict[i]=-1;
            }
        else
            {if(p->attr_double[axis]>threshVal)
            predict[i]=-1;
            }
        p=p->next;
        i++;
    }
    return predict;
}
Stump buildStump(data *dataSet,double *w)
{
    Stump sp;
    int len;
    int i,j,k,l;
    twoSubData twosubdata;
    twosubdata.left=new data;
    twosubdata.right=new data;
    twosubdata.left->next=NULL;
    twosubdata.right->next=NULL;
    data *left=twosubdata.left;//=(data *)malloc(sizeof(data)*MAX_SIZE_OF_TRAINING_SET);
    data *right=twosubdata.right;
    data *p;
    data *datatmp;
    int *predict=new int[MAX_SIZE_OF_TRAINING_SET];
    double errArr[MAX_SIZE_OF_TRAINING_SET];
    double weightError=0;
    double threshVal;
    double minErr=MAX;
    double *range=(double *)malloc(sizeof(double)*2);
    int numSteps=MAX_SIZE_OF_TRAINING_SET/10;
    double stepSize;
    double rangemin,rangemax;
    string threshIneq[2]={"lt","gt"};
    for(i=0; i<ATTR_NUM-1;i++)//属性
    {
        range=rangeSize(dataSet,i);
        rangemin=range[0];
        rangemax=range[1];
        stepSize=(rangemax-rangemin)/numSteps;
        for(j=0;j<=numSteps;j++)//其实是numstep+1步
        {
            threshVal=rangemin+stepSize*j;
            for(k=0;k<2;k++)//大于小于
            {
                predict=stumpClassify(dataSet,i,threshVal,threshIneq[k]);
                p=dataSet->next;
                l=0;
                weightError=0;
                while(p!=NULL&&l<100)
                {
                    //cout<<predict[l]<<"&"<<p->attr_double[l]<<endl;
                    errArr[l]=predict[l]-p->attr_double[ATTR_NUM-1]>0?(predict[l]-p->attr_double[ATTR_NUM-1])/2:-(predict[l]-p->attr_double[ATTR_NUM-1])/2;
                    weightError+=errArr[l]*w[l];
                    p=p->next;
                    l++;
                }
                if(weightError<minErr)
                {
                    minErr=weightError;
                    p=dataSet->next;
                    left=twosubdata.left;
                    right=twosubdata.right;
                    l=0;
                    while(p!=NULL)
                    {
                        sp.predict[l]=predict[l];
                        datatmp=new data;//其实可以采用修改指针指向的方式来记录子集中的数据，而不是另外开辟空间
                        datatmp->next=NULL;
                        datatmp->id=p->id;
                        int j=0;
                        for(j=0; j<ATTR_NUM-1; j++)
                            datatmp->attr_double[j]=p->attr_double[j];
                        datatmp->attr_double[ATTR_NUM-1]=predict[l];
                        if(predict[l]>0)
                        {
                            left->next=datatmp;
                            left=left->next;
                        }
                        if(predict[l]<0)
                        {
                            right->next=datatmp;
                            right=right->next;
                        }
                        p=p->next;
                        l++;
                    }
                    sp.bestIndex=i;
                    sp.minErr=minErr;
                    sp.threshVal=threshVal;
                    sp.twosubdata=twosubdata;
                    sp.ltOrgt=threshIneq[k];
                    cout<<"minErr="<<minErr<<endl;
                    sp.alpha=0.5*log((1-minErr)/minErr);
                }
            }
        }
    }
    return sp;
}
int adaBoostTrainDS(data *trainData,int numIt)
{
    Stump sp[MAX_NUMIT];
    data *p;
    p=trainData->next;
    int i,j,k;
    double len=0;
    double w[MAX_SIZE_OF_TRAINING_SET];
    double wSum=0;
    double aggErr[MAX_SIZE_OF_TRAINING_SET];
    double sumErr;
    while(p!=NULL)
    {
        len++;
        p=p->next;
    }
    for(i=0;i<len;i++)
    {
        w[i]=1.0/len;
        //cout<<w[i]<<" ";
    }
    for(i=0;i<numIt;i++)
    {
        sumErr=0;
        sp[i]=buildStump(trainData,w);
        cout<<"alpha="<<sp[i].alpha<<endl;
        p=trainData->next;
        wSum=0;
        for(j=0;j<len&&p!=NULL;j++)
        {
            w[j]=w[j]*(exp(-1*sp[i].alpha*(p->attr_double[ATTR_NUM-1]*sp[i].predict[j])));
            wSum+=w[j];
            p=p->next;
        }
        for(j=0;j<len;j++)
        {
            w[j]=w[j]/wSum;
            aggErr[j]=0;
        }
        for(j=0;j<len;j++)
        {
            for(k=0;k<i;k++)
                {
                    aggErr[j]+=sp[k].alpha*(sp[k].predict[j]);
                }
            if(aggErr[j]>0)
                aggErr[j]=1;
            else
                aggErr[j]=-1;
        }
        p=trainData->next;
        for(j=0;j<len&&p!=NULL;j++)
        {
            sumErr+=(aggErr[j]-p->attr_double[ATTR_NUM-1])>0?(aggErr[j]-p->attr_double[ATTR_NUM-1])/2:-(aggErr[j]-p->attr_double[ATTR_NUM-1])/2;
            p=p->next;
        }
        if(sumErr/len<0.01)
        {
            cout<<sumErr<<endl;
            break;
        }
        cout<<"sumErr="<<sumErr/len<<endl;
    }
    for(i=0;i<50;i++)//50为投票器个数，但不一定是50，只会小于等于50
    {
        cout<<sp[i].alpha<<"   "<<sp[i].bestIndex<<"   "<<sp[i].threshVal<<"   "<<sp[i].ltOrgt<<"   "<<sp[i].minErr<<endl;
    }
    return 0;
}
int main()
{
    data *trainData=new data;
    trainData->next=NULL;
    loadData(trainData);
    adaBoostTrainDS(trainData,50);
    return 0;
}
