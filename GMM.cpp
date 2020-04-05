#include "GMM.h"

namespace MLL
{

	double GMM::COV2(const Matrix &sigma)
	{
		double cov = 0;
		for(size_t i = 0; i < sigma._row; i++)
		{
			for(size_t j = 0; j < sigma._col; j++)
			{
				cov += sqrt(sigma._data[i][j]);
			}
		}
		return cov;
	}

	Matrix GMM::E_step(const Matrix &x)
	{
		Matrix w;
		w.initMatrix(K, x._row, 0);
		Matrix wNorm;
		wNorm.initMatrix(1, x._row, 0);
		Matrix xOne;
		double p = 0;
		for(size_t i = 0; i < x._row; i++)
		{
			for(int k = 0; k < w._row; k++)
			{
				xOne = x.getOneRow(i);
				p = Phi[k] * 1.0 / (pow(PI,double(K/2.0)) * COV2(Sigma[k])) * exp(-0.5 *(((xOne-Mu[k]) * Sigma[k].niMatrix()) * (xOne-Mu[k]).transposeMatrix())._data[0][0]);
				w._data[k][i] = p;
				wNorm._data[0][i] += p;
			}
			for(int k = 0; k < w._row; k++)
			{
				w._data[k][i] /= wNorm._data[0][i];
			}
		}
		return w;
	}

	int GMM::M_step(const Matrix &w, const Matrix &x)
	{
		Phi.clear();
		Mu.clear();
		Sigma.clear();
		Matrix mu;
		mu.initMatrix(1, x._col, 0);
		Matrix sigma;
		sigma.initMatrix(x._col, x._col, 0);
		for(size_t k = 0; k < w._row; k++)
		{
			Phi.push_back(0);
			for(size_t j = 1; j < x._col; j++)
				mu._data[0][j] = 0;
			for(size_t i = 0; i < sigma._row; i++)
				for(size_t j = 1; j < sigma._col; j++)
					sigma._data[i][j] = 0;
			for(size_t i = 0; i < x._row; i++)
			{
				Phi[k] += w._data[k][i];//计算phi
				for(size_t j = 0; j < x._col; j++)
					mu._data[0][j] += w._data[k][i] * x._data[i][j];//计算mu
			}
			Phi[k] /= x._row;//更新phi
			for(size_t j = 0; j < x._col; j++)
				mu._data[0][j] /= (Phi[k] * x._row);
			Mu.push_back(mu);//更新mu
			for(size_t i = 0; i < x._row; i++)
			{
				for(size_t j = 0; j < x._col; j++)
				{
					for(size_t j2 = 0; j2 < x._col; j2 ++)
					{
						sigma._data[j][j2] += w._data[k][i] * (x._data[i][j]-mu._data[0][j]) * (x._data[i][j2]-mu._data[0][j2]);
					}
				}
			}
			for(size_t i = 0; i < sigma._row; i++)
				for(size_t j = 0; j < sigma._col; j++)
					sigma._data[i][j] /= (Phi[k] * x._row);
			Sigma.push_back(sigma);//更新Sigma
		}
		return 0;
	}

	GMM::GMM(const std::string &file, const int &k, const int &Iter)
	{
		Matrix x;
		x.init_by_data(file);
		Matrix sigma(x._col,x._col,100,"diag");//初始化方差
		Matrix mu(k, x._col, 0);
		K = k;
		Matrix onek;
		for(int i = 0; i < x._row; i++)
		{
			for(int j = 0; j < x._col; j++)
				mu._data[i%K][j] += x._data[i][j];
		}
		for(int k = 0; k < K; k++)
		{
			Phi.push_back(1.0/K);
			Sigma.push_back(sigma);
			for(int j = 0; j < x._col; j++)
				mu._data[k][j] = mu._data[k][j] / x._row * K;
			Mu.push_back(mu.getOneRow(k));
		}
		Matrix w;
		for(int it=0; it<Iter; it++)
		{
			std::cout<<"it==============="<<it<<std::endl;
			for(int i = 0; i < K; i++)
			{
				std::cout<<Phi[i]<<std::endl;
				Mu[i].print();
				Sigma[i].print();
			}
			w = E_step(x);
			M_step(w, x);
		}
	}
}
