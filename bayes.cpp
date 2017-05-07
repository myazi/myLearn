#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#include "matrix.h"
#define EMAILSUM 50 //非常特别的一个常量，适用范围有限
#define FILE_MAX_NUM 99
#define FILE_NAME_LEN 30
#define FILE_WORD_LEN 1000
#define CLASS_SUM 2
#define VEC_LEN 1000
using namespace std;

struct Bayes
{
    Matrix pXY;
    Matrix pY;
    Matrix pX;
};
Bayes bayes;//保存bayes分类器所有的参数
char file[CLASS_SUM][FILE_MAX_NUM][FILE_NAME_LEN];//加载数据的文件名

/**
生成单词字典
*/
dataToMatrix createVocabList(dataToMatrix *dtm)
{
    int i,j,k,vl;
    int vec_len=0;
    string dic[VEC_LEN];
    Data *p;
    for(k=0; k<EMAILSUM; k++)
    {
        for(i=0; i<dtm[k].col; i++)
        {
            p=dtm[k].dataSet->next;
            for(j=0; j<dtm[k].row; j++)
            {
                for(vl=0; vl<vec_len; vl++)
                {
                    if(!p->attr_string[j].compare(dic[vl]))
                        break;
                }
                if(vl==vec_len)
                {
                    dic[vec_len]=p->attr_string[j];
                    vec_len++;
                }
            }
            p=p->next;
        }
    }
    dataToMatrix dicMat;
    dicMat.dataSet=new Data;
    dicMat.dataSet->next=NULL;
    p=dicMat.dataSet;
    Data *datatmp=new Data;
    datatmp->next=NULL;
    for(i=0; i<vec_len; i++)
    {
        datatmp->attr_string[i]=dic[i];
    }
    p->next=datatmp;
    dicMat.col=1;
    dicMat.row=vec_len;
    cout<<"vec_len="<<vec_len<<endl;
    return dicMat;
}
/**
生成训练样本矩阵
**/
Matrix createFectVec(dataToMatrix *dtm,dataToMatrix dicMat)
{
    int i,j,k=0,vl;
    Data *p;
    Data *q;
    q=dicMat.dataSet->next;//字典同样也是一行
    Matrix vecX;
    vecX.initMatrix(&vecX,EMAILSUM,dicMat.row);
    for(k=0; k<EMAILSUM; k++)
    {
        for(i=0; i<dtm[k].col; i++)//只有一行
        {
            p=dtm[k].dataSet->next;
            for(j=0; j<dtm[k].row; j++)
            {
                for(vl=0; vl<dicMat.row; vl++)
                {
                    if(!p->attr_string[j].compare(q->attr_string[vl]))
                    {
                        vecX.mat[k][vl]=1;
                        break;
                    }
                }
            }
        }
    }
    /*
    for(i=0;i<vecX.col;i++)
    {
        for(j=0;j<vecX.row;j++)
            cout<<vecX.mat[i][j]<<"  ";
        cout<<endl;
    }*/
    return vecX;
}

/**
生成测试样本矩阵向量
**/
Matrix createTestVec(dataToMatrix testX,dataToMatrix dicMat)
{
    int i,j,vl;
    Data *p;
    Data *q;
    p=testX.dataSet->next;
    q=dicMat.dataSet->next;//字典同样也是一行
    Matrix testVecX;
    testVecX.initMatrix(&testVecX,testX.col,dicMat.row);
    for(i=0; i<testX.col; i++)//只有一行
    {
        for(j=0; j<testX.row; j++)
        {
            for(vl=0; vl<dicMat.row; vl++)
            {
                if(!p->attr_string[j].compare(q->attr_string[vl]))
                {
                    testVecX.mat[0][vl]=1;
                    break;
                }
            }
        }
    }
    return testVecX;
}

