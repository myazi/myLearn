#include "GMM.h"
#define PI 3.1415926

struct Par
{
    vector<double> phi;
    vector<Matrix> mu;
    vector<Matrix> Sigma;
    int K;
};

double COV2(Matrix Sigma)
{
    double cov=0;
    for(size_t i=0; i<Sigma.row; i++)
    {
        for(size_t j=0; j<Sigma.col; j++)
        {
            cov+=sqrt(Sigma.data[i][j]);
        }
    }
    //cout<<"cov="<<cov<<endl;
    return cov;
}

Matrix E_step(Matrix &x, Par &par)
{
    Matrix w;
    w.initMatrix(par.K,x.row,0,"s");
    Matrix wNorm;
    wNorm.initMatrix(1,x.row,0,"s");
    Matrix xOne;
    double p=0;
    for(size_t i=0; i<x.row; i++)
    {
        for(int k=0; k<w.row; k++)
        {
            xOne = x.getOneRow(i);
            p = par.phi[k] * 1.0 / (pow(PI,double(par.K/2.0)) * COV2(par.Sigma[k])) * exp(-0.5 *(((xOne-par.mu[k]) * par.Sigma[k].niMatrix()) * (xOne-par.mu[k]).transposeMatrix()).data[0][0]);
            w.data[k][i]=p;
            wNorm.data[0][i]+=p;
        }
        for(int k=0; k<w.row; k++)
        {
            w.data[k][i]/=wNorm.data[0][i];
            //cout<<w.data[k][i]<<"   ";
        }
        //cout<<endl;
    }
    return w;
}

int M_step(const Matrix w, Matrix &x, Par &par)
{
    par.phi.clear();
    par.mu.clear();
    par.Sigma.clear();
    Matrix mu;
    mu.initMatrix(1,x.col,0,"ss");
    Matrix Sigma;
    Sigma.initMatrix(x.col,x.col,0,"s");
    for(size_t k=0; k<w.row; k++)
    {
        par.phi.push_back(0);
        for(size_t j=1; j<x.col; j++)
                mu.data[0][j]=0;
        for(size_t i=0;i<Sigma.row;i++)
            for(size_t j=1; j<Sigma.col; j++)
                Sigma.data[i][j]=0;
        for(size_t i=0;i<x.row;i++)
        {
            par.phi[k]+=w.data[k][i];//计算phi
            for(size_t j=0; j<x.col; j++)
                mu.data[0][j]+=w.data[k][i] * x.data[i][j];//计算mu
        }
        par.phi[k]/=x.row;//更新phi
        for(size_t j=0;j<x.col;j++)
            mu.data[0][j]/=(par.phi[k] * x.row);
        par.mu.push_back(mu);//更新mu
        for(size_t i=0; i<x.row;i++)
        {
            for(size_t j=0;j<x.col;j++)
            {
                for(size_t j2=0;j2<x.col;j2++)
                {
                    Sigma.data[j][j2]+=w.data[k][i] * (x.data[i][j]-mu.data[0][j]) * (x.data[i][j2]-mu.data[0][j2]);
                }
            }
        }
        for(size_t i=0;i<Sigma.row;i++)
            for(size_t j=0; j<Sigma.col;j++)
                Sigma.data[i][j]/=(par.phi[k] * x.row);
        par.Sigma.push_back(Sigma);//更新Sigma
    }
    return 0;
}

int GMM(const int &K, const int &Iter)
{
    Matrix x;
    x.LoadData("data\\GMM.txt");
    Matrix Sigma;
    Sigma.initMatrix(x.col,x.col,10,"diag");//初始化方差为2
    Matrix mu;
    mu.initMatrix(K,x.col,0,"ss");
    Par par;
    par.K=K;
    for(size_t i=0; i<x.row; i++)
    {
        for(size_t j=0; j<x.col; j++)
            mu.data[i%K][j]+=x.data[i][j];
    }
    for(int k=0; k<K; k++)
    {
        par.phi.push_back(1.0/K);
        par.Sigma.push_back(Sigma);
        for(size_t j=0; j<mu.col; j++)
            mu.data[k][j]/x.row*K;
        par.mu.push_back(mu.getOneRow(k));
    }
    Matrix w;
    for(int it=0;it<Iter;it++)
    {
        cout<<"it==============="<<it<<endl;
        for(int i=0;i<K;i++)
        {
            cout<<par.phi[i]<<endl;
            par.mu[i].print();
            par.Sigma[i].print();
        }
        w=E_step(x,par);
        M_step(w,x,par);
    }
    return 0;
}

