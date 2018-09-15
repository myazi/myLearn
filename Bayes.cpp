#include <io.h>
#include <string.h>
#include <stdio.h>
#include "Bayes.h"
#include "MatrixOpe.h"
#define CLASS_SUM 2
#define X_RANGE 2  //X_RANGE 表示X的取值范围，由于采用one-hot编码，所以X的每一个分量的取值范围为0或1

struct Par
{
    Matrix pX_1Y;
    Matrix pY;
    Matrix pX;
};

Par bayes;//保存bayes分类器所有的参数

DIC createVocabList(vector<DataStr> dataClass)//生成单词字典
{
    int i,j,k,vl;
    vector<string> dic;
    for(k=0; k<dataClass.size(); k++)
    {
        for(i=0; i<dataClass[k].size(); i++)
        {
            for(j=0; j<dataClass[k][i].size(); j++)
            {
                for(vl=0; vl<dic.size(); vl++)
                {
                    //cout<<dataClass[k][i][j]<<endl;
                    if(!dataClass[k][i][j].compare(dic[vl]))
                        break;
                }
                if(vl==dic.size())
                {
                    dic.push_back(dataClass[k][i][j]);
                }
            }
        }
    }
    return dic;
}

Matrix createFectVec(vector<DataStr> dataClass,DIC dic)//生成训练样本矩阵
{
    int i,j,k=0,vl;
    int sampleNum=0;
    for(k=0; k<dataClass.size(); k++)
    {
        sampleNum+=dataClass[k].size();
    }
    Matrix vecX;
    vecX.initMatrix(sampleNum,dic.size()+1,0,"ss");
    int iSample=0;
    for(k=0; k<dataClass.size(); k++)
    {
        for(i=0; i<dataClass[k].size(); i++)//只有一行
        {
            vecX.data[iSample][dic.size()]=k;//标签
            for(j=0; j<dataClass[k][i].size(); j++)
            {
                for(vl=0; vl<dic.size(); vl++)
                {
                    if(!dataClass[k][i][j].compare(dic[vl]))
                    {
                        vecX.data[iSample][vl]=1;//one-hot编码特征
                        break;
                    }
                }
            }
            iSample++;
        }
    }
    return vecX;
}

int trainNB(Matrix X,Matrix Y)//训练函数的实现，注意对参数进行平滑处理
{
    //类，特征，特征取值
    bayes.pY.initMatrix(CLASS_SUM,1,0,"ss");//两类初始化为2行的列向量
    bayes.pX_1Y.initMatrix(CLASS_SUM,X.col,0,"ss");//X_1Y表示在Y下X=1的概率，反之X=0的概率为1-
    bayes.pX.initMatrix(X.col,1,0,"ss");//

    int i,j,k;
    for(k=0; k<bayes.pX_1Y.row; k++)
    {
        for(i=0; i<bayes.pX_1Y.col; i++)
        {
            bayes.pX_1Y.data[k][i]=1;//平滑处理,默认出现一次，后期归一化时把特征向量的长度也考虑进去,这里的平滑是指每一类字典与整个字典的，未涉及测试
            //样本中的未登入词
        }
    }
    for(i=0; i<X.row; i++)
    {
        if(Y.data[i][0]==0)
        {
            bayes.pY.data[0][0]++;
            for(j=0; j<X.col; j++)
            {
                bayes.pX_1Y.data[0][j]+=X.data[i][j];
            }
        }
        else
        {
            bayes.pY.data[1][0]++;
            for(j=0; j<X.col; j++)
            {
                bayes.pX_1Y.data[1][j]+=X.data[i][j];
            }
        }
    }
    for(i=0; i<X.col; i++)
    {
        //所有类下x各个特征分量出现的概率
        //bayes.pX.data[i][0]=(bayes.pX_1Y.data[0][i]-1) + (bayes.pX_1Y.data[1][i]-1) + 1;
        //bayes.pX.data[i][0]/=bayes.pY.data[0][0] + bayes.pY.data[1][0] + 2;

        //某一类下x各个特征分量出现的概率
        bayes.pX_1Y.data[0][i]/=bayes.pY.data[0][0] + 2;
        bayes.pX_1Y.data[1][i]/=bayes.pY.data[1][0] + 2;

        bayes.pX.data[i][0] = (bayes.pX_1Y.data[0][i] + bayes.pX_1Y.data[1][i])/2;
    }
    //计算出PY两类的概率
    for(k=0; k<bayes.pY.row; k++)
    {
        bayes.pY.data[k][0]/=X.row;
    }
    cout<<"pY="<<bayes.pY.data[0][0]<<endl;

    for(k=0; k<bayes.pX_1Y.row; k++)
    {
        for(i=0; i<bayes.pX_1Y.col; i++)
        {
            cout<<bayes.pX_1Y.data[k][i]<<" & ";
        }
        cout<<"---";
    }
}

/**
分类决策函数
**/
int classifyNB(Matrix testVecX)
{
    double p0=1.0,p1=1.0;
    int i,j;
    for(i=0; i<testVecX.row; i++)
    {
        p0=1.0,p1=1.0;
        for(j=0; j<testVecX.col ; j++)
        {
            //特征分量不出现的概率为1-
            if(testVecX.data[i][j]==0)
            {
                p0*=(1-bayes.pX_1Y.data[0][j])/(1-bayes.pX.data[j][0]);
                p1*=(1-bayes.pX_1Y.data[1][j])/(1-bayes.pX.data[j][0]);
            }
            //特征分量出现的概率
            else
            {
                p0*=(bayes.pX_1Y.data[0][j])/bayes.pX.data[j][0];
                p1*=(bayes.pX_1Y.data[1][j])/bayes.pX.data[j][0];
            }
        }
        p0*=bayes.pY.data[0][0];
        p1*=bayes.pY.data[1][0];
        cout<<"p0="<<p0<<"&"<<"p1="<<p1;
        if(p0<p1)
            cout<<"class="<<1;
        else
            cout<<"class="<<0;
        cout<<endl;
    }
    return 0;

}
void getAllFiles(string path, vector <string>&files)
{
    //文件句柄
    long   hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        }
        while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
void Bayes()
{

    int k,i,j;
    string path[CLASS_SUM]= {"data\\email\\ham\\","data\\email\\spam\\"};
    vector<string> files;
    vector< vector<string> > filess;
    RowDataStr rowData;
    DataStr data;
    vector<DataStr> dataClass;
    for(k=0; k<CLASS_SUM; k++)
    {
        getAllFiles(path[k],files);
        filess.push_back(files);
        files.clear();
    }
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    for(k=0; k<filess.size(); k++)
    {
        for(i=0; i<filess[k].size(); i++)
        {
            LoadDataStr(rowData,filess[k][i].c_str());
            data.push_back(rowData);
            rowData.clear();
        }
        dataClass.push_back(data);
        data.clear();
    }
    DIC dic=createVocabList(dataClass);
    cout<<dic.size();
    /**
    生成样本矩阵
    **/
    Matrix X=createFectVec(dataClass,dic);

    /**
    生成样本标签
    **/
    Matrix Y;
    Y = X.getOneCol(X.col-1);
    X.deleteOneCol(X.col-1);
    cout<<X.row<<"*"<<X.col<<endl;
    cout<<Y.row<<"*"<<Y.col<<endl;
    Y.print();
    trainNB(X,Y);///通过样本矩阵和标签矩阵调用参数求解函数求解bayes决策的参数

    classifyNB(X);///分类决策

    //bayes.pX_1Y.print();
    cout<<"----------"<<endl;
    Matrix pxt=bayes.pX.transposeMatrix();
    pxt.print();
}

