#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "matrix.h"
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;
#define MAX 1000000
#define MIN -100000
//SMO参数结构体
struct OS
{
    Matrix x;
    Matrix y;
    double C;
    double soft;
    int m;
    Matrix alphas;
    double b;
    Matrix eCache;
    Matrix kernel;
    bool k;
};
//核函数的结构体
struct kTup
{
    int type;//0,1
    double arg;
};
class SMOP
{
    //非常值得注意的是svm中训练样本按列排列，即每一列是一个样本，所以导致w是行向量
public:
    OS os;
public:
    /**
    根据参数，来生成不同的核函数
    */
    Matrix kernelTran(Matrix x,Matrix xOneCol,kTup ktup)
    {
        Matrix K;
        K.initMatrix(&K,x.col,1);
        Matrix xOneColT;
        xOneColT.initMatrix(&xOneColT,xOneCol.row,xOneCol.col);
        xOneColT.transposematrix(xOneCol,&xOneColT);
        if(ktup.type==1)
        {
            K.multsmatrix(&K,x,xOneColT);
        }
        if(ktup.type==2)
        {
            //高斯核
        }
        return K;
    }
    /**
    结构体OS的初始化，用于保存所以SMO算法中需要用到的参数
    */
    int initOs(Matrix x,Matrix y,double C,double soft,kTup ktup)
    {
        os.x.initMatrix(&os.x,x.col,x.row);
        os.x.copy(x,&os.x);
        os.y.initMatrix(&os.y,y.col,y.row);
        os.y.copy(y,&os.y);
        os.eCache.initMatrix(&os.eCache,x.col,2);
        os.alphas.initMatrix(&os.alphas,x.col,1);

        os.b=0;
        os.C=C;
        os.m=x.col;
        os.kernel.initMatrix(&os.kernel,os.m,os.m);
        os.soft=soft;
        if(ktup.type!=0)
        {
            int i=0,j=0;
            Matrix xOneCol;
            xOneCol.initMatrix(&xOneCol,1,os.x.row);
            Matrix kOneRow;
            kOneRow.initMatrix(&kOneRow,os.m,1);
            cout<<"-----------"<<endl;
            for(i=0; i<os.m; i++)
            {
                xOneCol=xOneCol.getOneCol(x,i);
                kOneRow=kernelTran(x,xOneCol,ktup);
                for(j=0; j<os.m; j++)
                    os.kernel.mat[j][i]=kOneRow.mat[j][0];
            }
            os.k=true;
            return 0;
        }
        os.k=false;
        return 0;
    }
    /**
    上下限剪辑函数
    */
    double clipAlpha(double alpha,double L,double H)
    {
        if(alpha>H)
            return H;
        if(alpha<L)
            return L;
        return alpha;
    }
    /**
    误差值的计算也是根据数学表达式来实现的

    **/
    double calcEk(int k)
    {
        Matrix ay;
        ay.initMatrix(&ay,os.x.col,1);
        int i;
        for(i=0; i<os.m; i++)
        {
            ay.mat[i][0]=os.alphas.mat[i][0]*os.y.mat[i][0];
        }
        Matrix ayT;
        ayT.initMatrix(&ayT,ay.row,ay.col);
        ayT.transposematrix(ay,&ayT);
        Matrix f;
        f.initMatrix(&f,1,1);
        if(!os.k)
        {
            Matrix ayTx;
            ayTx.initMatrix(&ayTx,ayT.col,os.x.row);
            ayTx.multsmatrix(&ayTx,ayT,os.x);

            Matrix xOneCol;
            xOneCol.initMatrix(&xOneCol,1,os.x.row);
            xOneCol=xOneCol.getOneCol(os.x,k);

            Matrix xOneColT;
            xOneColT.initMatrix(&xOneColT,xOneCol.row,xOneCol.col);
            xOneColT.transposematrix(xOneCol,&xOneColT);
            f.multsmatrix(&f,ayTx,xOneColT);//值得商榷
        }
        else
        {
            Matrix kOneRow;
            kOneRow.initMatrix(&kOneRow,os.m,1);
            kOneRow=kOneRow.getOneRow(os.kernel,k);
            f.multsmatrix(&f,ayT,kOneRow);//值得商榷
        }
        double fXK=f.mat[0][0]+os.b-os.y.mat[k][0];
        return fXK;
    }
    ///更新误差矩阵
    void updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//计算误差值
        os.eCache.mat[k][0]=1;
        os.eCache.mat[k][1]=Ek;
    }
    /**
    对于第二个alpha的选择采用Ei-Ej的大小进行衡量，同时这里引用一个矩阵来保存不变的E，减少计算量
    选择出合适第二个alpha2，对于第一次进来，保存误差值E的矩阵必然没有一个有效的，所以采用随机策略
    随机选择策略其实可以任意选择，具体没有实现，而是采用等价方式实现，但需要注意数据越界问题
    **/
    int selectJ(int i,double Ei)
    {
        int maxK=-1;
        double Ek;
        double Ej;
        double deltaE;
        double maxDeltaE=0;
        int k;
        for(k=0; k<os.alphas.col; k++)
        {
            if(os.eCache.mat[k][0]==1&&k!=i)
            {
                Ek=calcEk(k);
                deltaE=abs(Ei-Ek);
                if(deltaE>maxDeltaE)
                {
                    maxK=k;
                    maxDeltaE=deltaE;
                    Ej=Ek;
                }
            }
            //随机选择一个j, 理论上只有第一次时所有的ek都没计算过，所以任选一个即可
            if(maxK==-1)
            {
                maxK=(i*2+1)%100;
                Ej=calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    内层循环实现两个alpha的优化，由外层循环函数提供alpha1的可选集合（所有样本或者是支持向量），
    遍历集合上的每一个alpha，对其进行判断是否违反KKT条件，如果违反则选为第一个alpha1，
    同样调用selctJ函数来启发式选择使得alpha1改变最大的alpha2，
    选择出合适ij之后，还得根据当前alpha1和alpha2来确定优化的上下限。
    然后通过迭代公式，来计算alpha1和alpha2，计算的时候判断是否采用核方法。
    并对计算结果进行上下限剪辑，最终确定优化后的alpha1和alpha2，同时对当前优化后结果对b也进行修改
    考虑到每一次选择第二个alpha2时，都需要比较一个误差值，而这个误差值在每次优化过程中都只有对应修改alpha的误差值有变化
    而其他的是不变的，所以用一个矩阵来保存有效的误差值。
    */
    int innerL(int i)
    {
        double Ei;
        double Ej;
        int j;
        double alphaIOld;
        double alphaJOld;
        double L;
        double H;
        double eta;
        int n;
        double temp[3];//ii，jj，ij
        double b1,b2;
        if(os.y.mat[i][0]*(Ei-os.y.mat[i][0]*os.soft)<0&&os.alphas.mat[i][0]<os.C||os.y.mat[i][0]*(Ei-os.y.mat[i][0]*os.soft)>0&&os.alphas.mat[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld=os.alphas.mat[i][0];
            alphaJOld=os.alphas.mat[j][0];
            if(os.y.mat[i][0]!=os.y.mat[j][0])
            {
                L=max(0.0,os.alphas.mat[j][0]-os.alphas.mat[i][0]);
                H=min(os.C,os.alphas.mat[j][0]-os.alphas.mat[i][0]+os.C);
            }
            else
            {
                L=max(0.0,os.alphas.mat[j][0]+os.alphas.mat[i][0]-os.C);
                H=min(os.C,os.alphas.mat[j][0]+os.alphas.mat[i][0]);
            }
            if(L==H)
            {
                cout<<"l=h------------------"<<endl;
                return 0;
            }
            if(!os.k)
            {
                temp[0]=0;
                temp[1]=0;
                temp[2]=0;
                for(n=0; n<os.x.row; n++)
                {
                    temp[0]+=pow(os.x.mat[i][n],2);
                    temp[1]+=pow(os.x.mat[j][n],2);
                    temp[2]+=os.x.mat[i][n]*os.x.mat[j][n];
                }
            }
            else
            {
                temp[0]=os.kernel.mat[i][i];
                temp[1]=os.kernel.mat[j][j];
                temp[2]=os.kernel.mat[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                cout<<"eta<0-----------------"<<endl;
                return 0;
            }
            os.alphas.mat[j][0]+=os.y.mat[j][0]*(Ei-Ej)/eta;
            os.alphas.mat[j][0]=clipAlpha(os.alphas.mat[j][0],L,H);//对alpha进行剪辑
            updataEk(j);//更新误差值
            if(fabs(os.alphas.mat[j][0]-alphaJOld)<0.000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            os.alphas.mat[i][0]+=os.y.mat[i][0]*os.y.mat[j][0]*(alphaJOld-os.alphas.mat[j][0]);
            updataEk(i);//更新误差值

            b1=os.b-Ei-os.y.mat[i][0]*(os.alphas.mat[i][0]-alphaIOld)*temp[0]-os.alphas.mat[j][0]*(os.alphas.mat[j][0]-alphaJOld)*temp[2];
            b2=os.b-Ej-os.y.mat[i][0]*(os.alphas.mat[i][0]-alphaIOld)*temp[2]-os.alphas.mat[j][0]*(os.alphas.mat[j][0]-alphaJOld)*temp[1];
            if(0<os.alphas.mat[i][0]&&os.C>os.alphas.mat[i][0])
                os.b=b1;
            else if(0<os.alphas.mat[j][0]&&os.C>os.alphas.mat[j][0])
                os.b=b2;
            else
                os.b=(b1+b2)/2;
            return 1;
        }
        cout<<"kkt--------------------------"<<endl;
        return 0;
    }

    /**

    SMO算法的入口函数，其主要功能是初始化SMO所有的参数到结构体OS
    确定迭代结束标志，并在所有样本和支持向量上循环的选择合适alpha1，调用inner确定alpha1，
    外层循环每一次是迭代优化一对alpha,内层循环才是正真实现迭代优化一对alpha
    最后，对smo算法的实现进行检查，通过alpha求解出w，b，并在训练样本上比较其预测值与实际值的差异。
    该办法只能检查SMO算法实现的正确性，不能检查SVM的性能。
    */

    int smoP(Matrix x,Matrix y,double C,double soft,int maxIter,kTup ktup)
    {
        int iter=0;
        int i,j;
        initOs(x,y,C,soft,ktup);///初始化OS结构体，OS结构体中定义了SMOP算法需要的大部分参数
        bool entireSet=true;//标志用于判断当前迭代是针对所有alpha，还是针对0-C之间的部分alpha，这里其实第一个alpha的启发式选择，即选择落在支持向量上的点
        int alphaPairsChanged=0;//该参数判断在确定第一个参数之后，是否能选择出符合要求的第二alpha，返回值为1表示能，0为不能
        for(iter=0; iter<maxIter&&(alphaPairsChanged>0||entireSet); iter++)
        {//循环结束标志为迭代次数已到预设值，或者是不能再继续优化（对于所有的支持向量上的点都找不到第二个alpha对第一个alpha进行优化后，重新再遍历所有的点寻找可优化的参数对）
            //还是找不到则再次遍历支持向量上的点，这次必然也是找不到，才结束迭代
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<os.m; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<endl;
                }
                iter++;
            }
            else
            {
                for(i=0; i<os.m; i++)
                {
                    if(os.alphas.mat[i][0]>0&&os.alphas.mat[i][0]<os.C)//只选择支持向量上的点
                        continue;
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<endl;
                }
                iter++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///对SMO算法实现的正确性进行验证，输出预测值与实际值的差别，全为0表示在训练样本上预测全对
        Matrix ay;
        ay.initMatrix(&ay,os.x.col,1);
        for(i=0; i<os.m; i++)
        {
            ay.mat[i][0]=os.alphas.mat[i][0]*os.y.mat[i][0];
        }

        Matrix xT;
        xT.initMatrix(&xT,os.x.row,os.x.col);
        xT.transposematrix(x,&xT);

        Matrix w;
        w.initMatrix(&w,os.x.row,1);
        w.multsmatrix(&w,xT,ay);

        Matrix label;
        label.initMatrix(&label,os.x.col,1);
        label.multsmatrix(&label,os.x,w);
        cout<<os.b<<"  ";
        for(i=0; i<os.x.row; i++)
        {
            cout<<w.mat[i][0]<<"  ";
        }
        cout<<endl;
        ///验证训练样本数据，验证SVM实现的正确性
        for(i=0; i<os.m; i++)
        {
            if((label.mat[i][0]+os.b)>0)
                cout<<1-os.y.mat[i][0]<<endl;
            else
                cout<<-1-os.y.mat[i][0]<<endl;
        }
        return 0;
    }

};
int main()
{
    ///加载数据文件保存到对象dtm的矩阵元素中
    dataToMatrix dtm;
    char file[20]="data\\svm.txt";
    dtm.loadData(&dtm,file);

    ///通过矩阵对象中的load函数初始化样本的特征和类别矩阵x,y
    Matrix x;
    x.loadMatrix(&x,dtm);
    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);
    x.deleteOneRow(&x,x.row);

    ///调用SMO函数求解SVM模型
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}
