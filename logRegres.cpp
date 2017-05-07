#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "matrix.h"
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;
#define MAX 1000000
#define MIN -100000

/**
sigmad函数,这里没有直接将预测类别转换为整型，而是还是返回一个double值
*/
double sigmoid(double z)
{
    return 1.0/(1+exp(-z));
}
/**
梯度下降算法，主要是确定负梯度方向，步长，采用迭代的思想迭代n至收敛，
当目标函数是凸规划问题，那么局部最小值就是全局最小值

**/
int gradAscent(Matrix x,Matrix y)
{
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);///初始化优化参数向量为1

    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    xT.transposematrix(x,&xT);

    Matrix z;
    z.initMatrix(&z,x.col,1);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.001;///迭代步长
    double error;///记录错误率
    int k,c=0;
    int i,j;
    for(c=0; c<1000; c++)
    {
        z.multsmatrix(&z,x,weights);
        for(i=0; i<x.col; i++)
            z.mat[i][0]=sigmoid(z.mat[i][0]);///预测类别
        z.submatrix(&z,y,z);///计算负梯度方向，同时可以作为预测类别与真实类别的误差
        error=0;
        for(k=0; k<x.col; k++)///统计错误率
            error+=z.mat[k][0];
        cout<<"error="<<error<<endl;
        if(error<2&&error>-2)///设置错误率小于一定值时退出迭代
            break;
        w1.multsmatrix(&w1,xT,z);///计算负梯度方向
        for(j=0; j<x.row; j++)
            w1.mat[j][0]*=alpha;///负梯度方向与步长的乘积确定迭代值
        weights.addmatrix(&weights,weights,w1);///往负梯度方向走一个步长

    }
    /**
    验证算法的正确性
    **/
    Matrix test;
    test.initMatrix(&test,y.col,y.row);
    test.multsmatrix(&test,x,weights);
    cout<<"test"<<endl;
    for(i=0; i<y.col; i++)
    {
        if(test.mat[i][0]>0)
            cout<<1-y.mat[i][0]<<endl;
        else
            cout<<0-y.mat[i][0]<<endl;
    }
}
/**
随机梯度下降与梯度下降法不同的是在负梯度方向的确定，梯度下降是根据所有的样本来确定负梯度方向，
而随机梯度下降每次只看一个样本点来确定负梯度方向，虽然不完全可信，但随着迭代次数增加，同样收敛

**/
int stoGradAscent(Matrix x,Matrix y)//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
{
    int i,j,c=0;
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);

    Matrix z;
    z.initMatrix(&z,1,1);

    Matrix xOne;
    xOne.initMatrix(&xOne,1,x.row);

    Matrix xOneT;
    xOneT.initMatrix(&xOneT,xOne.row,xOne.col);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.01;///步长
    double error;
    for(c=0; c<500; c++)
    {
        for(i=0; i<x.col; i++)
        {
            xOne=xOne.getOneCol(x,i);///随机选择一个样本点，这里没有作随机选择，而是按序选择
            z.multsmatrix(&z,xOne,weights);
            z.mat[0][0]=sigmoid(z.mat[0][0]);
            z.mat[0][0]=y.mat[i][0]-z.mat[0][0];
            xOneT.transposematrix(xOne,&xOneT);
            w1.multsmatrix(&w1,xOneT,z);///根据一样样本的预测误差来确定负梯度方向
            for(j=0; j<w1.row; j++)
                w1.mat[j][0]*=alpha;
            weights.addmatrix(&weights,weights,w1);///迭代
        }
        cout<<"weights"<<endl;
        weights.print(weights);
    }
    /**
    验证算法的正确性
    */
    Matrix test;
        test.initMatrix(&test,y.col,y.row);
        test.multsmatrix(&test,x,weights);
        for(i=0; i<y.col; i++)
        {
            if(test.mat[i][0]>0)
                cout<<1-y.mat[i][0]<<endl;
            else
                cout<<0-y.mat[i][0]<<endl;
        }
}

/**
逻辑回归，这里主要考虑其常用的两种求参数算法，一种是梯度下降，一种是随机梯度下降

*/

int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\logReg.txt";
    dtm.loadData(&dtm,file);

    Matrix x;
    x.loadMatrix(&x,dtm);

    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);

    x.deleteOneRow(&x,x.row);

    //gradAscent(x,y);
    stoGradAscent(x,y);
    return 0;
}
