/***
决策树，ID3,C4.5,树回归

考虑

1信息增益，信息增益率，最小均方差
2属性类型（double，string）连续值和缺损值
3剪枝
**/



#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#define MAX_SIZE_OF_TRAINING_SET 1000
#define ATTR_NUM 3
#define MAX 1024000
#define MIN 0.0000001
using namespace std;
struct data
{
    int id;
    double attr_double[ATTR_NUM];//用于数值型属性
    data *next;
};
struct twoSubData
{
    data *left;
    data *right;
};
struct split
{
    int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
    double value;//若为分裂节点，则表示分裂阈值，否则为叶子节点，用来记录叶子节点的均值
};
typedef struct bitnode
{
    struct bitnode *left;//小于等于阈值的左子树
    struct bitnode *right;//大于阈值的右子树
    int leafType;//叶子节点类型.0:值，1:模型
    int feature;//只有非叶子节点才有分裂属性
    double value;//叶子节点为值型,非叶子节点为阈值
    int modle[ATTR_NUM];//叶子节点为model型
    struct data *data;//可用于记录data的指针头
    int len;//记录该结点子集的长度
}bitnode,*bitree;
twoSubData binSplitDataSet(data *dataSet,int axis,double value)
{
    twoSubData twosubdata;
    twosubdata.left=new data;
    twosubdata.right=new data;
    twosubdata.left->next=NULL;
    twosubdata.right->next=NULL;
    data *left=twosubdata.left;//=(data *)malloc(sizeof(data)*MAX_SIZE_OF_TRAINING_SET);
    data *right=twosubdata.right;
    data *p;
    data *datatmp;
    p=dataSet->next;
    int i,j,k;
    for(i=0;p!=NULL;i++)
    {
        if(p->attr_double[axis]<=value)
        {
            datatmp=new data;
            datatmp->next=NULL;
            datatmp->id=p->id;
            for(j=0;j<ATTR_NUM;j++)
                datatmp->attr_double[j]=p->attr_double[j];
            left->next=datatmp;
            left=left->next;
        }
        else
        {
            datatmp=new data;
            datatmp->next=NULL;
            datatmp->id=p->id;
            for(j=0;j<ATTR_NUM;j++)
                datatmp->attr_double[j]=p->attr_double[j];
            right->next=datatmp;
            right=right->next;
        }
        p=p->next;
    }
    return twosubdata;
}
double mean(data *dataSet)
{
    int i;
    double meanvalue=0;
    double meanErr=0;
    data *p;
    p=dataSet->next;
    for(i=0;p!=NULL;i++)
    {
        meanvalue+=p->attr_double[ATTR_NUM-1];
        p=p->next;
    }
    meanvalue/=(i);//这里注意i即表示长度，因为i是从0开始算的，所以最后的加1不能减去
    return meanvalue;
}
double MeanErr(data *dataSet)
{
    int i;
    double meanvalue=0;
    double meanErr=0;
    data *p;
    meanvalue=mean(dataSet);
    p=dataSet->next;
    for(i=0;p!=NULL;i++)
    {
        meanErr+=(p->attr_double[ATTR_NUM-1]-meanvalue)*(p->attr_double[ATTR_NUM-1]-meanvalue);
        p=p->next;
    }
    meanErr=sqrt(meanErr/(i));//这里注意i即表示长度，因为i是从0开始算的，所以最后的加1不能减去
    //cout<<"meanErr="<<meanErr<<endl;
    return meanErr;
}
/*double linearSolve(data *dataSet)
{
    int i,j;
    data *p;
    p=dataSet->next;
    while(p!=NULL)
    {
        p
        p=p->next;
    }
}*/
split chooseBestSplit(data *dataSet,int leafType,double minErr,int minLen)
{
    int signvalue=1;
    twoSubData twosubdata;
    data *p;
    data *left;
    data *right;
    split sp;
    int len;
    int i,j;
    double oldMeanErr=MeanErr(dataSet);
    double bestMeanErr=MAX;
    double newMeanErr;
    p=dataSet->next;
    double value=p->attr_double[ATTR_NUM-1];
    for(i=0;p!=NULL;i++)
    {
        signvalue=0;
        if(p->attr_double[ATTR_NUM-1]!=value)
        {
            signvalue=0;
        }
        len++;
        p=p->next;
    }
    if(signvalue||len==1)
    {
        cout<<"signvalue+len"<<endl;
        sp.bestIndex=-1;
        sp.value=mean(dataSet);
        return sp;
    }
    sp.bestIndex=0;
    sp.value=0;
    for(i=0;i<ATTR_NUM-1;i++)
    {
        p=dataSet->next;
        for(j=0;p!=NULL;j++)
        {
            twosubdata=binSplitDataSet(dataSet,i,p->attr_double[i]);
            left=twosubdata.left->next;
            right=twosubdata.right->next;
            len=0;//len记得在进入下次循环是清0
            while(left!=NULL&&right!=NULL)
            {
                left=left->next;
                right=right->next;
                len++;
            }
            //cout<<"len===="<<len<<endl;
            if(len<minLen)
            {
                p=p->next;//提前结束当前循环之前还得把指针指向下一个
                continue;
            }
            newMeanErr=MeanErr(twosubdata.left)+MeanErr(twosubdata.right);
            //cout<<"id="<<j<<"  newMeanErr="<<newMeanErr<<endl;
            if(newMeanErr<bestMeanErr)
            {
                sp.bestIndex=i;
                sp.value=p->attr_double[i];
                bestMeanErr=newMeanErr;
            }
            p=p->next;
        }
    }
    //cout<<"value="<<sp.value<<"  index="<<sp.bestIndex<<endl;
    if(oldMeanErr-bestMeanErr<minErr||oldMeanErr-bestMeanErr<MIN)
    {
        sp.bestIndex=-1;
        sp.value=mean(dataSet);
        //cout<<"minErr"<<endl;
        return sp;
    }
    //cout<<sp.bestIndex<<"&"<<sp.value<<"  ";
    //cout<<oldMeanErr<<"&"<<bestMeanErr<<endl;
    return sp;
}
int createBinTree(bitree &t,data *dataSet)
{
    data *p=dataSet->next;
    if(!(t=(bitnode *)malloc(sizeof(bitnode)))) exit(-1);

    int len=0;
    while(p!=NULL)
    {
        len++;
        //cout<<"data:  "<<p->attr_double[0]<<"  "<<p->attr_double[1]<<"  "<<p->attr_double[2]<<endl;
        p=p->next;
    }
    cout<<"len="<<len<<endl;;
    split sp=chooseBestSplit(dataSet,0,0.01,10);
    //cout<<"index="<<sp.bestIndex<<endl;
    t->feature=sp.bestIndex;
    t->value=sp.value;
    t->data=dataSet;
    t->len=len;
    if(t->feature==-1)
    {

        t->left=NULL;
        t->right=NULL;
        t->data=dataSet;
        t->len=len;
        cout<<"feature="<<t->feature<<"  value="<<t->value<<endl;
        return 0;
    }
    else
    {
        cout<<"feature="<<t->feature<<"  value="<<t->value<<endl;
        twoSubData twosubdata=binSplitDataSet(dataSet,sp.bestIndex,sp.value);
        createBinTree((t->left),twosubdata.left);
        createBinTree((t->right),twosubdata.right);
    }
    return 0;
}
void loadData(data *dataSet,int trainOrtest)
{
    ifstream infile;
    string tmpstrline;
    string tmpstr;
    data *p;
    p=dataSet;
    data *datatmp;
    infile.open("data\\cart.txt",ios::in);
    int i=0,j=0,yblen=0,fetlen=0;
    while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
    {
        getline(infile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
        stringstream input(tmpstrline);
        if(tmpstrline!="\0"&&i%2==trainOrtest)////由于读取文件结束符同样会继续该操作
        {
            datatmp=new data;
            datatmp->id=i;
            datatmp->next=NULL;
            j=0;
            while(input>>datatmp->attr_double[j])
            {
                j++;
                fetlen=j;
            }
            p->next=datatmp;
            p=p->next;
        }
        i++;
    }
    //检测数据加载是否正确
    /*yblen=i;
    p=dataSet->next;
    for(i=0; p!=NULL; i++)
    {
        for(j=0; j<fetlen; j++)
        {
            cout<<p->attr_double[j]<<"  ";
        }
        p=p->next;
        cout<<endl;
    }*/
}
int preorder(bitree &t,data *dataSet)//递归先序遍历二叉树
{
    data *p;
    if(t!=NULL)
    {
        cout<<t->feature<<"  "<<t->value<<"  len="<<t->len<<endl;
        p=dataSet->next;
        if(t->left!=NULL)//这里之所以不再下一次递归时检测t->left是否为NULL，是因为递归函数中另外一个参数用到了left->data
        //常规的先序遍历一般在下一次递归为NULL返回
            preorder(t->left,t->left->data);
        if(t->right!=NULL)
            preorder(t->right,t->right->data);
    }
    return 0;
}
int  prune(bitree &t,data *testData)
{
    data *p=testData;
    int len=0;
    while(p!=NULL)
    {
        p=p->next;
        len++;
    }
    if(len==0)
        return 0;
    if(t==NULL)//检测子树是否为NULL,不然后面的操作执行不了
        return 0;
    if(t->left->feature>-1||t->right->feature>-1)
    {
        twoSubData twosubdata=binSplitDataSet(testData,t->feature,t->value);
        if(t->left->feature>-1)
            prune(t->left,twosubdata.left);
        if(t->right->feature>-1)
        prune(t->right,twosubdata.right);
    }
    else
    {
        twoSubData twosubdata=binSplitDataSet(testData,t->feature,t->value);
        double errortwo=MeanErr(twosubdata.left)+MeanErr(twosubdata.right);
        double errorone=MeanErr(testData);
        if(errorone<errortwo)
        {
            cout<<"merge"<<endl;
            t->left=NULL;
            t->right=NULL;
            t->feature=-1;
            t->value=mean(testData);
            return 0;
        }
    }

}
int main()
{
    data *dataSet=new data;
    dataSet->next=NULL;
    loadData(dataSet,0);
    //MeanErr(dataSet);
    //chooseBestSplit(dataSet,0,0,1);
    bitree t;
    if(!(t=(bitree)malloc(sizeof(bitnode)))) exit(-1);
    //t=NULL;
    createBinTree(t,dataSet);
        cout<<t->feature<<endl;
    preorder(t,dataSet);
    data *testData=new data;
    loadData(testData,1);
    prune(t,testData);
    preorder(t,dataSet);
    return 0;
}

