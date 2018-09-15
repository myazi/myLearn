#include "SVM.h"
#define MAX 1000000
#define MIN -100000
//SMO参数结构体
int SVM()
{
    ///加载数据文件保存到对象dtm的矩阵元素中

    ///通过矩阵对象中的load函数初始化样本的特征和类别矩阵x,y
    //char file[20]="data\\svm.txt";
    Matrix x;
    //x.LoadData(file);

    char file[20]="data\\svm.txt";
    x.LoadData(file);
    //x = x.transposeMatrix();

    Matrix y;
    y=x.getOneCol(x.col-1);
    x.deleteOneCol(x.col-1);
    for(int i=x.row-1;i>100;i--)
    //x.deleteOneCol(x.col-1);
        x.deleteOneRow(i);
    cout<<x.row<<"*"<<x.col<<endl;
    cout<<y.row<<"*"<<y.col<<endl;
    ///调用SMO函数求解SVM模型
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}


