#include "MatrixOpe.h"
/***数据归一化处理，data[i][j]-min[j]/range[j]**/
int autoNorm(Matrix x)
{
    int j=0,i=0;

    Matrix minVals(1,x.col,0,"ss");
    Matrix maxVals(1,x.col,0,"ss");
    Matrix ranges(1,x.col,0,"ss");
    for(j=0; j<x.col; j++)
    {
        minVals.data[0][j]=x.data[0][j];
        maxVals.data[0][j]=x.data[0][j];
    }
    for(i=0; i<x.row; i++)
    {
        for(j=0; j<x.col; j++)
        {
            if(x.data[i][j]<minVals.data[0][j])
                minVals.data[0][j]=x.data[i][j];
            if(x.data[i][j]>maxVals.data[0][j])
                maxVals.data[0][j]=x.data[i][j];
        }
    }
    for(j=0; j<x.col; j++)
        ranges.data[0][j]=maxVals.data[0][j]-minVals.data[0][j];
    for(i=0; i<x.row; i++)
    {
        for(j=0; j<x.col; j++)
        {
            x.data[i][j]-=minVals.data[0][j];
            if(ranges.data[0][j]!=0)
                x.data[i][j]/=ranges.data[0][j];
        }
    }
    return 0;
}
/**计算每个测试样本与训练样本的距离，保存在distance矩阵中**/
Matrix cdistances(Matrix test,Matrix x)
{
    int i,j;
    Matrix distances(x.row,1,0,"ss");
    for(i=0; i<x.row; i++)
    {
        for(j=0; j<x.col; j++)
        {
            distances.data[i][0]+=pow((x.data[i][j]-test.data[0][j]),2);
        }
        distances.data[i][0]=sqrt(distances.data[i][0]);
        //cout<<"dis="<<distances.data[i][0]<<endl;
    }
    return distances;
}
/***选择出K个近邻**/
Matrix getK(Matrix oneTest,Matrix x,int K)
{
    int i,j,k;
    Matrix distances(x.row,1,0,"xx");//为每一个测试样本初始化k个近邻为前k个训练样本，并记录近邻的id
    distances=cdistances(oneTest,x);
    Matrix Kdistances(K,2,0,"x");
    double Max=-1;
    int Maxi=-1;
    for(i=0; i<K; i++)
    {
        Kdistances.data[i][0]=distances.data[i][0];
        Kdistances.data[i][1]=i;//记录近邻的id
        if(Kdistances.data[i][0]>Max)
        {
            Max=Kdistances.data[i][0];
            Maxi=i;//选出当前k个近邻中最大的一个
        }
    }
    //为每一个测试样本从第K个训练样本中遍历更新新的k个近邻
    for(i=K; i<x.row; i++)
    {
        if(distances.data[i][0]<Max)
        {
            Kdistances.data[Maxi][0]=distances.data[i][0];
            Max=distances.data[i][0];//暂时更新当前替换的距离为最大距离，因为已经不能用之前的最大距离了
            Kdistances.data[Maxi][1]=i;//记录近邻的id
            for(k=0; k<K; k++)
            {
                if(Kdistances.data[k][0]>Max)
                {
                    Max=Kdistances.data[k][0];
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
int classfiy(Matrix &testData,Matrix &testDatay,Matrix &x,Matrix &y,const int &K)
{
    int i,j,k;
    int sumz=0,sumf=0;
    Matrix knn(K,2,0,"s");
    autoNorm(x);
    autoNorm(testData);
    for(i=0; i<testData.row; i++)
    {
        sumz=0;
        sumf=0;
        Matrix oneTest;
        oneTest=testData.getOneRow(i);
        knn=getK(oneTest,x,K);
        for(j=0; j<K; j++)
        {

            if(y.data[int(knn.data[j][1])][0]==1)
                sumz++;
            else
                sumf++;
            cout<<y.data[int(knn.data[j][1])][0]<<"  ";
        }
        if(sumz>sumf)
            cout<<"juece="<<"1"<<"&"<<"shiji="<<testDatay.data[i][0]<<endl;
        else
            cout<<"juece="<<"-1"<<"&"<<"shiji="<<testDatay.data[i][0]<<endl;
    }
    return 0;
}
int KNN()
{
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\knn.txt";
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    x.print();
    y.print();
    Matrix testx;
    char testFile[20]="data\\knnTest.txt";
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    testx.LoadData(testFile);
    Matrix testy;
    testy=testx.getOneCol(testx.col-1);
    testx.deleteOneCol(testx.col-1);
    cout<<"000000000000000000"<<endl;
    classfiy(testx,testy,x,y,10);
    return 0;
}

