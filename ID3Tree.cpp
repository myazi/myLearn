/***
决策树，ID3,C4.5,树回归

考虑

1信息增益，信息增益率，最小均方差(树回归中又有值回归和模型回归)
2属性类型（double，string）连续值和缺损值
3剪枝
**/



#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#define MAX_SIZE_OF_TRAINING_SET 100
#define ATTR_NUM 5
#define JC_ATTR_KIND 10
#define FET_ATTR_KIND 10
using namespace std;
struct data
{
    int id;
    string attr[ATTR_NUM];//用于字符串属性
    double attr_double[ATTR_NUM];//用于数值型属性
    data *next;
};
struct matrixTree
{
    int id;
    int splitfet;
    string fetvalue;
    string jc_attr;
    int len;
    int kind;
    int parent;
    matrixTree *next;
    struct data *data;//可用于记录data的指针头
};
matrixTree matrixtree[MAX_SIZE_OF_TRAINING_SET];//用作存储后序遍历生成树的序列
stack<matrixTree> s;
int node=0;//用作存储后序遍历生成树的序列数组的下标
int fetflag[ATTR_NUM]= {0,0,0,0,0}; //初始化为0,最后一个用于统计还剩多少特征未使用
// 计算划分后子集的信息熵，该信息熵由子集中决策属性出现的概率决定
string* getkindattr(data *dataSet,int axis)
{
    int i,j,kind=0;
    data *p;
    p=dataSet->next;
    string attr_kind_sum[JC_ATTR_KIND];
    string *att=new string[JC_ATTR_KIND];
    for(i=0; p!=NULL; i++)
    {
        for(j=0; j<kind; j++)
        {
            if(p->attr[axis]==attr_kind_sum[j])
            {
                break;
            }
        }
        if(j==kind)
        {
            attr_kind_sum[kind]=p->attr[axis];
            kind++;
        }
        p=p->next;
    }
    attr_kind_sum[kind]="0";
    i=0;
    while(attr_kind_sum[i]!="0")
    {
        att[i]=attr_kind_sum[i];
        i++;
    }
    att[i]="0";
    //检查得到当前特征所有可能的取值是否正确
    /*
    for(i=0; attr_kind_sum[i]!="0"; i++)
    {
        cout<<attr_kind_sum[i]<<"  ";
    }
    cout<<endl;*/
    return att;
}

double calcShannonEnt(data *dataSet)
{
    int i,j;
    double len=0;
    data *p;
    p=dataSet->next;
    string *jc_attr_kind=(string *)malloc(sizeof(string)*JC_ATTR_KIND);
    jc_attr_kind=getkindattr(dataSet,ATTR_NUM-1);
    double jc_attr_kind_sum[JC_ATTR_KIND];
    double prob=0;
    double shannoEnt=0;
    for(i=0; i<JC_ATTR_KIND; i++)
    {
        jc_attr_kind_sum[i]=0;
    }
    for(i=0; p!=NULL; i++)
    {
        for(j=0; jc_attr_kind[j]!="0"; j++)
        {
            if(p->attr[ATTR_NUM-1]==jc_attr_kind[j])//初始化为0记得
            {
                jc_attr_kind_sum[j]++;
            }
        }
        len++;
        p=p->next;
    }
    //统计该子数据集上的所有样本的决策属性以及不同决策属性上样本数
    for(i=0; jc_attr_kind[i]!="0"; i++)
    {
        cout<<"subdatasetLen="<<len<<"   "<<jc_attr_kind[i]<<"="<<jc_attr_kind_sum[i]<<endl;
    }
    for(i=0; i<JC_ATTR_KIND; i++)
    {
        if(jc_attr_kind_sum[i]!=0)
        {
            prob=jc_attr_kind_sum[i]/len;
            shannoEnt-=prob*log(prob);
        }
    }
    //检测信息熵计算是否正确
    cout<<"shannoEnt="<<shannoEnt<<endl;
    return shannoEnt;
}

data* splitDataSet(data *dataSet,int axis,string value)
{
    data *subDataSet;//=(data *)malloc(sizeof(data)*MAX_SIZE_OF_TRAINING_SET);
    subDataSet=new data;
    subDataSet->next=NULL;
    data *q=subDataSet;
    data  *p;
    data *datatmp;
    p=dataSet->next;
    int i,j,k;
    for(i=0; p!=NULL; i++)
    {
        if(p->attr[axis]==value)
        {
            datatmp=new data;
            datatmp->next=NULL;
            datatmp->id=p->id;
            for(k=0; k<ATTR_NUM; k++)
                datatmp->attr[k]=p->attr[k];
            q->next=datatmp;
            q=q->next;
        }
        p=p->next;
    }
    q=subDataSet->next;
    cout<<"value="<<value;
    while(q!=NULL)
    {
        cout<<" "<<q->id;
        q=q->next;
    }
    cout<<endl;
    return subDataSet;
}

