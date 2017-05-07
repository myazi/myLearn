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

线性回归函数的实现，考虑一般的线性回归，最小平方和作为损失函数，则目标函数是一个无约束的凸二次规划问题，
由凸二次规划问题的极小值在导数为0处取到，且极小值为全局最小值，且有闭式解。根据数学表达式实现矩阵之间的运算求得参数w。
**/
int regression(Matrix x,Matrix y)
{
    int i=0;
    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    x.transposematrix(x,&xT);
    //xT.print(xT);

    Matrix xTx;
    xTx.initMatrix(&xTx,xT.col,x.row);
    xTx.multsmatrix(&xTx,xT,x);///x的转置乘上x
    //xTx.print(xTx);

    Matrix xTx_1;
    xTx_1.initMatrix(&xTx_1,xTx.row,xTx.col);
    xTx_1.nimatrix(&xTx_1,xTx);///x的转置乘上x之后求逆
    //xTx_1.print(xTx_1);

    Matrix xTx_1xT;
    xTx_1xT.initMatrix(&xTx_1xT,xTx_1.col,xT.row);
    xTx_1xT.multsmatrix(&xTx_1xT,xTx_1,xT);///逆再乘上x的转置
    //xTx_1.print(xTx_1xT);

    Matrix ws;
    ws.initMatrix(&ws,xTx_1xT.col,y.row);
    ws.multsmatrix(&ws,xTx_1xT,y);///再乘上y就是权重权重向量，而且是一个列向量

    cout<<"ws"<<endl;
    ws.print(ws);
    return 0;
}
/**
局部加权线性回归是在线性回归的基础上对每一个测试样本（训练的时候就是每一个训练样本）在其已有的样本进行一个加权拟合，
权重的确定可以通过一个核来计算，常用的有高斯核（离测试样本越近，权重越大，反之越小），这样对每一个测试样本就得到了不一样的
权重向量，所以最后得出的拟合曲线不再是线性的了，这样就增加的模型的复杂度来更好的拟合非线性数据。

**/
//需要注意的是局部加权线性回归是对每一个样本进行权重计算，所以对于每一个样本都有一个权重w，所以下面的函数只是局部线性回归的一个主要辅助函数
Matrix locWeightLineReg(Matrix test,Matrix x,Matrix y,double k)
{
    Matrix w;
    w.initMatrix(&w,x.col,x.col);
    double temp=0;
    int i,j;

    /**
    根据测试样本点与整个样本的距离已经选择的核确定局部加权矩阵，采用对角线上为局部加权值
    **/
    for(i=0;i<x.col;i++)
    {
        temp=0;
        for(j=0;j<x.row;j++)
        {
            temp+=(test.mat[0][j]-x.mat[i][j])*(test.mat[0][j]-x.mat[i][j]);
        }
        w.mat[i][i]=exp(temp/-2.0*k*k);
    }

    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    xT.transposematrix(x,&xT);

    Matrix wx;
    wx.initMatrix(&wx,w.col,x.row);
    wx.multsmatrix(&wx,w,x);

    Matrix xTwx;
    xTwx.initMatrix(&xTwx,xT.col,x.row);
    xTwx.multsmatrix(&xTwx,xT,wx);

    Matrix xTwx_1;
    xTwx_1.initMatrix(&xTwx_1,xTwx.row,xTwx.col);
    xTwx_1.nimatrix(&xTwx_1,xTwx);

    //xTwx_1.print(xTwx_1);

    Matrix xTwx_1xT;
    xTwx_1xT.initMatrix(&xTwx_1xT,xTwx_1.col,xT.row);
    xTwx_1xT.multsmatrix(&xTwx_1xT,xTwx_1,xT);

    Matrix xTwx_1xTw;
    xTwx_1xTw.initMatrix(&xTwx_1xTw,xTwx_1xT.col,w.row);
    xTwx_1xTw.multsmatrix(&xTwx_1xTw,xTwx_1xT,w);

    Matrix ws;
    ws.initMatrix(&ws,xTwx_1xTw.col,y.row);
    ws.multsmatrix(&ws,xTwx_1xTw,y);

    cout<<"ws"<<endl;
    ws.print(ws);
    return ws;
}
/**
通过调用上面的局部加权线性回归函数实现对每一个测试样例确定最后的参数，同样以列向量的形式存储，对n个特征的样本就n行，对m个样本就一个m列，
**/

int lwlrtest(Matrix x,Matrix y,double k)
{
    int i,j;
    Matrix test;
    test.initMatrix(&test,1,x.row);
    Matrix wsOne;
    wsOne.initMatrix(&wsOne,x.row,1);
    Matrix ws;
    ws.initMatrix(&ws,x.row,x.col);

    /**
    每一个样本单独调用局部加权线性回归
    **/
    for(i=0;i<ws.row;i++)///ws以列为一个权重参数向量，m列表示m个样本
    {
        test=test.getOneCol(x,i);
        wsOne=locWeightLineReg(test,x,y,k);
        for(j=0;j<ws.col;j++)
        {
            ws.mat[j][i]=wsOne.mat[j][0];
        }
    }
    for(i=0;i<ws.row;i++)
    {
        for(j=0;j<ws.col;j++)
        {
            cout<<ws.mat[j][i]<<"  ";
        }
        cout<<endl;
    }

    /**验证局部加权线性回归的正确性*/
    double yy;
    for(i=0;i<ws.row;i++)
    {
        yy=0;
        for(j=0;j<ws.col;j++)
        {
            yy+=ws.mat[j][i]*x.mat[i][j];
        }
        cout<<"y="<<yy<<endl;
    }
    return 0;
}
/**
下面的岭回归函数只是在一般的线性回归函数的基础上在对角线上引入了岭的概念，不仅有解决矩阵不可逆的线性，同样也有正则项的目的，
采用常用的二范数就得到了直接引入lam的形式。
**/

int ridgeRegres(Matrix x,Matrix y,double lam)
{
    int i=0;
    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    x.transposematrix(x,&xT);
    xT.print(xT);

    Matrix xTx;
    xTx.initMatrix(&xTx,xT.col,x.row);
    xTx.multsmatrix(&xTx,xT,x);
    xTx.print(xTx);

    Matrix denom;
    denom.initMatrix(&denom,xTx.col,xTx.row,lam);
    xTx.addmatrix(&xTx,xTx,denom);
    xTx.print(xTx);

    Matrix xTx_1;
    xTx_1.initMatrix(&xTx_1,xTx.row,xTx.col);
    xTx_1.nimatrix(&xTx_1,xTx);

    Matrix xTx_1xT;
    xTx_1xT.initMatrix(&xTx_1xT,xTx_1.col,xT.row);
    xTx_1xT.multsmatrix(&xTx_1xT,xTx_1,xT);

    Matrix ws;
    ws.initMatrix(&ws,xTx_1xT.col,y.row);
    ws.multsmatrix(&ws,xTx_1xT,y);

    cout<<"ws"<<endl;
    ws.print(ws);
    return 0;
}
int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\lineReg.txt";
    dtm.loadData(&dtm,file);
    dtm.print(dtm);

    cout<<"x,y"<<endl;
    cout<<"----------------------"<<endl;
    Matrix x;
    x.loadMatrix(&x,dtm);
    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);
    y.print(y);
    x.deleteOneRow(&x,x.row);
    x.print(x);

    regression(x,y);///调用线性回归函数
    //lwlrtest(x,y,1.0);///调用局部加权线性回归函数
    //ridgeRegres(x,y,0.1);///调用岭回归函数
    return 0;
}
