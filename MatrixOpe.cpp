#include <MatrixOpe.h>
#include <stdlib.h>
namespace MLL
{
	Matrix operator+(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "addData data1 data2 is no" << std::endl;
			exit(-1);
		}
		size_t i = 0, j = 0;
		Matrix add(matrix1._row, matrix1._col, 0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				add._data[i][j] = matrix1._data[i][j] + matrix2._data[i][j];
			}
		}
		return add;
	}
	Matrix operator-(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._col || matrix1._row != matrix2._row)
		{
			std::cout << matrix1._row<< "*" << matrix1._col << std::endl;
			std::cout << matrix2._row<< "*" << matrix2._col << std::endl;
			std::cout << "addData data1 data2 is no" << std::endl;
			exit(-1);
		}
		size_t i = 0, j = 0;
		Matrix sub(matrix1._row, matrix1._col, 0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				sub._data[i][j] = matrix1._data[i][j] - matrix2._data[i][j];
			}
		}
		return sub;
	}
	Matrix operator*(const Matrix &matrix1, const Matrix &matrix2)
	{
		if(matrix1._col != matrix2._row)
		{
			std::cout << matrix1._row << "*" << matrix1._col << std::endl;
			std::cout << matrix2._row << "*" << matrix2._col << std::endl;
			std::cout << "multsData error" << std::endl;
			exit(-1);
		}
		size_t i = 0, j = 0, k = 0;
		Matrix mults(matrix1._row,matrix2._col,0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix2._col; j++)
			{
				mults._data[i][j] = 0;
			}
		}
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix2._col; j++)
			{
				for(k = 0; k < matrix1._col; k++)
				{
					mults._data[i][j] += matrix1._data[i][k] * matrix2._data[k][j];
				}
			}
		}
		return mults;
	}
	Matrix operator*(const double &alpha, const Matrix &matrix1)
	{
		size_t i = 0, j = 0;
		Matrix mults(matrix1._row,matrix1._col,0);
		for(i = 0; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				mults._data[i][j] = alpha * matrix1._data[i][j];
			}
		}
		return mults;
	}
	Matrix operator/(const double &alpha, const Matrix &matrix1)
	{
		size_t i = 0, j = 0;
		Matrix mults(matrix1._row,matrix1._col,0);
		for(i = 0 ; i < matrix1._row; i++)
		{
			for(j = 0; j < matrix1._col; j++)
			{
				mults._data[i][j] = matrix1._data[i][j] / alpha;
			}
		}
		return mults;
	}
}
