#include "MatrixOpe.h"

/**
加载数值型样本数据
*/
typedef std::vector<std::vector<float> > Data;  //定义为模板更好
typedef std::vector<float> RowData;
typedef std::vector<float> ColData;

typedef std::vector<std::vector<std::string> > DataStr;  //定义为模板
typedef std::vector<std::string> RowDataStr;
typedef std::vector<std::string> ColDataStr;

void LoadDataNum(Data &data, const char *infile)
{
    std::cout<<infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    float val;
    RowData rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    rowData.push_back(val);
                }
                data.push_back(rowData);
            }
            rowData.clear();
        }
    }
    dataFile.close();
}
void LoadDataStr(DataStr &data, const char *infile)
{
    std::cout<<infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    std::string val;
    RowDataStr rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    rowData.push_back(val);
                }
                data.push_back(rowData);
            }
            rowData.clear();
        }
    }
    dataFile.close();
}
void LoadDataStr(RowDataStr &data, const char *infile)
{
    std::cout<<infile<<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    std::string val;
    RowDataStr rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            std::stringstream input(tmpstrline);
            if(tmpstrline!="\0")
            {
                while(input>>val)
                {
                    data.push_back(val);
                }
            }
        }
    }
    dataFile.close();
}
