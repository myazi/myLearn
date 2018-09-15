/**
Kmeans算法致命的就是聚成空类的出现，值得分析一下kmeans什么情况下会出现空类，
该程序中未对此作处理，后面肯定还是会遇到这个问题的
**/
#include "MatrixOpe.h"
#define MAX 1000000
#define MIN -100000
#define random(x) (rand()%x)
#define MAXK 10

/**
Kmeans聚类中心数据结构
*/
struct CenAndDis
{
    Matrix cen;//聚类中心
    Matrix dis;//每一个样本到其聚类中心的距离，即为一个m行的列向量
};

///返回一个样本与某一个聚类中心的距离
double distances(Matrix xOne,Matrix kCen)
{
    int i,j;
    double dis;
    for(i=0; i<xOne.col; i++)
    {
        dis=pow((xOne.data[0][i]-kCen.data[0][i]),2);
    }
    return dis;
}

/**
随机初始化一个聚类中心，初始化的方式有两种，一种是随机从样本中选一个作为聚类中心，
一种是在样本空间内初始化一个聚类中心，可以不是具体某个样本，类中心的初始化非常关键

下面采用的是从样本中选择作为聚类中心，从样本中选取聚类中心同样也会出现聚成空类的现象，对此解决办法是

**/
Matrix randCent(Matrix x,int kNum)
{
    int i,j,k;
    Matrix initmeans;
    initmeans.initMatrix(kNum,x.col,0,"ss");

    double min[2];
    double max[2];

    /**
    在样本空间内随机初始化k个类中心
    **/
    for(j=0; j<x.col; j++)
    {
        min[j]=MAX;
        max[j]=MIN;
        for(i=0; i<x.row; i++)
        {
            if(x.data[i][j]<min[j])
            {
                min[j]=x.data[i][j];
            }
            if(x.data[i][j]>max[j])
            {
                max[j]=x.data[i][j];
            }
        }
    }
    /**
    随机从样本中选择k个样本作为类中心
    */
    for(k=0; k<kNum; k++)
    {
        for(j=0; j<x.col; j++)
        {
            //改变初值的选取，得到的聚类中心会不一致，可见初值的选取的关键，
            initmeans.data[k][j]=x.data[k][j];//min[j]+(max[j]-min[j])*(random(10)/10.0);
            cout<<initmeans.data[k][j]<<"  ";
        }
        cout<<endl;
    }
    return initmeans;
}
CenAndDis kMeans(Matrix &x,const int &kNum, const int &Iter)
{
    int i,j,k,it;
    double dis[MAXK];///记录一个样本到k个类中心的距离，可以采用矩阵动态申请更好，这里默认最多MAXK个类中心

    Matrix kmeans;//聚类中心
    kmeans=randCent(x,kNum);//随机初始化k个类中心

    Matrix xOne;//存储一个样本
    Matrix kOne;//存储一个聚类中心样本
    Matrix kLabel;//存储每个样本的类别
    kLabel.initMatrix(x.row,1,0,"ss");

    double minDis=MAX;//最小的距离

    for(it=0; it<Iter; it++)
    {
        //根据k个聚类中心划分类标签
        for(i=0; i<x.row; i++)
        {
            xOne = x.getOneRow(i);
            minDis=MAX;
            for(k=0; k<kNum; k++)
            {
                kOne = kmeans.getOneRow(k);
                dis[k]=distances(xOne,kOne);
                if(dis[k]<minDis)
                {
                    kLabel.data[i][0]=k;
                    minDis=dis[k];
                }
            }
        }
        //kmeans清零，当计算类中心采用+=的形式则需要清零，而下面更新kmeans同样是采用+=的形式，因为右边有一项就是kmeans
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.col; j++)
            {
                kmeans.data[k][j]=0;
            }
        }
        //更新kmeans
        for(i=0; i<x.row; i++)
        {
            k=kLabel.data[i][0];
            for(j=0; j<x.col; j++)
            {
                kmeans.data[k][j]=(kmeans.data[k][j]*(i)+x.data[i][j])/(i+1);//采用累加方式求均值，该方法增加计算量
                //当然也可以把一类的所有样本取出来，求和再取均值，这里没有这样做，而是来一个加一个
            }
        }
        //输出当前次EM后的聚类中心
        cout<<"--------------------"<<endl;
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.col; j++)
            {
                cout<<kmeans.data[k][j]<<"  ";
            }
            cout<<endl;
        }
    }
    /**
    将聚类结果保存到结构体中
    */
    CenAndDis cendis;
    cendis.cen.initMatrix(kNum,x.col,0,"b");
    cendis.dis.initMatrix(x.row,1,0,"ss");
    cendis.cen = kmeans;

    ///保存所有样本到其类中心的距离到结构体中
    for(i=0; i<x.row; i++)
    {
        k=kLabel.data[i][0];
        xOne=x.getOneRow(i);
        kOne=kmeans.getOneRow(k);
        cendis.dis.data[i][0]=distances(xOne,kOne);
    }
    //
    double sum=0;
    for(i=0; i<x.row; i++)
    {
        sum+=cendis.dis.data[i][0];
    }
    cout<<"err="<<sum<<endl;
    return cendis;
}

