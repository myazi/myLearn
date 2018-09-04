#ifndef LoadData_H
#define LoadData_H
#include <iostream>
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

typedef std::vector<std::vector<float> > Data;  //定义为模板更好
typedef std::vector<float> RowData;
typedef std::vector<float> ColData;

typedef std::vector<std::vector<std::string> > DataStr;  //定义为模板
typedef std::vector<std::string> RowDataStr;
typedef std::vector<std::string> ColDataStr;

void LoadDataNum(Data &data, const char *infile);
void LoadDataStr(DataStr &data, const char *infile);
void LoadDataStr(RowDataStr &data, const char *infile);

#endif // LoadData_H
