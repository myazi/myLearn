#include "MatrixOpe.h"
#define PI 3.1415926

vector<double> Phi;
vector<Matrix> Sigma;
vector<Matrix> Mu;
int K;

double COV2(Matrix sigma)
{
    double cov=0;
    for(size_t i=0; i<sigma.row; i++)
    {
        for(size_t j=0; j<sigma.col; j++)
        {
            cov+=sqrt(sigma.data[i][j]);
        }
    }
    return cov;
}

Matrix E_step(Matrix &x)
{
    Matrix w;
    w.initMatrix(K,x.row,0,"s");
    Matrix wNorm;
    wNorm.initMatrix(1,x.row,0,"s");
    Matrix xOne;
    double p=0;
    for(size_t i=0; i<x.row; i++)
    {
        for(int k=0; k<w.row; k++)
        {
            xOne = x.getOneRow(i);
            p = Phi[k] * 1.0 / (pow(PI,double(K/2.0)) * COV2(Sigma[k])) * exp(-0.5 *(((xOne-Mu[k]) * Sigma[k].niMatrix()) * (xOne-Mu[k]).transposeMatrix()).data[0][0]);
            w.data[k][i]=p;
            wNorm.data[0][i]+=p;
        }
        for(int k=0; k<w.row; k++)
        {
            w.data[k][i]/=wNorm.data[0][i];
        }
    }
    return w;
}

int M_step(const Matrix w, Matrix &x)
{
    Phi.clear();
    Mu.clear();
    Sigma.clear();
    Matrix mu;
    mu.initMatrix(1,x.col,0,"ss");
    Matrix sigma;
    sigma.initMatrix(x.col,x.col,0,"s");
    for(size_t k=0; k<w.row; k++)
    {
        Phi.push_back(0);
        for(size_t j=1; j<x.col; j++)
            mu.data[0][j]=0;
        for(size_t i=0; i<sigma.row; i++)
            for(size_t j=1; j<sigma.col; j++)
                sigma.data[i][j]=0;
        for(size_t i=0; i<x.row; i++)
        {
            Phi[k]+=w.data[k][i];//计算phi
            for(size_t j=0; j<x.col; j++)
                mu.data[0][j]+=w.data[k][i] * x.data[i][j];//计算mu
        }
        Phi[k]/=x.row;//更新phi
        for(size_t j=0; j<x.col; j++)
            mu.data[0][j]/=(Phi[k] * x.row);
        Mu.push_back(mu);//更新mu
        for(size_t i=0; i<x.row; i++)
        {
            for(size_t j=0; j<x.col; j++)
            {
                for(size_t j2=0; j2<x.col; j2++)
                {
                    sigma.data[j][j2]+=w.data[k][i] * (x.data[i][j]-mu.data[0][j]) * (x.data[i][j2]-mu.data[0][j2]);
                }
            }
        }
        for(size_t i=0; i<sigma.row; i++)
            for(size_t j=0; j<sigma.col; j++)
                sigma.data[i][j]/=(Phi[k] * x.row);
        Sigma.push_back(sigma);//更新Sigma
    }
    return 0;
}

int GMM()
{
    const int k=3;
    const int Iter=10;
    Matrix x;
    x.LoadData("data\\GMM.txt");
    Matrix sigma(x.col,x.col,5,"diag");//初始化方差
    Matrix mu(k,x.col,0,"ss");
    K=k;
    Matrix onek;
    for(int i=0; i<x.row; i++)
    {
        for(int j=0; j<x.col; j++)
            mu.data[i%K][j]+=x.data[i][j];
    }
    for(int k=0; k<K; k++)
    {
        Phi.push_back(1.0/K);
        Sigma.push_back(sigma);
        for(int j=0; j<x.col; j++)
            mu.data[k][j]=mu.data[k][j]/x.row *K;
        Mu.push_back(mu.getOneRow(k));
    }
    Matrix w;
    for(int it=0; it<Iter; it++)
    {
        cout<<"it==============="<<it<<endl;
        for(int i=0; i<K; i++)
        {
            cout<<Phi[i]<<endl;
            Mu[i].print();
            Sigma[i].print();
        }
        w=E_step(x);
        M_step(w,x);
    }
    return 0;
}