Matrix subMatrix(const Matrix &x, const Matrix &clusterAssment,const int &label)
{
    int i=0,j=0,k=0;
    Matrix submatrix;
    submatrix = x;
    for(i=0; i<x.row; i++)
    {
        if( int(clusterAssment.data[i][0])==label)
        {
            for(j=0; j<x.col; j++)
            {
                submatrix.data[k][j]=x.data[i][j];
            }
            k++;
        }
    }
    submatrix.row=k;
    return submatrix;
}
/***
二分聚类的思想是每一次都采用一个典型的二类聚类，而在选择把那个子类进行二分的评价标准是SSE，
即分裂哪一类使得SSE最小就分裂哪一类，SSE是最小平方和误差，即使得聚类结果的距离最小平方和误差最小
就对哪一类进行二分，直到不再减少SSE，或者有一类为空类，或者到达预设的聚为K类

**/
int biKmeans(Matrix &x,const int &kNum,const int &Iter)
{
    int i,j,k,d;
    Matrix kmeans;
    kmeans.initMatrix(kNum,x.col,0,"ss");///初始化聚为一类，

    Matrix xOne;//一个样本
    Matrix kOne;//一个聚类中心
    Matrix clusterAssment;///矩阵的第一列保存其到所属类别，第二列保存样本到所属类别的距离
    clusterAssment.initMatrix(x.row,2,0,"ss");

    CenAndDis cenanddis;///聚类结果
    cenanddis.cen.initMatrix(kNum,x.col,0,"ss");
    cenanddis.dis.initMatrix(x.col,1,0,"ss");

    CenAndDis bestCenanddis;///记录当前最好的距离结果
    bestCenanddis.cen.initMatrix(kNum,x.col,0,"ss");
    bestCenanddis.dis.initMatrix(x.row,1,0,"ss");

    for(i=0; i<x.row; i++)
    {
        for(j=0; j<x.col; j++)
        {
            kmeans.data[0][j]=(kmeans.data[0][j]*i+x.data[i][j])/(i+1);
        }
        clusterAssment.data[i][0]=0;///初始化聚为一类，类中心为之前初始化为一类的类中心
    }
    for(i=0; i<x.row; i++)
    {
        xOne=x.getOneRow(i);
        clusterAssment.data[i][1]=distances(xOne,kmeans);///初始化为一类后，所有样本到其所属类的距离
    }

    Matrix submatrix;///分裂后的子集矩阵
    submatrix = x;

    double lowestSSE=MAX;///记录当前聚类结果的SSE值
    double sseSplit=0;///记录分裂后子集的SSE值
    double sseNosplit=0;///记录没有分裂的自己的SSE值
    int bestCentToSplit;///记录预分裂子集的类别
    double dis[2];///记录分裂后两个子集的SSE值
    for(d=1; d<kNum; d++)
    {
        lowestSSE=MAX;//如果这个初始距离设置成当前不分裂下的总距离，那么不能保证一定得到kNum个类中心
        for(k=0; k<d; k++)
        {
            submatrix=subMatrix(x,clusterAssment,k);///用于保存属于k类的子集用于下面kmeans尝试进行划分
            cenanddis=kMeans(submatrix,2,10);///尝试用kmeans进行二分
            sseSplit=0;
            sseNosplit=0;
            /**
            分裂类别的SSE值和
            */
            for(i=0; i<submatrix.row; i++)
            {
                sseSplit+=cenanddis.dis.data[i][1];
            }
            /**
            未分裂的类别的SSE值和
            */
            for(i=0; i<x.row; i++)
            {
                if(clusterAssment.data[i][0]!=k)
                {
                    sseNosplit+=clusterAssment.data[i][1];
                }
            }
            ///与最小的SSE进行比较，选择出最佳的分裂类
            if(sseSplit+sseNosplit<lowestSSE)//如果将第k个簇进行而划分更好，则记录下来，并更新lowsetSSE
            {
                bestCentToSplit=k;
                bestCenanddis=cenanddis;
                lowestSSE=sseSplit+sseNosplit;
            }
        }
        ///最后确定选出最好的二划分簇之后，下面开始在x数据上进行更新聚类中心和距离，clusterAssment变量
        for(i=0; i<x.row; i++)
        {
            if(clusterAssment.data[i][0]==bestCentToSplit)
            {
                xOne=x.getOneRow(i);
                for(k=0; k<2; k++)
                {
                    kOne=bestCenanddis.cen.getOneRow(k);
                    dis[k]=distances(kOne,xOne);
                }
                if(dis[0]<dis[1])
                {
                    clusterAssment.data[i][0]=bestCentToSplit;///分裂后的两个类，第一个类的类别还是当前类别值
                    clusterAssment.data[i][1]=dis[0];
                }
                else
                {
                    clusterAssment.data[i][0]=bestCentToSplit+1;///分裂后的两个类，第二个类的类别分配一个新的类别值，一直往上加1
                    clusterAssment.data[i][1]=dis[1];
                }
            }
        }
        ///计算最终的SSE值
        double sum=0;
        for(i=0; i<x.row; i++)
        {
            sum+=clusterAssment.data[i][1];
        }
        cout<<"err==="<<sum<<endl;
    }
}

int kmeans()
{
    Matrix x;
    x.LoadData("data\\kmeans.txt");
    kMeans(x,3,10);
    biKmeans(x,3,10);
}