/**
训练函数的实现，注意对参数进行平滑处理
**/
int trainNB(Matrix vecX,Matrix vecY)
{
    bayes.pY.initMatrix(&bayes.pY,2,1);//两类初始化为2行的列向量
    bayes.pX.initMatrix(&bayes.pX,1,vecX.row);
    bayes.pXY.initMatrix(&bayes.pXY,2,vecX.row);

    double dicY[CLASS_SUM]= {0,0};
    int i,j,k,len=0;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(j=0; j<vecX.row; j++)
        {
            bayes.pXY.mat[k][j]=1;//平滑处理,默认出现一次，后期归一化时把特征向量的长度也考虑进去,这里的平滑是指每一类字典与整个字典的，未涉及测试
            //样本中的未登入词
            bayes.pX.mat[0][j]=0;
        }
    }
    int wordsum=0;
    for(i=0; i<vecX.col; i++)
    {
        if(vecY.mat[i][0]==1)
        {
            bayes.pY.mat[0][0]++;
            for(j=0; j<vecX.row; j++)
            {
                bayes.pXY.mat[0][j]+=vecX.mat[i][j];
                dicY[0]+=vecX.mat[i][j];
                wordsum+=vecX.mat[i][j];
                bayes.pX.mat[0][j]+=vecX.mat[i][j];
            }
        }
        else
        {
            bayes.pY.mat[1][0]++;
            for(j=0; j<vecX.row; j++)
            {
                bayes.pXY.mat[1][j]+=vecX.mat[i][j];
                dicY[1]+=vecX.mat[i][j];
                wordsum+=vecX.mat[i][j];
                bayes.pX.mat[0][j]+=vecX.mat[i][j];
            }
        }
    }
    //计算出PY两类的概率
    for(i=0; i<CLASS_SUM; i++)
    {
        bayes.pY.mat[i][0]/=vecX.col;
    }

    double s=0;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<vecX.row; i++)
        {
            bayes.pXY.mat[k][i]/=dicY[k]+vecX.row;
            s+=bayes.pXY.mat[k][i];
        }
    }
    double ss=0;
    for(i=0; i<vecX.row; i++)
    {
        bayes.pX.mat[0][i]/=wordsum;//+vec_len;
        ss+=bayes.pX.mat[0][i];
    }
    /*
    cout<<"pY="<<bayes.pY.mat[0][0]<<endl;
    cout<<"dicy"<<dicY[0]<<"  "<<dicY[1]<<endl;
    cout<<"wordsum="<<wordsum<<endl;
    cout<<"ss="<<ss<<endl;
    cout<<"s="<<s<<endl;*/
}

/**
分类决策函数
**/
int classifyNB(Matrix testVecX)
{
    double p0=1.0,p1=1.0;
    int i,j;
    for(i=0; i<testVecX.row ; i++)
    {
        if(testVecX.mat[0][i]!=0)
        {
            p0*=(bayes.pXY.mat[0][i]/bayes.pX.mat[0][i]);
            p1*=(bayes.pXY.mat[1][i]/bayes.pX.mat[0][i]);
            cout<<p0<<endl;
            cout<<p1<<endl;
        }
    }
    p0*=bayes.pY.mat[0][0];
    p1*=bayes.pY.mat[1][0];
    cout<<"p0="<<p0<<"&"<<"p1="<<p1<<endl;
    if(p0<p1)
        cout<<"class="<<1;
    else
        cout<<"class="<<0;
    return 0;

}

/**
    生成数据的文件名
*/
void getFileName()
{
    int i=0,j=0,k=0,l=0;
    string path[2]= {"data\\email\\ham\\*.txt","data\\email\\spam\\*.txt"};
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<25&&i<FILE_MAX_NUM; i++)
        {
            for(j=0,l=0; j<FILE_NAME_LEN; j++,l++)
            {
                if(path[k][j]!='*')
                    file[k][i][j]=path[k][l];
                else
                {
                    if((i+1)/10>0)
                    {
                        file[k][i][j]=48+((i+1)/10);
                        file[k][i][j+1]=48+(i+1)%10;
                        j++;
                    }
                    else
                    {
                        file[k][i][j]=48+i+1;
                    }
                }
            }
            //cout<<file[k][i]<<"  "<<i<<"  "<<i/10<<endl;
        }
    }
}

int main()
{

    int k,i,j;
    getFileName();///调用训练数据文件名生成函数

    /**
    加载数据样本
    **/
    dataToMatrix dtm[EMAILSUM];
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<25&&i<FILE_MAX_NUM; i++)
        {
            dtm[25*k+i].loadData(&dtm[25*k+i],file[k][i],1);
        }
    }

    dataToMatrix dicMat;// 字典
    dicMat=createVocabList(dtm);
    dicMat.print(dicMat);

    /**
    生成样本矩阵
    **/
    Matrix vecX;
    vecX.initMatrix(&vecX,EMAILSUM,dicMat.row);
    vecX=createFectVec(dtm,dicMat);

    /**
    生成样本标签
    **/
    Matrix vecY;
    vecY.initMatrix(&vecY,EMAILSUM,1);
    for(i=0; i<EMAILSUM; i++)
    {
        if(i<25)
            vecY.mat[i][0]=1;
        else
            vecY.mat[i][0]=-1;
    }

    trainNB(vecX,vecY);///通过样本矩阵和标签矩阵调用参数求解函数求解bayes决策的参数

    /**
    生成测试样本
    */
    dataToMatrix dtmTest;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char testfile[30]="data\\email\\test.txt";
    dtmTest.loadData(&dtmTest,testfile,1);

    /**
    生成测试样本矩阵向量
    **/
    Matrix testVecX;
    testVecX.initMatrix(&testVecX,1,dicMat.row);
    testVecX=createTestVec(dtmTest,dicMat);
    testVecX.print(testVecX);

    classifyNB(testVecX);///分类决策
}
