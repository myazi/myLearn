#include "MatrixOpe.h"

/**
梯度下降算法，主要是确定负梯度方向，步长，采用迭代的思想迭代n至收敛，
当目标函数是凸规划问题，那么局部最小值就是全局最小值

**/
int gradAscent_Log(Matrix x,Matrix y)
{
    if(y.col!=1)
    {
        cout<<"logReg is two class"<<endl;
        return -1;
    }
    Matrix weights(x.col,y.col,0.1,"T");
    Matrix xT = x.transposeMatrix();

    float alpha=0.01;///迭代步长
    float error=0;///记录错误率
    int iter=0;
    int i,j;
    Matrix z(y.row,y.col,0,"T");//最好确定矩阵的大小
    Matrix grad(x.col,y.col,0,"T");
    for(iter=0; iter<5000; iter++)
    {
        z = x * weights;
        for(i=0; i<z.row; i++)
        {
            z.data[i][0]=sigmoid(z.data[i][0]);
        }
        z = y - z;
        error=0;
        for(i=0; i<x.row; i++)///统计错误率
            error+=z.data[i][0];
        //cout<<"error="<<error<<endl;
        //if(error<x.row/100 && error>x.row/100)///设置错误率小于一定值时退出迭代
           // break;
        grad = xT * z;///计算负梯度方向
        //grad = (1.0/x.row) * grad;
        for(i=0; i<grad.row; i++)
            grad.data[i][0]*= alpha;///负梯度方向与步长的乘积确定迭代值
        weights = weights + grad;///往负梯度方向走一个步长
    }

    /**
    验证算法的正确性
    **/
    int er1=0,er2=0;
    Matrix train=x * weights;
    cout<<"test"<<endl;
    for(i=0; i<y.row; i++)
    {
        if(train.data[i][0]>0)
        {
            cout<<1-y.data[i][0]<<endl;
            er1+=(1-y.data[i][0]);
        }
        else
        {
            cout<<0-y.data[i][0]<<endl;
            er2-=(0-y.data[i][0]);
        }
    }
    //cout<<"er1="<<er1<<endl;
    //cout<<"er2="<<er2<<endl;
}
/**
随机梯度下降与梯度下降法不同的是在负梯度方向的确定，梯度下降是根据所有的样本来确定负梯度方向，
而随机梯度下降每次只看一个样本点来确定负梯度方向，虽然不完全可信，但随着迭代次数增加，同样收敛

**/
int stoGradAscent_Log(Matrix x,Matrix y)//随机梯度下降每一次选择m个样本进行求梯度下降方向，该代码中只选择一个样本进行求解梯度下降方向与数值
{
    Matrix xOneRow(1,x.col,0,"T");
    Matrix xOneRowT(x.col,1,0,"T");

    Matrix weights(x.col,y.col,0.1,"T");
    Matrix z(1,y.col,0,"T");//最好确定矩阵的大小
    Matrix grad(x.col,y.col,0,"T");

    double alpha=0.001;///步长
    double error;
    int i,j,c;
    for(c=0; c<1000; c++)
    {
        for(i=0; i<x.row; i++)
        {
            xOneRow=x.getOneRow(i);///随机选择一个样本点，这里没有作随机选择，而是按序选择
            z = xOneRow * weights;
            z.data[0][0]=sigmoid(z.data[0][0]);
            z.data[0][0]=y.data[i][0]-z.data[0][0];
            xOneRowT = xOneRow.transposeMatrix();
            grad = xOneRowT * z;///根据一样样本的预测误差来确定负梯度方向
            for(j=0; j<grad.row; j++)
                grad.data[j][0]*=alpha;
            weights = weights + grad; ///迭代
        }
    }
    //验证算法的正确性
    cout<<"test"<<endl;
    Matrix test = x * weights;
    for(i=0; i<y.row; i++)
    {
        if(test.data[i][0]>0)
        {
            cout<<1-y.data[i][0]<<endl;
        }
        else
        {
            cout<<0-y.data[i][0]<<endl;
        }
    }
}

int LogReg()
{
    const char *file="data\\LogReg.txt";
    const string model="gradAscent";
    const double alpha=0.01;
    Matrix x;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    x.LoadData(file);
    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);

    if(model=="gradAscent")
        gradAscent_Log(x,y);
    if(model=="stoGradAscent")
        stoGradAscent_Log(x,y);

    return 0;
}
