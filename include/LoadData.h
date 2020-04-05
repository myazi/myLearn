#ifndef LoadData_H
#define LoadData_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <math.h>
#include <stdlib.h>

/**
加载数值型样本数据
*/

typedef std::vector<std::vector<float> > Data;
typedef std::vector<float> RowData;
typedef std::vector<float> ColData;

typedef std::vector<std::vector<std::string> > DataStr;
typedef std::vector<std::string> RowDataStr;
typedef std::vector<std::string> ColDataStr;


template<typename elemType>
void LoadData(std::vector<std::vector<elemType> > &data, const std::string &infile)
{
    std::cout << "loaddata filename is:" << infile <<std::endl;
    std::ifstream dataFile;
    dataFile.open(infile);
    std::string tmpstrline;
    elemType val;
	std::vector<elemType> rowData;
    if(dataFile)
    {
        while(!dataFile.eof())
        {
            getline(dataFile,tmpstrline,'\n');//读取文件中一行的数据，保存为string类型
            //char buffer[256];
            //dataFile.getline(buffer,100);//读取文件中一行的数据，保存为string类型
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
    else
    {
        std::cout<< "open data file fail" <<  std::endl;    
    }
    dataFile.close();
}

void LoadDataStr(RowDataStr &data, const std::string &infile);
void LoadData_spare(Data &data, const std::string &infile);

#endif // LoadData_H
