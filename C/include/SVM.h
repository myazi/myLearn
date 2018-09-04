#include "MatrixOpe.h"
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
    double arg;//核函数参数
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
    Matrix kernelTran(Matrix x,Matrix xOneRow,kTup ktup)
    {
        Matrix K(x.row,1,0,"ss");
        Matrix xOneRowT = xOneRow.transposeMatrix();
        if(ktup.type==1)
        {
            K = K.multsMatrix(x,xOneRowT);
        }
        if(ktup.type==2)
        {
            //高斯核
        }
        return K.transposeMatrix();
    }
    /**
    结构体OS的初始化，用于保存所以SMO算法中需要用到的参数
    */
    int initOs(Matrix x,Matrix y,double C,double soft,kTup ktup)
    {
        //os.x.initMatrix(x.col,x.row,0,"ss");
        os.x = x;
        //os.y.initMatrix(y.col,y.row,0,"ss");
        os.y = y;
        os.eCache.initMatrix(x.row,2,0,"ss");
        os.alphas.initMatrix(x.row,1,0,"ss");

        os.b=0;
        os.C=C;
        os.m=x.row;
        os.kernel.initMatrix(os.m,os.m,0,"s");
        os.soft=soft;
        if(ktup.type!=0)
        {
            int i=0,j=0;
            Matrix xOneRow(1,os.x.col,0,"ss");
            Matrix kOneCol(os.m,1,0,"ss");
            for(i=0; i<os.m; i++)
            {
                xOneRow=x.getOneRow(i);
                kOneCol=kernelTran(x,xOneRow,ktup);
                for(j=0; j<os.m; j++)
                    os.kernel.data[i][j]=kOneCol.data[0][j];
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
        Matrix ay(os.x.row,1,0,"ss");
        size_t i;
        for(i=0; i<ay.row; i++)
        {
            ay.data[i][0]=os.alphas.data[i][0]*os.y.data[i][0];
        }
        Matrix ayT =ay.transposeMatrix();
        Matrix f(1,1,0,"ss");
        if(!os.k)
        {
            Matrix ayTx =ayTx.multsMatrix(ayT,os.x);
            Matrix xOneRow = os.x.getOneRow(k);
            Matrix xOneRowT = xOneRow.transposeMatrix();
            f = f.multsMatrix(ayTx,xOneRowT);//值得商榷
        }
        else
        {
            Matrix kOneCol = os.kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOneCol);//值得商榷
        }
        double fXK=f.data[0][0]+os.b-os.y.data[k][0];
        return fXK;
    }
    ///更新误差矩阵
    void updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//计算误差值
        os.eCache.data[k][0]=1;
        os.eCache.data[k][1]=Ek;
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
        size_t k;
        for(k=0; k<os.alphas.col; k++)
        {
            if(os.eCache.data[k][0]==1&&k!=i)
            {
                Ek=calcEk(k);
                deltaE=fabs(Ei-Ek);
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
        double Ei=0;
        double Ej=0;
        int j;
        double alphaIOld;
        double alphaJOld;
        double L;
        double H;
        double eta;
        int n;
        double temp[3];//ii，jj，ij
        double b1,b2;
        if(os.y.data[i][0]*(Ei-os.y.data[i][0]*os.soft)<0&&os.alphas.data[i][0]<os.C||os.y.data[i][0]*(Ei-os.y.data[i][0]*os.soft)>0&&os.alphas.data[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld=os.alphas.data[i][0];
            alphaJOld=os.alphas.data[j][0];
            if(os.y.data[i][0]!=os.y.data[j][0])
            {
                L=max<double>(0.0,os.alphas.data[j][0]-os.alphas.data[i][0]);
                H=min(os.C,os.alphas.data[j][0]-os.alphas.data[i][0]+os.C);
            }
            else
            {
                L=max<double>(0.0,os.alphas.data[j][0]+os.alphas.data[i][0]-os.C);
                H=min<double>(os.C,os.alphas.data[j][0]+os.alphas.data[i][0]);
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
                    temp[0]+=pow(os.x.data[i][n],2);
                    temp[1]+=pow(os.x.data[j][n],2);
                    temp[2]+=os.x.data[i][n]*os.x.data[j][n];
                }
            }
            else
            {
                temp[0]=os.kernel.data[i][i];
                temp[1]=os.kernel.data[j][j];
                temp[2]=os.kernel.data[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                cout<<"eta<0-----------------"<<endl;
                return 0;
            }
            os.alphas.data[j][0]+=os.y.data[j][0]*(Ei-Ej)/eta;
            os.alphas.data[j][0]=clipAlpha(os.alphas.data[j][0],L,H);//对alpha进行剪辑
            updataEk(j);//更新误差值
            if(fabs(os.alphas.data[j][0]-alphaJOld)<0.00000000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            os.alphas.data[i][0]+=os.y.data[i][0]*os.y.data[j][0]*(alphaJOld-os.alphas.data[j][0]);
            updataEk(i);//更新误差值

            b1=os.b-Ei-os.y.data[i][0]*(os.alphas.data[i][0]-alphaIOld)*temp[0]-os.alphas.data[j][0]*(os.alphas.data[j][0]-alphaJOld)*temp[2];
            b2=os.b-Ej-os.y.data[i][0]*(os.alphas.data[i][0]-alphaIOld)*temp[2]-os.alphas.data[j][0]*(os.alphas.data[j][0]-alphaJOld)*temp[1];
            if(0<os.alphas.data[i][0]&&os.C>os.alphas.data[i][0])
                os.b=b1;
            else if(0<os.alphas.data[j][0]&&os.C>os.alphas.data[j][0])
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
        int i;
        initOs(x,y,C,soft,ktup);///初始化OS结构体，OS结构体中定义了SMOP算法需要的大部分参数
        bool entireSet=true;//标志用于判断当前迭代是针对所有alpha，还是针对0-C之间的部分alpha，这里其实第一个alpha的启发式选择，即选择落在支持向量上的点
        int alphaPairsChanged=0;//该参数判断在确定第一个参数之后，是否能选择出符合要求的第二alpha，返回值为1表示能，0为不能
        for(iter=0; iter<maxIter&&(alphaPairsChanged>0||entireSet); iter++)
        {
            //循环结束标志为迭代次数已到预设值，或者是不能再继续优化（对于所有的支持向量上的点都找不到第二个alpha对第一个alpha进行优化后，重新再遍历所有的点寻找可优化的参数对）
            //还是找不到则再次遍历支持向量上的点，这次必然也是找不到，才结束迭代
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<os.m; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            else
            {
                for(i=0; i<os.m; i++)
                {
                    if(os.alphas.data[i][0]>0&&os.alphas.data[i][0]<os.C)//只选择支持向量上的点
                        continue;
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<alphaPairsChanged<<"  entireSet="<<entireSet<<endl;
                }
                iter++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///对SMO算法实现的正确性进行验证，输出预测值与实际值的差别，全为0表示在训练样本上预测全对
        Matrix ay(os.x.row,1,0,"ss");
        for(i=0; i<os.m; i++)
        {
            ay.data[i][0]=os.alphas.data[i][0]*os.y.data[i][0];
        }

        Matrix xT = x.transposeMatrix();

        Matrix w = w.multsMatrix(xT,ay);

        Matrix label = label.multsMatrix(os.x,w);
        cout<<os.b<<"  ";
        for(i=0; i<os.x.col; i++)
        {
            cout<<w.data[i][0]<<"  ";
        }
        cout<<endl;
        cout<<"-----------"<<endl;
        ///验证训练样本数据，验证SVM实现的正确性
        for(i=0; i<os.m; i++)
        {
            if((label.data[i][0]+os.b)>0)
                cout<<1-os.y.data[i][0]<<"  ";
            else
                cout<<0-os.y.data[i][0]<<"  ";
        }
        return 0;
    }

};
int SVM();


