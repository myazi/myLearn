/***

**/

#include "Adaboost.h"
//#define MAX_SIZE_OF_TRAINING_SET 100
//#define MAX_NUMIT 100
//#define ATTR_NUM 22
#define MAX 1000000
#define MIN -100000
using namespace std;

struct twoSubData_A
{
    Data left;
    Data right;
};
struct Stump
{
    double minErr;
    int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
    string ltOrgt;//大于阈值的类别为-1，还是小于阈值的类别为-1
    double threshVal;
    vector<int> label;
    double alpha;//基学习器投票权重
    twoSubData_A twosubdata;//树桩包括两个分支的数据
};

vector<Stump> stump;
double* rangeSize(const Data &data,int axis)
{
    double rangemin=MAX;
    double rangemax=MIN;
    double *range=(double *)malloc(sizeof(double)*2);
    for(size_t i=0; i<data.size(); i++)
    {
        if(data[i][axis]>rangemax)
            rangemax=data[i][axis];
        if(data[i][axis]<rangemin)
            rangemin=data[i][axis];
    }
    range[0]=rangemin;
    range[1]=rangemax;
    return range;
}
vector<int> Classify(const Data data, int axis, double threshVal, string threshIneq)
{
    vector<int> label;
    for(size_t i=0; i<data.size(); i++)
    {
        label.push_back(1);
        if(threshIneq=="lt")
        {
            if(data[i][axis]<=threshVal)
                label[i]=-1;
        }
        else
        {
            if(data[i][axis]>threshVal)
                label[i]=-1;
        }
    }
    return label;
}
Stump buildStump(const Data &data,vector<double> weight)
{
    Stump stump;
    int i,j,k,l;
    double *errArr=(double *)malloc(sizeof(double)*data.size());
    double minErr=MAX;
    double weightError=0;
    double *range=(double *)malloc(sizeof(double)*2);
    double rangemin,rangemax;
    double stepSize;
    int numSteps=data.size()/10;
    double threshVal;
    int label_index=data[0].size()-1;
    vector<int> label;
    string threshIneq[2]= {"lt","gt"};
    for(i=0; i<data[0].size()-1; i++) //属性
    {
        range=rangeSize(data,i);
        rangemin=range[0];
        rangemax=range[1];
        stepSize=(rangemax-rangemin)/numSteps;
        for(j=0; j<=numSteps; j++) //其实是numstep+1步
        {
            threshVal=rangemin+stepSize*j;
            for(k=0; k<2; k++) //大于小于
            {
                label=Classify(data,i,threshVal,threshIneq[k]);
                weightError=0;
                for(l=0; l<data.size(); l++)
                {
                    errArr[l]=label[l]-data[l][label_index]>0?(label[l]-data[l][label_index])/2:-(label[l]-data[l][label_index])/2;
                    weightError+=errArr[l]*weight[l];
                }
                if(weightError<minErr)
                {
                    minErr=weightError;
                    for(l=0; l<data.size(); l++)
                    {
                        if(label[l]>0)
                            stump.twosubdata.left.push_back(data[l]);
                        else
                            stump.twosubdata.right.push_back(data[l]);
                    }
                    stump.label=label;
                    stump.bestIndex=i;
                    stump.minErr=minErr;
                    stump.threshVal=threshVal;
                    stump.ltOrgt=threshIneq[k];
                    cout<<"minErr="<<minErr<<endl;
                    stump.alpha=0.5*log((1-minErr)/minErr);
                }
            }
        }
    }
    return stump;
}
int adaBoostTrainDS(const Data &data,const int &numIt)
{
    int i,j,k;
    int label_index=data[0].size()-1;
    vector<double> weight;
    double weightSum=0;
    vector<double> aggErr;
    double sumErr=0;
    vector<Stump> stump(numIt);
    for(i=0; i<data.size(); i++)
    {
        weight.push_back(1.0/data.size());
        aggErr.push_back(0);
    }
    for(i=0; i<numIt; i++)
    {
        stump[i] = buildStump(data,weight);//写成赋值语句结构体中的结构体复制出现问题
        cout<<"alpha===="<<stump[i].alpha<<endl;
        weightSum=0;
        for(j=0; j<data.size(); j++)
        {
            weight[j]=weight[j]*(exp(-1*stump[i].alpha*(data[i][label_index]*stump[i].label[j])));
            weightSum+=weight[j];
        }
        for(j=0; j<data.size(); j++)
        {
            weight[j]/=weightSum;
        }
        for(j=0; j<data.size(); j++)
        {
            for(k=0; k<i; k++)
            {
                aggErr[j]+=stump[k].alpha*(stump[k].label[j]);
            }
            if(aggErr[j]>0)
                aggErr[j]=1;
            else
                aggErr[j]=-1;
        }
        sumErr=0;
        for(j=0; j<data.size(); j++)
        {
            sumErr+=(aggErr[j]-data[0][label_index])>0?(aggErr[j]-data[0][label_index])/2:-(aggErr[j]-data[0][label_index])/2;
        }
        if(sumErr/data.size()<0.01)
        {
            cout<<sumErr<<endl;
            break;
        }
    }
    for(i=0; i<numIt; i++) //50为投票器个数，但不一定是50，只会小于等于50
    {
        cout<<stump[i].alpha<<"   "<<stump[i].bestIndex<<"   "<<stump[i].threshVal<<"   "<<stump[i].ltOrgt<<"   "<<stump[i].minErr<<endl;
    }
    return 0;
}
int Adaboost()
{
    const int numIt=20;
    Data data;
    LoadDataNum(data,"data\\adaboost.txt");
    adaBoostTrainDS(data,numIt);
    return 0;
}
