#include "LineReg.h"

namespace MLL{

    /**

    线性回归函数的实现，考虑一般的线性回归，最小平方和作为损失函数，则目标函数是一个无约束的凸二次规划问题，
    由凸二次规划问题的极小值在导数为0处取到，且极小值为全局最小值，且有闭式解。根据数学表达式实现矩阵之间的运算求得参数w。
    **/
    int LineReg::regression()
    {
        Matrix xT = _x.transposeMatrix();
        Matrix xTx = xTx.multsMatrix(xT,_x);
        Matrix xTx_1 = xTx.niMatrix();
        Matrix xTx_1xT = xTx_1xT.multsMatrix(xTx_1,xT);
        Matrix ws;
        ws=ws.multsMatrix(xTx_1xT,_y);
        std::cout<<"ws"<<std::endl;
        ws.print();
		Matrix pre_y = _x * ws; 
		for(int i = 0; i < _y._row; i++)
		{
			std::cout << "pre_y:" << pre_y._data[i][0] << "\t" << "fact_y:" << _y._data[i][0]<< "\t" << "error=" << pre_y._data[i][0] - _y._data[i][0] << std::endl;
		}
        return 0;
    }
    
	/**
    下面的岭回归函数只是在一般的线性回归函数的基础上在对角线上引入了岭的概念，不仅有解决矩阵不可逆的线性，同样也有正则项的目的，
    采用常用的二范数就得到了直接引入lam的形式。
    **/
    int LineReg::ridgeRegres()
    {
        Matrix xT= _x.transposeMatrix();
        Matrix xTx=xTx.multsMatrix(xT,_x);
		Matrix denom(xTx._row,xTx._col,_lamd,"diag");
        xTx=xTx.addMatrix(xTx,denom);
        Matrix xTx_1=xTx.niMatrix();
        Matrix xTx_1xT=xTx_1xT.multsMatrix(xTx_1,xT);
        Matrix ws=ws.multsMatrix(xTx_1xT,_y);
        std::cout<<"ws"<<std::endl;
        ws.print();
        return 0;
    }

    /**
    局部加权线性回归是在线性回归的基础上对每一个测试样本（训练的时候就是每一个训练样本）在其已有的样本进行一个加权拟合，
    权重的确定可以通过一个核来计算，常用的有高斯核（离测试样本越近，权重越大，反之越小），这样对每一个测试样本就得到了不一样的
    权重向量，所以最后得出的拟合曲线不再是线性的了，这样就增加的模型的复杂度来更好的拟合非线性数据。

    **/
    //需要注意的是局部加权线性回归是对每一个样本进行权重计算，所以对于每一个样本都有一个权重w，所以下面的函数只是局部线性回归的一个主要辅助函数
    Matrix LineReg::locWeightLineReg(Matrix &test)
    {
        Matrix w(_x._row,_x._row,0);
        double temp=0;
        int i = 0, j = 0;

        /**
        根据测试样本点与整个样本的距离已经选择的核确定局部加权矩阵，采用对角线上为局部加权值
        **/
        for(i = 0;i < _x._row;i++)
        {
            temp = 0;
            for(j = 0;j < _x._col;j++)
            {
                temp += (test._data[0][j] - _x._data[i][j]) * (test._data[0][j] - _x._data[i][j]);
            }
            w._data[i][i] = exp(temp / -2.0 *_k * _k);
        }
        Matrix xT = _x.transposeMatrix();
        Matrix wx = wx.multsMatrix(w,_x);
        Matrix xTwx;
        xTwx = xTwx.multsMatrix(xT,wx);
        Matrix xTwx_1;
        xTwx_1 = xTwx.niMatrix();
        Matrix xTwx_1xT;
        xTwx_1xT = xTwx_1xT.multsMatrix(xTwx_1,xT);
        Matrix xTwx_1xTw;
        xTwx_1xTw = xTwx_1xTw.multsMatrix(xTwx_1xT,w);
        Matrix ws = xTwx_1xTw * _y;
        return ws;
    }
    /**
    通过调用上面的局部加权线性回归函数实现对每一个测试样例确定最后的参数，同样以列向量的形式存储，对n个特征的样本就n行，对m个样本就一个m列，
    **/

    int LineReg::lwlrtest()
    {
        int i,j;
        Matrix test;
        Matrix wsOne;
        Matrix ws;
        ws = _x.copyMatrix();
        /**
        每一个样本单独调用局部加权线性回归
        **/
        for(i=0; i < _x._row;i++)///ws以列为一个权重参数向量，m列表示m个样本
        {
            test = _x.getOneRow(i);
            wsOne = locWeightLineReg(test);
            for(j = 0; j < _x._col; j++)
            {
                ws._data[i][j] = wsOne._data[0][j];
            }
        }
        for(i = 0; i < ws._row; i++)
        {
            for(j = 0; j < ws._col; j++)
            {
                std::cout<<ws._data[j][i]<<"  ";
            }
            std::cout<<std::endl;
        }

        /**验证局部加权线性回归的正确性*/
        double yy;
        for(i = 0; i < ws._row;i++)
        {
            yy = 0;
            for(j = 0; j < ws._col; j++)
            {
                yy += ws._data[j][i] * _x._data[i][j];
            }
            std::cout<<"y="<<yy<<std::endl;
        }
        return 0;
    }

    LineReg::LineReg(const std::string &file, const std::string &model, const double &lamd, const double &k)
    {
        _x.init_by_data(file);
        _y = _x.getOneCol(_x._col-1);
        _x.deleteOneCol(_x._col-1);
		std::cout << "x:" << _x._row << "&" << _x._col << std::endl;
		std::cout << "y:" <<  _y._row << "&" << _y._col << std::endl;
        _lamd = lamd;
        _k = k; 
    }
}
