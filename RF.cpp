/***

采用CART构成森林

**/

#include "RF.h"
#define MAX 10000000
#define MIN -1

namespace MLL
{
	RF::twoSubData RF::binSplitDataSet(const Data &data,const int &axis, const double &value)
	{
		twoSubData twosubdata;
		int i = 0, j = 0, k = 0;
		for(i = 0; i < data.size(); i++)
		{
			if(data[i][axis] <= value)
			{
				twosubdata.left.push_back(data[i]);
			}
			else
			{
				twosubdata.right.push_back(data[i]);
			}
		}
		return twosubdata;
	}
	double RF::mean(const Data &data)
	{
		if(data.size() == 0)
			return 0;
		double meanvalue = 0;
		int label_index = data[0].size() - 1;
		for(size_t i = 0; i < data.size(); i++)
		{
			meanvalue += data[i][label_index];
		}
		meanvalue /= data.size();
		return meanvalue;
	}
	double RF::MeanErr(const Data &data)
	{
		if(data.size() == 0)
			return 0;
		double meanvalue = mean(data);
		double meanErr = 0;
		int label_index = data[0].size() - 1;
		for(size_t i = 0; i < data.size(); i++)
		{
			meanErr += (data[i][label_index] - meanvalue) * (data[i][label_index] - meanvalue);
		}
		meanErr = sqrt(meanErr / data.size());
		return meanErr;
	}
	RF::split RF::chooseBestSplit(const Data &data,const int &deep, const double &minErr, const int &minLen)
	{
		int signvalue = 1;
		twoSubData twosubdata;
		split sp;
		int i = 0, j = 0;
		double oldMeanErr = MeanErr(data);
		double bestMeanErr = MAX;
		double newMeanErr = 0;
		double value = data[0][data[0].size() - 1];
		for(i = 0; i < data.size(); i++)
		{
			signvalue = 1;
			if(data[i][data[0].size() - 1] != value)
			{
				signvalue = 0;
				break;
			}
		}
		if(signvalue)//所有值都相等是成立
		{
			std::cout<<"signvalue"<<std::endl;
			sp.bestIndex = -1;
			sp.value = mean(data);
			return sp;
		}
		for(i = 0; i < deep && i < data[0].size() - 1; i++)
		{
			for(j = 0; j < data.size(); j++)
			{
				twosubdata = binSplitDataSet(data, i, data[j][i]);
				if(twosubdata.left.size() == 0 || twosubdata.right.size() == 0)
					continue;
				newMeanErr = MeanErr(twosubdata.left) + MeanErr(twosubdata.right);
				//std::cout<<"id="<<j<<"  newMeanErr="<<newMeanErr<<std::endl;
				if(newMeanErr < bestMeanErr)
				{
					sp.bestIndex = i;
					sp.value = data[j][i];
					bestMeanErr = newMeanErr;
				}
			}
		}
		//std::cout<<"value="<<sp.value<<"  index="<<sp.bestIndex<<std::endl;
		if(oldMeanErr - bestMeanErr < minErr || oldMeanErr - bestMeanErr < MIN)
		{
			sp.bestIndex = -1;
			sp.value = mean(data);
			//std::cout<<"minErr"<<std::endl;
			return sp;
		}
		//std::cout<<sp.bestIndex<<"&"<<sp.value<<"  ";
		//std::cout<<oldMeanErr<<"&"<<bestMeanErr<<std::endl;
		return sp;
	}
	int RF::createBinTree(bitree &t,const Data &data, const int &deep, const int &epsilon)
	{
		if(!(t=(bitnode *)malloc(sizeof(bitnode)))) exit(-1);
		split sp = chooseBestSplit(data, deep, epsilon, 10);// 10为节点最小长度，但实际未用到
		std::cout<< "index=" << sp.bestIndex <<std::endl;
		t->feature = sp.bestIndex;
		t->meanValue = sp.value;
		if(t->feature == -1)
		{
			t->left = NULL;
			t->right = NULL;
			std::cout<< "feat-1" <<std::endl;
			return 0;
		}
		else
		{
			std::cout<< "feature=" << t->feature << "\t" << "value=" << t->meanValue <<std::endl;
			twoSubData twosubdata = binSplitDataSet(data, sp.bestIndex,sp.value);
			createBinTree((t->left), twosubdata.left, deep, epsilon);
			createBinTree((t->right), twosubdata.right, deep, epsilon);
		}
		return 0;
	}
	int RF::preorder(bitree &t)//递归先序遍历二叉树
	{
		if(t != NULL)
		{
			if(t->feature == -1)
				std::cout<< t->feature << "\t" << t->meanValue<< "\t" <<std::endl;//<<t->data.size()<<std::endl;//只输出叶子节点
			if(t->left != NULL)
				preorder(t->left);
			if(t->right != NULL)
				preorder(t->right);
		}
		return 0;
	}
	double RF::predict(bitree t, const RowData &data)//bitree &t，不能引用，不能改变树根
	{
		while(t)
		{
			if(t->feature == -1)
				return t->meanValue;
			else
			{
				if(data[t->feature] <= t->meanValue)
					t = t->left;
				else
					t = t->right;
			}
		}
		return 0;
	}
	Data RF::Bootstrap(const Data &data)
	{
		Data bdata;
		int smp = 0;
		for(size_t i=0;i<data.size();i++)
		{
			smp = rand() % data.size();
			bdata.push_back(data[smp]);
		}
		return bdata;
	}
	RF::RF(const std::string &file, const int &numIt,const int &deep,const int &epsilon)
	{
		Data data;
		Data bdata;
		LoadData(data,file);
		bitree *t = NULL;
		if(!(t=(bitree*)malloc(sizeof(bitree)*numIt))) exit(-1);
		for(int i = 0; i < numIt; i++)
		{
			std::cout<< "It=" << i+1 << "---------------------" <<std::endl;
			bdata = Bootstrap(data);
			createBinTree(t[i],bdata,deep,epsilon);
			preorder(t[i]);
		}
		Data testdata;
		LoadData(testdata,"data/rftest.txt");
		std::vector<double> labels(testdata.size(),0);
		std::vector<double> bs(numIt,0);
		for(size_t i =0; i < testdata.size(); i++)
		{
			for(size_t it = 0; it < numIt; it++)
			{
				bs[it] = predict(t[it],testdata[i]);
				labels[i] += bs[it];
			}
			labels[i] /= double(numIt);
			std::cout<< labels[i] << "&&" << testdata[i][testdata[0].size() - 1] <<std::endl;
		}
	}
}
