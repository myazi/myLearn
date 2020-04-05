/***
回归树 CART
考虑
1最小均方差 ,拟合函数采用均值函数， 拟合函数采用线性模型
2特征阈值的选择是从逐个样本中选择，不是按步长选择
2属性类型（string）连续值
3预剪枝，后剪枝
**/

#include "CART.h"
#define MAX  100000000
namespace MLL
{
	CART::twoSubData CART::binSplitDataSet(const Data &data,const int &axis,const double &value)
	{
		twoSubData twosubdata;
		for(size_t i = 0; i < data.size(); i++)
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
	double CART::mean(const Data &data)
	{
		if(data.size()==0)
			return 0;
		double meanvalue = 0;
		int label_index = data[0].size()-1;
		for(size_t i = 0; i < data.size(); i++)
		{
			meanvalue += data[i][label_index];
		}
		meanvalue /= data.size();
		return meanvalue;
	}
	double CART::MeanErr(const Data &data)
	{
		if(data.size() == 0)
			return 0;
		double meanvalue = mean(data);
		double meanErr = 0;
		int label_index = data[0].size()-1;
		for(size_t i=0; i<data.size(); i++)
		{
			meanErr += (data[i][label_index] - meanvalue) * (data[i][label_index] - meanvalue);
		}
		meanErr = sqrt(meanErr / data.size());
		return meanErr;
	}
	CART::split CART::chooseBestSplit(const Data &data,const double &minErr)
	{
		size_t i = 0, j = 0;
		double oldMeanErr = MeanErr(data);
		double bestMeanErr = MAX;
		double newMeanErr = 0;
		int signvalue = 1;
		twoSubData twosubdata;
		split sp;
		int label_index = data[0].size() - 1;
		double value = data[0][label_index];
		for(i = 0; i < data.size(); i++)
		{
			if(data[i][label_index] != value)
			{
				signvalue = 0;
				break;
			}
		}
		if(signvalue)//所有数据值都一样则记为叶子节点不再分裂
		{
			std::cout<<"signvalue"<<std::endl;
			sp.bestIndex = -1;
			sp.value = mean(data);
			return sp;
		}
		for(i = 0; i < data[0].size() - 1; i++)
		{
			for(j = 0; j < data.size(); j++)
			{
				twosubdata = binSplitDataSet(data,i,data[j][i]);
				if(twosubdata.left.size() == 0 || twosubdata.right.size() == 0)
					continue;
				newMeanErr = MeanErr(twosubdata.left) + MeanErr(twosubdata.right);
				if(newMeanErr < bestMeanErr)
				{
					sp.bestIndex = i;
					sp.value = data[j][i];//j为样本数，i为特征数
					bestMeanErr = newMeanErr;
				}
			}
		}
		if(oldMeanErr - bestMeanErr<minErr)
		{
			sp.bestIndex = -1;
			sp.value = mean(data);
		}
		return sp;
	}
	int CART::createBinTree(bitree &t,Data &data)
	{
		if(data.size() == 0)
			return 0;
		if(!(t = (bitree)malloc(sizeof(bitnode)))) exit(-1);
		split sp = chooseBestSplit(data,0.001);
		t->feature = sp.bestIndex;
		t->meanValue = sp.value;
		//t->data = data;//为什么data不能赋值
		if(t->feature == -1)
		{
			t->left = NULL;
			t->right = NULL;
			std::cout<< "feat-1" <<std::endl;
			return 0;
		}
		//std::cout<<"feature="<<t->feature<<"  value="<<t->meanValue<<std::endl;
		twoSubData twosubdata = binSplitDataSet(data,sp.bestIndex,sp.value);
		createBinTree((t->left), twosubdata.left);
		createBinTree((t->right), twosubdata.right);
		return 0;
	}
	int CART::preorder(bitree &t)//递归先序遍历二叉树
	{
		if(t!=NULL)
		{
			if(t->feature==-1)
				std::cout<<t->feature<<"  "<<t->meanValue<<"  "<<std::endl;//<<t->data.size()<<std::endl;//只输出叶子节点
			if(t->left!=NULL)
				preorder(t->left);
			if(t->right!=NULL)
				preorder(t->right);
		}
		return 0;
	}
	int CART::prune(bitree &t,const Data &testData)
	{
		if(t == NULL || testData.size() == 0)//检测子树是否为NULL,不然后面的操作执行不了
			return 0;
		if(t->left->feature != -1 || t->right->feature != -1)
		{
			twoSubData twosubdata = binSplitDataSet(testData,t->feature,t->meanValue);
			if(t->left->feature != -1)
			{
				prune(t->left, twosubdata.left);
			}
			if(t->right->feature != -1)
			{
				prune(t->right, twosubdata.right);
			}
			if(t->right->feature == -1 && t->left->feature == -1)
			{
				prune(t,testData);
			}
		}
		else
		{
			twoSubData twosubdata = binSplitDataSet(testData, t->feature, t->meanValue);
			double errortwo = MeanErr(twosubdata.left) + MeanErr(twosubdata.right);
			double errorone = MeanErr(testData);
			if(errorone < errortwo)
			{
				std::cout<<"prune merge"<<std::endl;
				t->left = NULL;
				t->right = NULL;
				t->feature = -1;
				t->meanValue = mean(testData);
			}
			return 0;
		}
		return 0;
	}
	double CART::predict(bitree t, const RowData &data)//bitree &t，不能引用，不能改变树根
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
	CART::CART(const std::string &file)
	{
		Data data;
		LoadData(data,file);
		bitree t;
		createBinTree(t,data);
		preorder(t);
		Data testdata;
		LoadData(testdata,"data/carttest.txt");
		prune(t,testdata);
		preorder(t);
		std::vector<double> labels(testdata.size(),0);
		for(size_t i = 0; i < testdata.size(); i++)
		{
			labels[i] = predict(t,testdata[i]);
			std::cout<< labels[i] << "\t" << testdata[i][testdata[0].size()-1] << std::endl;
		}
	}
}
