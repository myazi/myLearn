#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "loadData.h"
#include <fstream>
#include <sstream>
#include <stack>
#define MAX 1000000
#define MIN -100000
#define MAX_MATRIX_COL 1000
#define MAX_MATRIX_ROW 100
using namespace std;
class Matrix
{
public:
    double **mat;
    int col,row;
public:
    int loadMatrix(Matrix *matrix,dataToMatrix dtm)
    {
        int i,j;
        Data *p;
        p=dtm.dataSet->next;
        matrix->mat=(double **)malloc(sizeof(double*)*dtm.col);
        if(!matrix->mat)
        {
            cout<<"loadMatrix fail"<<endl;
            exit(-1);
        }
        for(i=0; i<dtm.col&&p!=NULL; i++)
        {
            matrix->mat[i]=(double *)malloc(sizeof(double)*dtm.row);
            if(!matrix->mat[i])
            {
                cout<<"loadmatrix fail"<<endl;
                exit(-1);
            }
            for(j=0; j<dtm.row; j++)
            {
                matrix->mat[i][j]=p->attr_double[j];
            }
            p=p->next;
        }
        matrix->row=dtm.row;
        matrix->col=dtm.col;
        return 0;
    }

    /**
    new和malloc的本质区别在于，前者是针对对象而言，无论是基本类型还是类类型，结构体类型都可以看作对象，
    使用new是会调用对象的构造函数，初始化对象，即首先得分配内存，后初始化，分配内存这事内部实现可能是调用malloc
    而malloc只是分配内存，对于基本类型而言，这是没问题的，后期可以继续初始化，而对于对象而言，malloc只分配内存
    不调用构造函数，所以不会对对象进行初始化
    **/

    int initMatrix(Matrix *matrix,int col,int row)
    {
        initMatrix(matrix,col,row,0);
    }
    int initMatrix(Matrix *matrix,int col,int row,double lam)
    {
        if(col==0||row==0)
        {
            cout<<"matrix row or col no can 0"<<endl;
            exit(-1);
        }
        matrix->col=col;
        matrix->row=row;
        matrix->mat=(double **)malloc(sizeof(double*)*col);
        if(!matrix->mat)
        {
            cout<<"initMatrix fail"<<endl;
            exit(-1);
        }
        int i=0,j=0;
        for(i=0; i<col; i++)
        {
            matrix->mat[i]=(double *)malloc(sizeof(double)*row);
            if(!matrix->mat[i])
            {
                cout<<"initMatrix fail"<<endl;
                exit(-1);
            }
            for(j=0; j<row; j++)
            {
                matrix->mat[i][j]=0;
                if(i==j)
                matrix->mat[i][j]=lam;
            }
        }
        return 0;
    }
    int print(Matrix matrix)
    {
        int i,j;
        for(i=0; i<matrix.col; i++)
        {
            for(j=0; j<matrix.row; j++)
            {
                cout<<matrix.mat[i][j]<<"  ";;
            }
            cout<<endl;
        }
    }
    int copy(Matrix matrixA,Matrix *matrixB)
    {
        if(!matrixB)//还应该考虑a
        {
            cout<<"matrixB is null"<<endl;
            exit(-1);
        }
        if(matrixB->col!=matrixA.col||matrixB->row!=matrixA.row)
        {
            cout<<"matrixA matrixB is no "<<endl;
            exit(-1);
        }
        int i,j;
        for(i=0; i<matrixA.col; i++)
        {
            for(j=0; j<matrixA.row; j++)
            {
                matrixB->mat[i][j]=matrixA.mat[i][j];
            }
        }
        return 0;
    }
    Matrix getOneRow(Matrix matrix,int iRow)
    {
        Matrix oneRow;
        oneRow.col=matrix.col;
        oneRow.row=1;
        initMatrix(&oneRow,oneRow.col,oneRow.row);
        int i=0;
        for(i=0; i<oneRow.col; i++)
        {
            oneRow.mat[i][0]=matrix.mat[i][iRow-1];
        }
        return oneRow;
    }
    Matrix getOneCol(Matrix matrix,int iCol)
    {
        Matrix oneCol;
        oneCol.row=matrix.row;
        oneCol.col=1;
        initMatrix(&oneCol,oneCol.col,oneCol.row);
        int i=0;
        for(i=0; i<oneCol.row; i++)
        {
            oneCol.mat[0][i]=matrix.mat[iCol][i];
        }
        return oneCol;
    }
    /**
    下面这个获取一行的数据的方式可以避免当该函数放到循环中是，由于反复申请内存导致内存不足
    **/
    void getOneCol(Matrix oneCol,Matrix matrix,int iCol)
    {
        oneCol.row=matrix.row;
        oneCol.col=1;
        int i=0;
        for(i=0; i<oneCol.row; i++)
        {
            oneCol.mat[0][i]=matrix.mat[iCol][i];
        }
    }
    
