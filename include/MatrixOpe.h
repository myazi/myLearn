#ifndef MatrixOpe_H
#define MatrixOpe_H
#include "Matrix.h"
#include <memory>
namespace MLL
{
	Matrix operator+(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator-(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);

	Matrix operator*(const double &alpha, const Matrix &matrix1);

	Matrix operator/(const double &alpha, const Matrix &matrix1);

	inline void sigmoid(float &z)
	{
		z = 1.0 / (1.0 + exp(-z));       
	}

	inline void sigmoid(Matrix &z)
	{
		for(size_t i = 0; i < z._row; i++)
		{
			for(size_t j = 0; j < z._col; j++)
			{
				z._data[i][j] = 1.0 / (1.0 + exp(- z._data[i][j]));
			}
		}
	}
	
	inline void one_hot(Matrix &Y, const int &C)
	{
		std::vector<double> label;
		if(Y._row > Y._col)
		{
			size_t i = 0;
			for(i = 0; i < Y._row; i++){
				label.push_back(Y._data[i][0]);
			}
			RowData cda(C);
			Data da(Y._row,cda);
			Y._data = da;
			Y._row = Y._row;
			Y._col = C;
			for(i = 0; i < Y._row; i++)
			{
				Y._data[i][label[i]] = 1;
			}
		}
		else
		{
			size_t j = 0;
			for(j = 0; j < Y._col; j++){
				label.push_back(Y._data[0][j]);
			}
			RowData cda(Y._col);
			Data da(C,cda);
			Y._data = da;
			Y._row = C;
			Y._col = Y._col;
			for(j = 0; j < Y._col; j++)
			{
				Y._data[label[j]][j] = 1;
			}
		}
	}
}
#endif // MatrixOpe_H
