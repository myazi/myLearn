#include "LineReg.h"
/**

线性回归函数的实现，考虑一般的线性回归，最小平方和作为损失函数，则目标函数是一个无约束的凸二次规划问题，
由凸二次规划问题的极小值在导数为0处取到，且极小值为全局最小值，且有闭式解。根据数学表达式实现矩阵之间的运算求得参数w。
**/
int regression(Matrix x,Matrix y)
{
    Matrix xT=x.transposeMatrix();
    Matrix xTx=xTx.multsMatrix(xT,x);
    Matrix xTx_1=xTx.niMatrix();
    Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
    Matrix ws;
    ws=ws.multsMatrix(xTx_1xT,y);
    cout<<"ws"<<endl;
    ws.print();
    return 0;
}
/**
下面的岭回归函数只是在一般的线性回归函数的基础上在对角线上引入了岭的概念，不仅有解决矩阵不可逆的线性，同样也有正则项的目的，
采用常用的二范数就得到了直接引入lam的形式。
**/

int ridgeRegres(Matrix x,Matrix y,double lam)
{
    Matrix xT=x.transposeMatrix();
    Matrix xTx=xTx.multsMatrix(xT,x);
    Matrix denom(xTx.row,xTx.col,lam,"diag");
    xTx=xTx.addMatrix(xTx,denom);
    Matrix xTx_1=xTx.niMatrix();
    Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
    Matrix ws=ws.multsMatrix(xTx_1xT,y);
    cout<<"ws"<<endl;
    ws.print();
    return 0;
}

/**
局部加权线性回归是在线性回归的基础上对每一个测试样本（训练的时候就是每一个训练样本）在其已有的样本进行一个加权拟合，
权重的确定可以通过一个核来计算，常用的有高斯核（离测试样本越近，权重越大，反之越小），这样对每一个测试样本就得到了不一样的
权重向量，所以最后得出的拟合曲线不再是线性的了，这样就增加的模型的复杂度来更好的拟合非线性数据。

**/
//需要注意的是局部加权线性回归是对每一个样本进行权重计算，所以对于每一个样本都有一个权重w，所以下面的函数只是局部线性回归的一个主要辅助函数
Matrix locWeightLineReg(Matrix test,Matrix x,Matrix y,const double &k)
{
    Matrix w(x.row,x.row,0,"T");
    double temp=0;
    int i,j;

    /**
    根据测试样本点与整个样本的距离已经选择的核确定局部加权矩阵，采用对角线上为局部加权值
    **/
    for(i=0;i<x.row;i++)
    {
        temp=0;
        for(j=0;j<x.col;j++)
        {
            temp+=(test.data[0][j]-x.data[i][j])*(test.data[0][j]-x.data[i][j]);
        }
        w.data[i][i]=exp(temp/-2.0*k*k);
    }
    Matrix xT=x.transposeMatrix();
    Matrix wx=wx.multsMatrix(w,x);
    Matrix xTwx;
    xTwx=xTwx.multsMatrix(xT,wx);
    Matrix xTwx_1;
    xTwx_1=xTwx.niMatrix();
    Matrix xTwx_1xT;
    xTwx_1xT=xTwx_1xT.multsMatrix(xTwx_1,xT);
    Matrix xTwx_1xTw;
    xTwx_1xTw=xTwx_1xTw.multsMatrix(xTwx_1xT,w);
    Matrix ws = xTwx_1xTw * y;
    return ws;
}
/**
通过调用上面的局部加权线性回归函数实现对每一个测试样例确定最后的参数，同样以列向量的形式存储，对n个特征的样本就n行，对m个样本就一个m列，
**/

int lwlrtest(Matrix x,Matrix y,const double &k)
{
    int i,j;
    Matrix test;
    Matrix wsOne;
    Matrix ws;
    ws=x.copyMatrix();
    /**
    每一个样本单独调用局部加权线性回归
    **/
    for(i=0;i<x.row;i++)///ws以列为一个权重参数向量，m列表示m个样本
    {
        test=x.getOneRow(i);
        wsOne=locWeightLineReg(test,x,y,k);
        for(j=0;j<x.col;j++)
        {
            ws.data[i][j]=wsOne.data[0][j];
        }
    }
    for(i=0;i<ws.row;i++)
    {
        for(j=0;j<ws.col;j++)
        {
            cout<<ws.data[j][i]<<"  ";
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
            yy+=ws.data[j][i]*x.data[i][j];
        }
        cout<<"y="<<yy<<endl;
    }
    return 0;
}

int LineReg(const char *file, const string &model, const double arg)
{
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    if(model=="regression")
    {
        regression(x,y);
    }
    if(model=="ridgeRegres")
    {
        ridgeRegres(x,y,0.01);
    }
    if(model=="lwlrtest")
    {
        lwlrtest(x,y,1);
    }
    return 0;
}
