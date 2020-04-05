#include "LogReg.h"

namespace MLL {
                       
    /**
    梯度下降算法，主要是确定负梯度方向，步长，采用迭代的思想迭代n至收敛，
    当目标函数是凸规划问题，那么局部最小值就是全局最小值

    **/
    int LogReg::gradAscent_Log()
    {
        if(_y._col != 1)
        {
            std::cout<<"logReg is two class"<<std::endl;
            return -1;
        }
        
		float error = 0;///记录错误率
        int it = 0;
        int i = 0, j = 0;

		Matrix weights(_x._col,_y._col,0.1);
        Matrix xT = _x.transposeMatrix();
        Matrix z(_y._row,_y._col,0); //调用构造函数时，最好确定矩阵的大小
        Matrix grad(_x._col,_y._col,0);
        
		for(it = 0; it < _iter; it++)
        {
            error = 0;
			z = _x * weights;
            for(i = 0; i < z._row; i++)
            {
                sigmoid(z._data[i][0]);
            }
            z = _y - z ;
			for(i = 0; i < z._row; i++)
            {
                z._data[i][0] *= exp( -0.5 * _y._data[i][0]);
            }
            for(i = 0; i < _x._row; i++)///统计错误率
                error += z._data[i][0];
			//std::cout<<"error="<<error<<std::endl;
            //if(error<x._row/100 && error>x._row/100)///设置错误率小于一定值时退出迭代
               // break;
            grad = xT * z ;///计算负梯度方向
            //grad = (1.0 / x._row) * grad;
            for(i = 0; i < grad._row; i++)
                grad._data[i][0] *= _alpha;///负梯度方向与步长的乘积确定迭代值
            weights = weights + grad;///往负梯度方向走一个步长
        }
		//写参数文件
		std::ofstream weight_file;
		weight_file.open("weight");
		for(int i=0; i < weights._row; i++)
		{
			for(int j=0; j < weights._col; j++)
			{
				weight_file<< weights._data[i][j]<<std::endl;
			}
		}
		weight_file.close();
        
		/**
        验证算法的正确性
        **/
        int er1 = 0, er2 = 0;
        Matrix train=_x * weights;
		std::cout<<"----------train-------"<<std::endl;
        for(i=0; i<_y._row; i++)
        {
            if(train._data[i][0]>0)
            {
                std::cout<<"pre_label: 1" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
                er1 += (1-_y._data[i][0]);
            }
            else
            {
                std::cout<<"pre_label: 0" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
                er2 -= (0-_y._data[i][0]);
            }
        }
        std::cout<<"er1="<<er1<<std::endl;
        std::cout<<"er2="<<er2<<std::endl;
    }

    /**
    随机梯度下降与梯度下降法不同的是在负梯度方向的确定，梯度下降是根据所有的样本来确定负梯度方向，
    而随机梯度下降每次只看一个样本点来确定负梯度方向，虽然不完全可信，但随着迭代次数增加，同样收敛

    **/
    int LogReg::stoGradAscent_Log()//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
    {
        Matrix xOnerow(1, _x._col, 0);
        Matrix xOnerowT(_x._col, 1, 0);

        Matrix weights(_x._col, _y._col, 0.1);
        Matrix z(1, _y._col, 0);
        Matrix grad(_x._col, _y._col, 0);

        int i = 0, j = 0, it = 0;
        for(it = 0; it < 1000; it++)
        {
            for(i = 0; i < _x._row; i++)
            {
                xOnerow=_x.getOneRow(i);///随机选择一个样本点，这里没有作随机选择，而是按序选择
                z = xOnerow * weights;
				sigmoid(z._data[i][0]);
                z._data[i][0]=_y._data[i][0]-z._data[i][0];
                xOnerowT = xOnerow.transposeMatrix();
                grad = xOnerowT * z;///根据一样样本的预测误差来确定负梯度方向
                for(j = 0; j < grad._row; j++)
                    grad._data[j][0] *= _alpha;
                weights = weights + grad; ///迭代
            }
        }
        //验证算法的正确性
        std::cout<<"test"<<std::endl;
        Matrix test = _x * weights;
        for(i = 0; i < _y._row; i++)
        {
            if(test._data[i][0] > 0)
            {
                std::cout<<1-_y._data[i][0]<<std::endl;
            }
            else
            {
                std::cout<<0-_y._data[i][0]<<std::endl;
            }
        }
    }
    LogReg::LogReg(const std::string &file, const std::string &model, const double alpha,const int iter)
    {
        _x.init_by_data(file);
		_y = _x.getOneCol(_x._col-1);
		_x.deleteOneCol(_x._col-1);
		_alpha = alpha;
        _iter = iter;
    }
}
