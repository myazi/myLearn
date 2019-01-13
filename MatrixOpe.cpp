#include <Matrix.h>
#include <stdlib.h>
Matrix operator+(Matrix matrix1,Matrix matrix2)
{
    if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"addData data1 data2 is no"<<endl;
        exit(-1);
    }
    RowData cda(matrix1.col);
    Data da(matrix1.row,cda);
    Matrix add;
    add.data=da;
    add.row=matrix1.row;
    add.col=matrix1.col;
    unsigned int i,j;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            add.data[i][j]=matrix1.data[i][j]+matrix2.data[i][j];
        }
    }
    return add;
}
Matrix operator-(Matrix matrix1,Matrix matrix2)
{
    if(matrix1.col!=matrix2.col||matrix1.row!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"addData data1 data2 is no"<<endl;
        exit(-1);
    }
    RowData cda(matrix1.col);
    Data da(matrix1.row,cda);
    Matrix sub;
    sub.data=da;
    sub.row=matrix1.row;
    sub.col=matrix1.col;
    unsigned int i,j;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            sub.data[i][j]=matrix1.data[i][j]-matrix2.data[i][j];
        }
    }
    return sub;
}
Matrix operator*(Matrix matrix1,Matrix matrix2)
{
    if(matrix1.col!=matrix2.row)
    {
        cout<<matrix1.row<<"*"<<matrix1.col<<endl;
        cout<<matrix2.row<<"*"<<matrix2.col<<endl;
        cout<<"multsData error"<<endl;
        exit(-1);
    }
    unsigned int i,j,k;
    Matrix mults;
    ColData cda(matrix2.col);
    Data da(matrix1.row,cda);
    mults.data=da;
    mults.row=matrix1.row;
    mults.col=matrix2.col;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix2.col; j++)
        {
            mults.data[i][j]=0;
        }
    }
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix2.col; j++)
        {
            for(k=0; k<matrix1.col; k++)
            {
                mults.data[i][j]+=matrix1.data[i][k]*matrix2.data[k][j];
            }
        }
    }
    return mults;
}
Matrix operator*(double alpha,Matrix matrix1)
{
    unsigned int i,j;
    Matrix mults;
    ColData cda(matrix1.col);
    Data da(matrix1.row,cda);
    mults.data=da;
    mults.row=matrix1.row;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            mults.data[i][j]=alpha * matrix1.data[i][j];
        }
    }
    return mults;
}
Matrix operator/(Matrix matrix1 ,double alpha)
{
    unsigned int i,j;
    Matrix mults;
    ColData cda(matrix1.col);
    Data da(matrix1.row,cda);
    mults.data=da;
    mults.row=matrix1.row;
    for(i=0; i<matrix1.row; i++)
    {
        for(j=0; j<matrix1.col; j++)
        {
            mults.data[i][j] = matrix1.data[i][j] / alpha;
        }
    }
    return mults;
}
Matrix sigmoid(Matrix z)
{

    for(size_t i=0;i<z.row;i++)
    {
        for(size_t j=0;j<z.col;j++)
        {
            z.data[i][j]=1.0/(1+exp(-z.data[i][j]));
        }
    }
    return z;
}
float sigmoid(float z)
{
    return 1.0/(1+exp(-z));
}
Matrix one_hot(Matrix Y, int C)
{
   if(Y.row>Y.col)
    {
        Matrix one_hot_Y(Y.row,C,0,"ss");
        int i=0;
        for(i=0; i<one_hot_Y.row; i++)
        {
            one_hot_Y.data[i][Y.data[i][0]]=1;
        }
        return one_hot_Y;
    }
    else
    {
        Matrix one_hot_Y(C,Y.col,0,"ss");
        int j=0;
        for(j=0; j<one_hot_Y.col; j++)
        {
            one_hot_Y.data[Y.data[0][j]][j]=1;
        }
        return one_hot_Y;
    }
}
