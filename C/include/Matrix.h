#ifndef Matrix_H
#define Matrix_H
#include "LoadData.h"
using namespace std;
class Matrix
{
public:
    Data data;
    unsigned int row;
    unsigned int col;
public:
    Matrix();

    Matrix(unsigned int row, unsigned int col,float lamd, const char* type);

    void initMatrix(unsigned int row, unsigned int col,float lamd, const char* type);

    void LoadData(const char *filename);

    void print();

    Matrix copyMatrix();

    Matrix getOneRow(unsigned int iRow);

    Matrix getOneCol(unsigned int jCol);

    void deleteOneRow(unsigned int iRow);

    void deleteOneCol(unsigned int iCol);

    Matrix transposeMatrix();//矩阵形式的转置

    Matrix addMatrix(const Matrix &matrix1,const Matrix &matrix2);

    Matrix subMatrix(const Matrix &matrix1,const Matrix &matrix2);

    Matrix multsMatrix(Matrix matrix1,Matrix matrix2);//矩阵形式的相乘

    Matrix dotmultsMatrix(Matrix matrix1,Matrix matrix2);//矩阵形式的相乘

    double detMatrix();//行列式

    Matrix niMatrix();//高斯消元矩阵求逆,特别注意，LU分解不能进行行列式变换
};
#endif // Data_H
