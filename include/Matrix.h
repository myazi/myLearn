#ifndef Matrix_H
#define Matrix_H
#include "LoadData.h"
//#include "LoadData_new.h"
namespace MLL
{
	class Matrix
	{
	public:
		Data _data;
		size_t _row;
		size_t _col;
	public:
		Matrix();
		
		~Matrix();

		Matrix(const Matrix &rhs);
		
		Matrix(const size_t &row, const size_t &col, const float &init_val);
		
		Matrix(const size_t &row, const size_t &col, const float &init_val, const std::string &type);

		void initMatrix(const size_t &row, const size_t &col, const float &init_val);

		void init_by_data(const std::string &filename);

		void init_by_spare(const std::string &filename, const size_t &row, const size_t &col);
		
		void print() const;

		Matrix copyMatrix() const;

		Matrix getOneRow(const size_t &iRow) const;

		Matrix getOneCol(const size_t &jCol) const;

		void deleteOneRow(const size_t &iRow);

		void deleteOneCol(const size_t &iCol);

		Matrix transposeMatrix();//矩阵形式的转置

		Matrix addMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix subMatrix(const Matrix &matrix1,const Matrix &matrix2);

		Matrix multsMatrix(const Matrix &matrix1, const Matrix &matrix2);//矩阵形式的相乘

		Matrix dotmultsMatrix(const Matrix &matrix1, const Matrix &matrix2);//矩阵形式的相乘

		double detMatrix();//行列式

		Matrix niMatrix();//高斯消元矩阵求逆,特别注意，LU分解不能进行行列式变换
	};
}
#endif // Data_H
