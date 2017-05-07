#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "matrix.h"
using namespace std;

/***数据归一化处理，data[i][j]-min[j]/range[j]**/
int autoNorm(Matrix x)
{
    int j=0,i=0;

    Matrix minVals;
    minVals.initMatrix(&minVals,1,x.row,MAX);

    Matrix maxVals;
    maxVals.initMatrix(&maxVals,1,x.row,MIN);

    Matrix ranges;
    ranges.initMatrix(&ranges,1,x.row);

    for(j=0; j<x.row; j++)
    {
        minVals.mat[0][j]=x.mat[0][j];
        maxVals.mat[0][j]=minVals.mat[0][j];
    }
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            if(x.mat[i][j]<minVals.mat[0][j])
                minVals.mat[0][j]=x.mat[i][j];
            if(x.mat[i][j]>maxVals.mat[0][j])
                maxVals.mat[0][j]=x.mat[i][j];
        }
    }
    for(i=0; i<x.row; i++)
        ranges.mat[0][i]=maxVals.mat[0][i]-minVals.mat[0][i];
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            x.mat[i][j]-=minVals.mat[0][j];
            x.mat[i][j]/=ranges.mat[0][j];
            //cout<<x.mat[i][j]<<"  ";
        }
        //cout<<endl;
    }
    return 0;
}
/**计算每个测试样本与训练样本的距离，保存在distance矩阵中**/
Matrix cdistances(Matrix test,Matrix x)
{
    int i,j;
    Matrix distances;
    distances.initMatrix(&distances,x.col,1,0);
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            distances.mat[i][0]+=pow((x.mat[i][j]-test.mat[0][j]),2);
        }
        distances.mat[i][0]=sqrt(distances.mat[i][0]);
        //cout<<"dis="<<distances.mat[i][0]<<endl;
    }
    return distances;
}
/***选择出K个近邻**/
Matrix getK(Matrix oneTest,Matrix x,int K)
{
    int i,j,k;
    //为每一个测试样本初始化k个近邻为前k个训练样本，并记录近邻的id
    Matrix distances;
    distances.initMatrix(&distances,x.col,1);
    distances=cdistances(oneTest,x);

    Matrix Kdistances;
    Kdistances.initMatrix(&Kdistances,K,2);
    double Max=-1;
    int Maxi=-1;
    for(i=0; i<K; i++)
    {
        Kdistances.mat[i][0]=distances.mat[i][0];
        Kdistances.mat[i][1]=i;//记录近邻的id
        if(Kdistances.mat[i][0]>Max)
        {
            Max=Kdistances.mat[i][0];
            Maxi=i;//选出当前k个近邻中最大的一个
        }
    }
    //为每一个测试样本从第K个训练样本中遍历更新新的k个近邻
    for(i=K; i<x.col; i++)
    {
        if(distances.mat[i][0]<Max)
        {
            Kdistances.mat[Maxi][0]=distances.mat[i][0];
            Max=distances.mat[i][0];//暂时更新当前替换的距离为最大距离，因为已经不能用之前的最大距离了
            Kdistances.mat[Maxi][1]=i;//记录近邻的id
            for(k=0; k<K; k++)
            {
                if(Kdistances.mat[k][0]>Max)
                {
                    Max=Kdistances.mat[k][0];
                    Maxi=k;//选出当前k个近邻中最大的一个
                }
            }
        }
    }
    return Kdistances;
}
//KNN决规则，这里采用平局权重投票，且不考虑样本不均衡问题
/**
1，knn分类决策函数，首先对训练数据和测试数据进行归一化，
2，计算每一个测试样本到m个训练样本的距离
3，从m个距离中选出最小的k个距离，并记录这k个最小距离的样本
4，由k个样本加权投票得到最终的决策类别

***/
int classfiy(Matrix testData,Matrix testDatay,Matrix x,Matrix y,int K)
{
    int i,j,k;
    int sumz=0,sumf=0;
    Matrix knn;
    knn.initMatrix(&knn,K,2);

    autoNorm(x);
    autoNorm(testData);
    for(i=0; i<testData.col; i++)
    {
        sumz=0;
        sumf=0;

        Matrix oneTest;
        oneTest.initMatrix(&oneTest,1,x.row);
        oneTest=oneTest.getOneCol(testData,i);

        knn=getK(oneTest,x,10);

        for(j=0; j<K; j++)
        {

            if(y.mat[int(knn.mat[j][1])][0]==1)
                sumz++;
            else
                sumf++;
            cout<<y.mat[int(knn.mat[j][1])][0]<<"  ";
        }
        if(sumz>sumf)
            cout<<"juece="<<"1"<<"&"<<"shiji="<<testDatay.mat[i][0]<<endl;
        else
            cout<<"juece="<<"-1"<<"&"<<"shiji="<<testDatay.mat[i][0]<<endl;
    }
    return 0;
}
int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\knn.txt";
    dtm.loadData(&dtm,file);
    //dtm.print(dtm);

    Matrix x;
    x.loadMatrix(&x,dtm);

    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);

    x.deleteOneRow(&x,x.row);


    dataToMatrix dtmTest;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char testFile[20]="data\\knnTest.txt";
    dtmTest.loadData(&dtmTest,testFile);
    //dtmTest.print(dtmTest);

    Matrix testData;
    testData.loadMatrix(&testData,dtmTest);

    Matrix testDatay;
    testDatay.initMatrix(&testDatay,x.col,1);
    testDatay=testDatay.getOneRow(testData,testData.row);

    testData.deleteOneRow(&testData,testData.row);


    classfiy(testData,testDatay,x,y,10);
    return 0;
}