int chooseBestFectureTosplit(data *dataSet,double minInfoGain,int minLen)
{
    int i,j,k;
    double baseEnt=calcShannonEnt(dataSet);
    double newEnt=0;
    double bestInfoGain=0;
    int bestFeature=-1;
    double infoGain=0;
    double splitInfo=0;
    double gainRatio=0;
    double prob=0;
    double len=0;
    data *subDataSet;
    data *p;
    p=dataSet->next;
    string *fet_attr_kind[ATTR_NUM-1];
    int *fet_attr_kind_sum[ATTR_NUM-1];//初始化为0记得
    for(i=0; i<ATTR_NUM-1; i++)
    {
        if(fetflag[i]!=1)
        {
            fet_attr_kind_sum[i]=(int *)malloc(sizeof(int)*FET_ATTR_KIND);
            fet_attr_kind[i]=(string *)malloc(sizeof(string)*FET_ATTR_KIND);
            fet_attr_kind[i]=getkindattr(dataSet,i);
            for(j=0; j<FET_ATTR_KIND; j++)
                fet_attr_kind_sum[i][j]=0;
        }
    }
    for(i=0; p!=NULL; i++)
    {
        for(j=0; j<ATTR_NUM-1; j++)
        {
            if(fetflag[j]!=1)
            {
                for(k=0; fet_attr_kind[j][k]!="0"; k++)
                {
                    if(p->attr[j]==fet_attr_kind[j][k])
                    {
                        fet_attr_kind_sum[j][k]++;
                    }
                }
            }
        }
        len++;
        p=p->next;
    }
    for(i=0; i<ATTR_NUM-1; i++)
    {
        if(fetflag[i]!=1)
        {
            newEnt=0;
            splitInfo=0;
            gainRatio=0;
            for(j=0; fet_attr_kind[i][j]!="0"; j++)
            {
                subDataSet=splitDataSet(dataSet,i,fet_attr_kind[i][j]);
                int sublen=0;
                p=subDataSet->next;
                while(p!=NULL)
                {
                    sublen++;
                    p=p->next;
                }
                if(sublen<minLen)
                {
                    newEnt=100;//结束本次循环后，把此次的信息熵置为MAX，即不选择该属性
                    break;//预剪枝操作
                }
                prob=fet_attr_kind_sum[i][j]/len;
                splitInfo+=(-log(prob)*prob);//计算信息增益率的分母
                cout<<"splitInfo="<<splitInfo<<endl;
                newEnt+=prob*calcShannonEnt(subDataSet);
            }
            //gainRatio=newEnt/splitInfo;//计算信息增益率
            infoGain=baseEnt-newEnt;
            cout<<"newEnt="<<newEnt<<endl;
            cout<<"fet="<<i<<"infogain="<<infoGain<<endl;
            if(infoGain>bestInfoGain&&infoGain>minInfoGain)//信息增益必须大于阈值，才考虑用于属性分裂
            {
                bestInfoGain=infoGain;
                bestFeature=i;
            }
        }
    }
    cout<<"bestFet="<<bestFeature<<endl;
    return bestFeature;
}
matrixTree dataToTree(data *dataSet,int bbestFet)
{
    int i,j,k;
    int signalflag=1;
    int len=0;
    int bestFet=0;
    data *p;
    p=dataSet->next;
    string fet_attr_kind[ATTR_NUM-1][FET_ATTR_KIND];
    int fet_attr_kind_sum[ATTR_NUM-1][FET_ATTR_KIND];//初始化为0记得
    for(i=0; p!=NULL; i++)
    {
        if(p->attr[ATTR_NUM-1]!=dataSet->next->attr[ATTR_NUM-1])
        {
            signalflag=0;
        }
        len++;
        p=p->next;
    }
    if(signalflag||len==1)//只有一类样本
    {
        matrixtree[node].jc_attr=dataSet->next->attr[ATTR_NUM-1];//data->next指向当前样本集第一样本
        matrixtree[node].splitfet=-1;
        matrixtree[node].fetvalue=dataSet->next->attr[bbestFet];
        matrixtree[node].parent=bbestFet;
        matrixtree[node].len=len;
        matrixtree[node].id=node;
        node++;
        return matrixtree[node];
    }
    fetflag[ATTR_NUM-1]=0;
    for(i=0; i<ATTR_NUM-1; i++)
        fetflag[ATTR_NUM-1]+=fetflag[i];
    //cout<<"fetflaglast="<<fetflag[ATTR_NUM-1]<<endl;
    if(fetflag[ATTR_NUM-1]==ATTR_NUM-1)//所有属性都使用完了
    {
        matrixtree[node].jc_attr=dataSet->next->attr[ATTR_NUM-1];
        matrixtree[node].splitfet=-1;
        matrixtree[node].fetvalue=dataSet->next->attr[bbestFet];
        matrixtree[node].parent=bbestFet;
        matrixtree[node].len=len;
        matrixtree[node].id=node;
        //cout<<"fetflag=1   "<<matrixtree[node].jc_attr<<endl;
        node++;
        return matrixtree[node];
    }
    bestFet=chooseBestFectureTosplit(dataSet,0.01,2);
    if(bestFet==-1)//如果最大的信息增益小于预设的最小信息增益或者是子树中样本数小于预设的最小数也返回
    {
        matrixtree[node].jc_attr=dataSet->next->attr[ATTR_NUM-1];
        matrixtree[node].splitfet=-1;
        matrixtree[node].fetvalue=dataSet->next->attr[bbestFet];
        matrixtree[node].parent=bbestFet;
        matrixtree[node].len=len;
        matrixtree[node].id=node;
        //cout<<"fetflag=1   "<<matrixtree[node].jc_attr<<endl;
        node++;
        return matrixtree[node];
    }
    fetflag[bestFet]=1;//选出最好的分裂属性之后，在返回分裂之前，不必再考虑用该属性进行分裂
    string *attr_kind_sum=getkindattr(dataSet,bestFet);//获得当前分裂属性有多少种取值
    for(i=0; attr_kind_sum[i]!="0"; i++);
    int kind=i;//用于记录当前分裂属性有多少种取值
    for(j=0; j<kind; j++)//分裂后的子集按序建树，同样也可以先对样本多的子集先进行建树，方便决策的时候能更大概率的先找到匹配的
    {
        matrixtree[node]=dataToTree(splitDataSet(dataSet,bestFet,attr_kind_sum[j]),bestFet);
    }
    fetflag[bestFet]=0;//每次返回上一节点时，把之前分裂的属性从新回收，可以用作下次分裂
    matrixtree[node].splitfet=bestFet;
    matrixtree[node].fetvalue=dataSet->next->attr[bbestFet];
    matrixtree[node].parent=bbestFet;
    matrixtree[node].jc_attr="-1";
    matrixtree[node].len=len;
    matrixtree[node].kind=kind;
    matrixtree[node].id=node;
    node++;
    return matrixtree[node];
}
void loadData(data *dataSet)
{
    ifstream infile;
    string tmpstrline;
    string tmpstr;
    data *p;
    p=dataSet;
    data *datatmp;
    infile.open("data\\id3.txt",ios::in);
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
            while(input>>tmpstr)
            {
                datatmp->attr[j]=tmpstr;
                j++;
                fetlen=j;
            }
            i++;
            p->next=datatmp;
            p=p->next;
        }
    }
    //检测数据加载是否正确
    /*
    yblen=i;
    p=dataSet->next;
    for(i=0; p!=NULL; i++)
    {
        for(j=0; j<fetlen; j++)
        {
            cout<<p->attr[j]<<"  ";
        }
        p=p->next;
        cout<<endl;
    }*/
}
int createTree()
{
    int i,j;
    int len;
    matrixTree *child;
    matrixTree *p;
    for(i=0;i<node;i++)
    {
        if(matrixtree[i].splitfet==-1)
        {
            s.push(matrixtree[i]);
        }
        else
        {
            len=matrixtree[i].len;
            p=&matrixtree[i];
            while((len))
            {
                len-=s.top().len;
                child=new matrixTree;
                *child=s.top();
                s.pop();
                p->next=child;
                p=p->next;
            }
            s.push(matrixtree[i]);
        }
    }
    s.pop();
    return 0;
}
int main()
{
    data *dataSet=new data;
    dataSet->next=NULL;
    loadData(dataSet);
    dataToTree(dataSet,0);//初值很重要，当返回到根节点时，如取-1，则会无法访问根节点上的属性特征值
    //createTree();
    //第二个参数的取值即表示根节点的父节点
    for(int i=0; i<node; i++)
        cout<<"node="<<i<<"  splitfet="<<matrixtree[i].splitfet<<"  "<<"jc_attr="\
        <<matrixtree[i].jc_attr<<"  "<<"len="<<matrixtree[i].len<<"  parent="<<matrixtree[i].parent<<"  value="<<matrixtree[i].fetvalue<<endl;
    createTree();
    matrixTree *p;
    for(int i=0;i<node;i++)
    {
        p=&matrixtree[i];
        while(p!=NULL)
        {
            cout<<p->id<<"  ";
            p=p->next;
        }
        cout<<endl;
    }
    return 0;
}