    int deleteOneRow(Matrix *matrix,int iRow)
    {
        if(matrix->row==1)
        {
            cout<<"matrix is vec"<<endl;
            exit(-1);
        }
        int i,j;
        for(i=iRow; i<matrix->row; i++)
        {
            for(j=0;j<matrix->col;j++)
            {
                matrix->mat[j][i]=matrix->mat[j][i+1];
            }
        }
        matrix->row--;
        return 0;
    }
    void transposematrix(Matrix matrix,Matrix *matrixT)//矩阵形式的转置
    {
        if(matrixT->col!=matrix.row||matrixT->row!=matrix.col)
        {
            cout<<"matrix matrixT is no "<<endl;
            exit(-1);
        }
        int i=0,j=0;
        for(i=0; i<matrixT->col; i++)
        {
            for(j=0; j<matrixT->row; j++)
            {
                matrixT->mat[i][j]=matrix.mat[j][i];
            }
        }
    }
    int addmatrix(Matrix *addMatrix,Matrix matrix1,Matrix matrix2)
    {
        if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row||addMatrix->col!=matrix1.col||addMatrix->row!=matrix1.row)
        {
            cout<<"addMatrix matrix1 matrix2 is no"<<endl;
            exit(-1);
        }
        int i,j;
        for(i=0; i<matrix1.col; i++)
        {
            for(j=0; j<matrix1.row; j++)
            {
                addMatrix->mat[i][j]=matrix1.mat[i][j]+matrix2.mat[i][j];
            }
        }
        return 0;
    }

    int submatrix(Matrix *subMatrix,Matrix matrix1,Matrix matrix2)
    {
        if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row||subMatrix->col!=matrix1.col||subMatrix->row!=matrix1.row)
        {
            cout<<"subMatrix matrix1 matrix2 is no"<<endl;
            exit(-1);
        }
        int i,j;
        subMatrix->col=matrix1.col;
        subMatrix->row=matrix1.row;
        for(i=0; i<matrix1.col; i++)
        {
            for(j=0; j<matrix1.row; j++)
            {
                subMatrix->mat[i][j]=matrix1.mat[i][j]-matrix2.mat[i][j];
            }
        }
        return 0;
    }

    int multsmatrix(Matrix *multsMatrix,Matrix matrix1,Matrix matrix2)//矩阵形式的相乘
    {
        if(matrix1.row!=matrix2.col||multsMatrix->col!=matrix1.col||multsMatrix->row!=matrix2.row)
        {
            cout<<"multsmatrix error"<<endl;
            exit(-1);
        }
        int i,j,k,l;
        for(i=0; i<matrix1.col; i++)
        {
            for(j=0; j<matrix2.row; j++)
            {
                multsMatrix->mat[i][j]=0;
            }
        }
        for(i=0; i<matrix1.col; i++)
        {
            for(j=0; j<matrix2.row; j++)
            {
                for(k=0; k<matrix1.row; k++)
                {
                    multsMatrix->mat[i][j]+=matrix1.mat[i][k]*matrix2.mat[k][j];
                }
            }
        }
        return 0;
    }
    //行列式
    double detmatrix(Matrix matrix)
    {
        if(matrix.col!=matrix.row)
        {
            cout<<"matrix det is no"<<endl;
            exit(-1);
        }
        Matrix matrixCopy;
        matrixCopy.initMatrix(&matrixCopy,matrix.col,matrix.row);
        matrixCopy.copy(matrix,&matrixCopy);
        double det=1;
        int i=0,j,k;
        double max=MIN;
        int swap=-1;
        double temp;
        double aij[MAX_MATRIX_COL][MAX_MATRIX_ROW];//这里用矩阵就更好了
        for(k=0; k<matrixCopy.row-1; k++)//k表示第k次消元，一共需要n-1次
        {
            for(i=0; i<matrixCopy.col; i++)
            {
                if(matrixCopy.mat[i][k]>max)//每一次消元都是比较第k列的元素，选出第k列中最大的一行
                {
                    swap=i;
                }
            }//找到第k次列主元消去的最大行的下标
            if(swap==-1||matrixCopy.mat[swap][k]==0)
                return -1;//最大主元为0
            for(j=0; j<matrixCopy.row; j++)
            {
                temp=matrixCopy.mat[k][j];
                matrixCopy.mat[k][j]=matrixCopy.mat[swap][j];
                matrixCopy.mat[swap][j]=temp;
            }//第k次消元，选出最大的一行是swap行，与第k行交换
            for(i=k+1; i<matrixCopy.col; i++)
            {
                aij[i][k]=matrixCopy.mat[i][k]/matrixCopy.mat[k][k];// 第k次消元，主元素为第k行第k列，把第k行以下的行都进行消元
                for(j=k; j<matrixCopy.row; j++)//对于k行以下的每一行的每一列元素都减去主行与消元因子的乘积
                {
                    matrixCopy.mat[i][j]-=aij[i][k]*matrixCopy.mat[k][j];
                }
            }
        }
        for(i=0; i<matrixCopy.col; i++)
        {
            det*=matrixCopy.mat[i][i];
        }
        cout<<"det="<<det<<endl;
        return det;
    }
    //高斯消元矩阵求逆,特别注意，LU分解不能进行行列式变换
    int nimatrix(Matrix *niMatrix,Matrix matrix)
    {
        if(matrix.col!=matrix.row)
        {
            cout<<"matrix ni is no "<<endl;
            exit(-1);
        }
        if(detmatrix(matrix)==0)//这里调用求行列式进行了列主元消去改变了参数矩阵，如何传递不改变是一个问题
        {
            cout<<"matrix det is no so ni is no "<<endl;
            exit(-1);
        }
        int i=0,j,k;
        double temp;
        Matrix cpMatrix;
        Matrix uMatrix;
        Matrix lMatrix;
        Matrix uniMatrix;
        Matrix lniMatrix;
        initMatrix(&uniMatrix,matrix.col,matrix.row);
        initMatrix(&lniMatrix,matrix.col,matrix.row);
        initMatrix(&cpMatrix,matrix.col,matrix.row);
        initMatrix(&uMatrix,matrix.col,matrix.row);
        initMatrix(&lMatrix,uMatrix.col,uMatrix.row);
        copy(matrix,&cpMatrix);
        double aij[MAX_MATRIX_COL][MAX_MATRIX_ROW];
        for(k=0; k<matrix.row-1; k++)//k表示第k次消元，一共需要n-1次
        {
            for(i=k+1; i<matrix.col; i++)
            {
                aij[i][k]=matrix.mat[i][k]/matrix.mat[k][k];// 第k次消元，主元素为第k行第k列，把第k行以下的行都进行消元
                for(j=k; j<matrix.row; j++)//对于k行以下的每一行的每一列元素都减去主行与消元因子的乘积
                {
                    matrix.mat[i][j]-=aij[i][k]*matrix.mat[k][j];
                }
            }
        }
        copy(matrix,&uMatrix);
        cout<<"uMatrix"<<endl;
        print(uMatrix);
        for(j=0; j<matrix.row; j++)
        {
            for(i=j+1; i<matrix.col; i++)
            {
                temp=0;
                for(k=0; k<j; k++)
                {
                    temp=lMatrix.mat[i][k]*uMatrix.mat[k][j];
                }
                lMatrix.mat[i][j]=1/uMatrix.mat[j][j]*(cpMatrix.mat[i][j]-temp);
            }
        }
        for(i=0; i<lMatrix.col; i++)
        {
            for(j=0; j<lMatrix.row; j++)
            {
                if(i==j)
                    lMatrix.mat[i][j]=1;
                if(j>i)
                    lMatrix.mat[i][j]=0;
            }
        }
        cout<<"lMatrix"<<endl;
        print(lMatrix);
        Matrix multsMatrix;
        multsMatrix.initMatrix(&multsMatrix,lMatrix.col,uMatrix.row);
        matrix.multsmatrix(&multsMatrix,lMatrix,uMatrix);
        cout<<"lu"<<endl;
        print(multsMatrix);
        //计算u逆
        for(j=0; j<uMatrix.row; j++)
        {
            for(i=j; i>=0; i--)
            {
                if(i==j)
                    uniMatrix.mat[i][j]=1/uMatrix.mat[i][j];
                else
                {
                    temp=0;
                    for(k=j; k>i; k--)
                    {
                        temp+=uMatrix.mat[i][k]*uniMatrix.mat[k][j];
                    }
                    uniMatrix.mat[i][j]=-1/uMatrix.mat[i][i]*temp;
                }
            }
        }
        cout<<"uniMatrix"<<endl;
        print(uniMatrix);
        //计算l逆
        for(j=0; j<lMatrix.row; j++)
        {
            for(i=0; i<lMatrix.col; i++)
            {
                if(j==i)
                    lniMatrix.mat[i][j]=1;
                else
                {
                    temp=0;
                    for(k=j; k<i; k++)
                    {
                        temp+=(lMatrix.mat[i][k]*lniMatrix.mat[k][j]);
                    }
                    lniMatrix.mat[i][j]=-temp;
                }
            }
        }
        cout<<"lniMatrix"<<endl;
        print(lniMatrix);
        multsmatrix(&multsMatrix,uniMatrix,lniMatrix);
        cout<<"luni"<<endl;
        print(multsMatrix);
        copy(multsMatrix,niMatrix);
        multsmatrix(&multsMatrix,cpMatrix,*niMatrix);
        cout<<"luluni"<<endl;
        print(multsMatrix);
        copy(cpMatrix,&matrix);
    }
    int LDL(Matrix x)//矩阵的LDL分解，不知道怎样用于矩阵特征值，特征向量求解
    {
        Matrix l;
        l.initMatrix(&l,x.col,x.row);
        Matrix d;
        d.initMatrix(&d,x.col,x.row);
        int i,j,k;
        Matrix temp;
        temp.initMatrix(&temp,x.col,x.row);
        for(i=0;i<x.col;i++)
        {
            l.mat[i][i]=1;
            for(j=0;j<i;j++)
            {
                for(k=0;k<j;k++)
                {
                    temp.mat[i][k]=l.mat[i][k]*d.mat[k][k];
                    temp.mat[i][j]-=temp.mat[i][k]*l.mat[j][k];
                }
                temp.mat[i][j]=temp.mat[i][j]+x.mat[i][j];
                l.mat[i][j]=temp.mat[i][j]/d.mat[j][j];
            }
            d.mat[i][i]=x.mat[i][i];
            for(k=0;k<i;k++)
            {
                d.mat[i][i]-=temp.mat[i][k]*l.mat[i][k];
            }
        }
        for(i=0;i<x.col;i++)
        {
            for(j=0;j<x.row;j++)
            {
                cout<<l.mat[i][j]<<"  ";
            }
            cout<<endl;
        }
        for(i=0;i<x.col;i++)
        {
            for(j=0;j<x.row;j++)
            {
                cout<<d.mat[i][j]<<"  ";
            }
            cout<<endl;
        }
    }
};
