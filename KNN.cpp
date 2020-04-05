#include "KNN.h"

namespace MLL
{
	/***数据归一化处理，_data[i][j]-min[j]/range[j]**/
	int KNN::autoNorm(Matrix &x)
	{
		int j = 0, i = 0;

		Matrix minVals(1, x._col, 0);
		Matrix maxVals(1, x._col, 0);
		Matrix ranges(1, x._col, 0);
		for(j = 0; j < x._col; j++)
		{
			minVals._data[0][j] = x._data[0][j];
			maxVals._data[0][j] = x._data[0][j];
		}
		for(i = 0; i < x._row; i++)
		{
			for(j = 0; j < x._col; j++)
			{
				if(x._data[i][j] < minVals._data[0][j])
					minVals._data[0][j] = x._data[i][j];
				if(x._data[i][j] > maxVals._data[0][j])
					maxVals._data[0][j] = x._data[i][j];
			}
		}
		for(j = 0; j < x._col; j++)
			ranges._data[0][j] = maxVals._data[0][j] - minVals._data[0][j];
		for(i = 0; i < x._row; i++)
		{
			for(j = 0; j < x._col; j++)
			{
				x._data[i][j] -= minVals._data[0][j];
				if(ranges._data[0][j] != 0)
					x._data[i][j] /= ranges._data[0][j];
			}
		}
		return 0;
	}
	/**计算每个测试样本与训练样本的距离，保存在distance矩阵中**/
	Matrix KNN::cdistances(const Matrix &test, const Matrix &x)
	{
		int i = 0, j = 0;
		Matrix distances(x._row, 1, 0);
		for(i = 0; i < x._row; i++)
		{
			for(j = 0; j < x._col; j++)
			{
				distances._data[i][0] += pow((x._data[i][j] - test._data[0][j]), 2);
			}
			distances._data[i][0] = sqrt(distances._data[i][0]);
			//std::cout<<"dis="<<distances._data[i][0]<<std::endl;
		}
		return distances;
	}
	/***选择出K个近邻**/
	Matrix KNN::getK(const Matrix &oneTest, const Matrix &x)
	{
		int i =0, j = 0, k = 0;
		Matrix distances(x._row, 1, 0);//为每一个测试样本初始化k个近邻为前k个训练样本，并记录近邻的id
		distances = cdistances(oneTest, x);
		Matrix Kdistances(_K, 2, 0);
		double Max = -1;
		int Maxi = -1;
		for(i = 0; i < _K; i++)
		{
			Kdistances._data[i][0] = distances._data[i][0];
			Kdistances._data[i][1] = i;//记录近邻的id
			if(Kdistances._data[i][0] > Max)
			{
				Max = Kdistances._data[i][0];
				Maxi = i;//选出当前k个近邻中最大的一个
			}
		}
		//为每一个测试样本从第K个训练样本中遍历更新新的k个近邻
		for(i = _K; i < x._row; i++)
		{
			if(distances._data[i][0] < Max)
			{
				Kdistances._data[Maxi][0] = distances._data[i][0];
				Max = distances._data[i][0];//暂时更新当前替换的距离为最大距离，因为已经不能用之前的最大距离了
				Kdistances._data[Maxi][1] = i;//记录近邻的id
				for(k = 0; k < _K; k++)
				{
					if(Kdistances._data[k][0] > Max)
					{
						Max = Kdistances._data[k][0];
						Maxi = k;//选出当前k个近邻中最大的一个
					}
				}
			}
		}
		return Kdistances;
	}
	//KNN决规则，这里采用平均权重投票，且不考虑样本不均衡问题
	/**
	1，knn分类决策函数，首先对训练数据和测试数据进行归一化，
	2，计算每一个测试样本到m个训练样本的距离
	3，从m个距离中选出最小的k个距离，并记录这k个最小距离的样本
	4，由k个样本加权投票得到最终的决策类别

	***/
	int KNN::classfiy(Matrix &test_data, const Matrix &test_datay, Matrix &x, const Matrix &y)
	{
		int i = 0, j = 0, k = 0;
		int sumz = 0,sumf = 0;
		Matrix knn(_K, 2, 0);
		autoNorm(x);
		autoNorm(test_data);
	    Matrix oneTest(1,test_data._col,0);
		for(i = 0; i < test_data._row; i++)
		{
			sumz = 0;
			sumf = 0;
			oneTest = test_data.getOneRow(i);
			knn = getK(oneTest,x);
			for(j = 0; j < _K; j++)
			{

				if(y._data[int(knn._data[j][1])][0]==1)
					sumz++;
				else
					sumf++;
				std::cout<<y._data[int(knn._data[j][1])][0]<<"  ";
			}
			if(sumz > sumf)
				std::cout<<"juece="<<"1"<<"\t"<<"fact="<<test_datay._data[i][0]<<std::endl;
			else
				std::cout<<"juece="<<"!1"<<"\t"<<"fact="<<test_datay._data[i][0]<<std::endl;
		}
		return 0;
	}
	KNN::KNN(const std::string &file, const int &K)
	{
		_K = K;
		_x.init_by_data(file);
		_y = _x.getOneCol(_x._col-1);
		_x.deleteOneCol(_x._col-1);
		
		Matrix testx;
		char testFile[20]="data/knnTest.txt";
		testx.init_by_data(testFile);
		Matrix testy;
		testy=testx.getOneCol(testx._col-1);
		testx.deleteOneCol(testx._col-1);
		classfiy(testx, testy, _x, _y);
	}
}
