#include "SoftMaxReg.h"

namespace MLL {
    /**
    梯度下降算法，主要是确定负梯度方向，步长，采用迭代的思想迭代n至收敛，
    当目标函数是凸规划问题，那么局部最小值就是全局最小值

    **/
    int SoftMaxReg::gradAscent_SoftMax()
    {
        Matrix weights(_x._col,_y._col,0.1);
        Matrix xT = _x.transposeMatrix();

        int it = 0;
        int i = 0, j = 0;
        Matrix z(_y._row, _y._col, 0);//最好确定矩阵的大小
        Matrix z_rowSum(z._row, 1, 0);//各个维度求和，归一化为概率形式
        Matrix grad(_x._col, _y._col, 0);
        for(it = 0; it < _iter; it++)
        {
            z = _x * weights;
            for(i = 0; i < z._row; i++)
            {
                z_rowSum._data[i][0] = 0;
                for(j = 0; j < z._col; j++)
                {
					MLL::sigmoid(z._data[i][j]);
                    z_rowSum._data[i][0] += z._data[i][j];//求和
                }
                for(j = 0; j < z._col; j++)
                {
                    z._data[i][j] /= z_rowSum._data[i][0];//归一化
                    //if(it % 1000 == 0)
                        //std::cout<<z._data[i][j] <<"  ";
                }
                //if(iter%5000==0)
                    //std::cout<<std::endl;
            }
            z = _y - z;

            grad = xT * z;///计算负梯度方向
            //grad = (1.0/x._row) * grad;
            for(i = 0; i < grad._row; i++)
            {
                for(j = 0; j < grad._col; j++)
                {
                    grad._data[i][j] *= _alpha;///负梯度方向与步长的乘积确定迭代值
                }
            }
            weights = weights + grad;///往负梯度方向走一个步长
            //weights.print();
        }
        /**
        验证算法的正确性
        **/
        Matrix test=_x * weights;
        std::cout<<"test"<<std::endl;
        for(i = 0; i < _y._row; i++)
        {
			int label = -1;
            for(j = 0; j < _y._col; j++)
			{
				if(_y._data[i][j] == 1)
				{
					label = j;
					break;
				}
			}
            //仅适用于二维one-hot
			if(test._data[i][0] > test._data[i][1])
			{
				std::cout<<"pre_label: 0" << "\t" << "fact_label: " << label <<std::endl;
			}
            else
			{
				std::cout<<"pre_label: 1" << "\t" << "fact_label: " << label <<std::endl;
			}
        }
    }
    /**
    随机梯度下降与梯度下降法不同的是在负梯度方向的确定，梯度下降是根据所有的样本来确定负梯度方向，
    而随机梯度下降每次只看一个样本点来确定负梯度方向，虽然不完全可信，但随着迭代次数增加，同样收敛

    **/
    int SoftMaxReg::stoGradAscent_SoftMax()//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
    {
        Matrix xOne_row(1,_x._col,0);
        Matrix xOne_rowT(_x._col,1,0);

        Matrix weights(_x._col,_y._col,0.1);
        Matrix z(1,_y._col,0);//最好确定矩阵的大小
        Matrix grad(_x._col,_y._col,0);
        double z_rowSum=0;
        int i = 0, j = 0, k = 0, it = 0;
        for(it = 0; it < _iter; it++)
        {
            for(i = 0; i < _x._row; i++)
            {
                xOne_row=_x.getOneRow(i);///随机选择一个样本点，这里没有作随机选择，而是按序选择
                z = xOne_row * weights;
                z_rowSum=0;
                for(j=0;j<z._col;j++)
                {
                    sigmoid(z._data[0][j]);
                    z_rowSum+=z._data[0][j];//求和
                }
                for(j=0;j<z._col;j++)
                {
                    z._data[0][j]/=z_rowSum;//归一化
                    //if(it%1000==0)
                        //std::cout<<z._data[0][j] <<"  ";
                }
                //if(it%1000==0)
                    //std::cout<<std::endl;
                for(j=0;j<_y._col;j++)
                {
                    z._data[0][j]=_y._data[i][j]-z._data[0][j];
                }
                xOne_rowT = xOne_row.transposeMatrix();
                grad = xOne_rowT * z;///根据一样样本的预测误差来确定负梯度方向
                for(k = 0; k < grad._row;k++)
                {
                    for(j = 0; j < grad._col; j++)
                    {
                        grad._data[k][j] *= _alpha;///负梯度方向与步长的乘积确定迭代值
                    }
                }
                weights = weights + grad; ///迭代
            }
        }
        //验证算法的正确性
        /**
        验证算法的正确性
        **/
        Matrix test = _x * weights;
        std::cout<<"test"<<std::endl;
        for(i = 0; i < _y._row; i++)
        {
            if(test._data[i][0] > test._data[i][1])
                std::cout<<0 - _y._data[i][1]<<"  ";
            else
                std::cout<<1 - _y._data[i][1]<<"  ";
            std::cout<<std::endl;
        }
    }

    SoftMaxReg::SoftMaxReg(const std::string &file, const std::string &model, const double &alpha, const int &iter)
    {
        _x.init_by_data(file);
        _y=_x.getOneCol(_x._col-1);
		_x.deleteOneCol(_x._col-1);
		one_hot(_y,2);
		std::cout << "x:" << _x._row << "&" << _x._col << std::endl;
		std::cout << "y:" <<  _y._row << "&" << _y._col << std::endl;
        _alpha = alpha;
        _iter = iter;
    }
}
