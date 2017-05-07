#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;
#define MAX_SIZE_OF_TRAINING_SET 1000
#define ATTR_NUM 1000


struct Data
{
    //可以用共用体类型
    int id;
    double attr_double[ATTR_NUM];//用于数值型属性
    string attr_string[ATTR_NUM];//用于数值型属性
    double weight;
    Data *next;
};
class dataToMatrix
{
public:
    Data *dataSet;
    int col;
    int row;
public:

    /**
    加载数值型样本数据
    */
    int loadData(dataToMatrix *dtm,char *file)
    {
        int i=0,j=0;
        ifstream infile;
        string tmpstrline;
        Data *p;
        dtm->dataSet=new Data;
        dtm->dataSet->next=NULL;
        p=dtm->dataSet;
        Data *datatmp;
        dtm->col=0;
        cout<<file<<endl;
        infile.open(file,ios::in);
        while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
        {
            getline(infile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            stringstream input(tmpstrline);
            if(tmpstrline!="\0")////由于读取文件结束符同样会继续该操作
            {
                datatmp=new Data;
                datatmp->id=i;
                datatmp->next=NULL;
                j=0;
                while(input>>datatmp->attr_double[j])j++;
                p->next=datatmp;
                p=p->next;
                dtm->col++;
            }
        }
        dtm->row=j;
        infile.close();
        return 0;
    }
    /**
    加载字符型样本数据
    */
    int loadData(dataToMatrix *dtm,char *file,int type)
    {
        int i=0,j=0;
        ifstream infile;
        string tmpstrline;
        Data *p;
        dtm->dataSet=new Data;
        dtm->dataSet->next=NULL;
        p=dtm->dataSet;
        Data *datatmp;
        dtm->col=1;
        cout<<file<<endl;
        infile.open(file,ios::in);
        datatmp=new Data;
        datatmp->next=NULL;
        while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
        {
            getline(infile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            stringstream input(tmpstrline);
            if(tmpstrline!="\0")////由于读取文件结束符同样会继续该操作
            {
                while(input>>datatmp->attr_string[j]){j++;};
            }
        }
        p->next=datatmp;
        dtm->row=j;
        infile.close();
        return 0;
    }
    int print(dataToMatrix dtm)
    {
        //检测数据加载是否正确
        int i,j;
        Data *p=dtm.dataSet->next;
        for(i=0; i<dtm.col&&p!=NULL; i++)
        {
            for(j=0; j<dtm.row; j++)
            {
                if(p->attr_string[j]!="")
                    cout<<p->attr_string[j]<<"  ";
                else
                    cout<<p->attr_double[j]<<"  ";
            }
            p=p->next;
            cout<<endl;
        }
        return i;
    }
};
