/***
决策树，ID3,C4.5,CART
考虑
1信息增益,信息增益率,基尼指数
2属性类型（string）
3预剪枝
**/

#include "DTree.h"
namespace MLL {
	
	std::vector<std::string> DTree::getkindattr(const DataStr &data,const int &axis)//获得axis属性的所有不同取值
	{
		size_t i = 0, j = 0;
		std::vector<std::string> attr_value;
		for(i = 0; i < data.size(); i++)
		{
			for(j = 0; j < attr_value.size(); j++)
			{
				if(data[i][axis] == attr_value[j])
					break;
			}
			if(j == attr_value.size())
			{
				attr_value.push_back(data[i][axis]);
			}
		}
		return attr_value;
	}

	double DTree::calcShannonEntOrGini(const DataStr &data, const std::string &type)
	{
		size_t i = 0;
		std::map<std::string,double> label_value;//每一类样本的个数统计
		double prob = 0;
		double shannoEnt = 0;
		double Gini = 1;
		std::string label = "1";
		for(i = 0; i < data.size(); i++)
		{
			label = data[i][data[0].size()-1];
			if(label_value.count(label))
				label_value[label] += 1;//如果类别已初始化则在之前的基础上加1
			else
				label_value[label] = 1;//如果类别未初始化，则把当前类别下样本数初始化为1
		}
		//统计该子数据集上的所有样本的决策属性以及不同决策属性上样本数
		for(std::map<std::string,double>::iterator it = label_value.begin(); it != label_value.end(); it++)
		{
			prob = it->second/data.size();
			shannoEnt -= prob*log(prob);
			Gini -= prob*prob;
		}
		//cout<<"shannoEnt="<<shannoEnt<<endl; //检测信息熵计算是否正确
		if(type == "Gini")
			return Gini;
		if(type == "Ent")
			return shannoEnt;
		return shannoEnt;
	}

	DataStr DTree::splitDataSet(const DataStr &data,const int &axis, const std::string &value)
	{
		DataStr subData;
		int i = 0;
		for(i = 0; i < data.size(); i++)
		{
			if(data[i][axis] == value)
			{
				subData.push_back(data[i]);
			}
		}
		return subData;
	}

	int DTree::chooseBestFectureTosplit(const DataStr &data, const std::string &type, const double &epsilon, const int &minLen)
	{
		int i = 0, j = 0, k = 0;
		double baseEnt = 0;
		double baseGini = 0;
		baseEnt = calcShannonEntOrGini(data,"Ent");
		baseGini = calcShannonEntOrGini(data,"Gini");
		int bestFeature = -1;
		double newEnt = 0;
		double newGini = 0;
		double bestInfoGain = 0;
		double bestgainRatio = 0;
		double bestGini = 0;
		double infoGain = 0;
		double splitInfo = 0;
		double gainRatio = 0;
		double giniGain = 0;
		double prob = 0;
		double len = 0;
		DataStr subData;
		std::vector<std::vector<std::string> > feat_value(data[0].size()-1);
		for(i = 0; i < data.size(); i++)//初始化每个属性的不同取值的二维数组
		{
			for(j = 0; j < data[0].size() - 1; j++)
			{
				if(feat_flag[j] != 1)
					feat_value[j] = getkindattr(data,j);
				else
					feat_value[j].push_back("");
			}
		}
		for(i = 0; i < data[0].size() - 1; i++)
		{
			if(feat_flag[i] != 1)
			{
				std::cout<<"---------"<<"feat="<<i<<"--------"<<std::endl;
				newEnt = 0;
				newGini = 0;
				splitInfo = 0;
				for(j = 0; j < feat_value[i].size(); j++)
				{
					subData = splitDataSet(data,i,feat_value[i][j]);
					if(subData.size() < minLen)
					{
						newGini = MAX;
						newEnt = MAX; //结束本次循环后，把此次的信息熵置为MAX，即不选择该属性
						break; //预剪枝操作
					}
					prob = double(subData.size())/double(data.size());
					splitInfo+=(-log(prob)*prob);//计算特征的信息熵
					std::cout<<"splitInfo="<<splitInfo<<std::endl;
					newEnt+=prob*calcShannonEntOrGini(subData,"Ent");
					newGini+=prob*calcShannonEntOrGini(subData,"Gini");
				}
				infoGain = baseEnt - newEnt; //计算信息增益
				gainRatio = infoGain / splitInfo; //计算信息增益率
				giniGain = baseGini - newGini; //计算Gini增率
				if(type == "ID3")
				{
					std::cout<< "fet=" << i << "infogain=" << infoGain <<std::endl;
					if(infoGain>bestInfoGain&&infoGain>epsilon)//信息增益必须大于阈值，才考虑用于属性分裂
					{
						bestInfoGain = infoGain;
						bestFeature = i;
					}
				}
				if(type == "C4.5")
				{
					std::cout<< "fet=" << i << "gainRatio=" << gainRatio <<std::endl;
					if(gainRatio > bestgainRatio&&gainRatio>epsilon)//信息增益必须大于阈值，才考虑用于属性分裂
					{
						bestgainRatio = gainRatio;
						bestFeature = i;
					}
				}
				if(type == "CART")
				{
					std::cout<< "fet=" << i << "giniGain=" << giniGain <<std::endl;
					if(giniGain > bestGini && giniGain > epsilon)//信息增益必须大于阈值，才考虑用于属性分裂
					{
						bestGini = giniGain;
						bestFeature = i;
					}
				}
			}
		}
		std::cout << "bestFet=" << bestFeature <<std::endl;
		return bestFeature;
	}

	DTree::Tree DTree::dataToTree(const DataStr &data,const std::string &type, const int &bbestFet)
	{
		int i = 0, j = 0;
		int signalflag = 1;//所有样本是否同类别，特征是否全用完标志
		int bestFet = 0;
		for(i = 0; i < data.size(); i++)
		{
			if(data[i][data[0].size()-1] != data[0][data[0].size()-1])
				signalflag=0;
		}
		if(signalflag)//只有一类样本
		{
			tree[node].label = data[0][data[0].size()-1];//类别设为第一个样本的label
			tree[node].split_feat = -1;
			tree[node].feat_value = "";
			tree[node].parent = bbestFet;
			tree[node].size = data.size();
			tree[node].id = node;
			std::cout<< "fetflag=1----" << tree[node].label << std::endl;
			node++;
			return tree[node];
		}
		for(i = 0; i < data[0].size() - 1; i++)
			feat_flag[data[0].size()-1] += feat_flag[i];
		if(feat_flag[data[0].size()-1] == feat_flag.size()-1)//所有属性都使用完了
		{
			tree[node].label = data[0][data[0].size()-1];//类别设为第一个样本的label
			tree[node].split_feat = -1;
			tree[node].feat_value = "";
			tree[node].parent = bbestFet;
			tree[node].size = data.size();
			tree[node].id = node;
			std::cout<< "fetflag=1----" << tree[node].label <<std::endl;
			node++;
			return tree[node];
		}
		bestFet = chooseBestFectureTosplit(data, type, 0.01, 1);// epsilon = 0.01, 1是节点最小元素个数
		if(bestFet == -1)//如果最大的信息增益小于预设的最小信息增益或者是子树中样本数小于预设的最小数也返回
		{
			tree[node].label = data[0][data[0].size()-1]; //这里设为第一个样本的label
			tree[node].split_feat = -1;
			tree[node].feat_value = "";
			tree[node].parent = bbestFet;
			tree[node].size = data.size();
			tree[node].id = node;
			std::cout<< "fetflag=1----" << tree[node].label <<std::endl;
			node++;
			return tree[node];
		}
		feat_flag[bestFet] = 1;//选出最好的分裂属性之后，在返回分裂之前，不必再考虑用该属性进行分裂
		std::vector<std::string> feat_value = getkindattr(data,bestFet);//获得当前分裂属性有多少种取值
		for(j = 0; j < feat_value.size(); j++)//分裂后的子集按序建树，同样也可以先对样本多的子集先进行建树，方便决策的时候能更大概率的先找到匹配的
		{
			tree[node] = dataToTree(splitDataSet(data,bestFet, feat_value[j]), type, bestFet);
		}
		feat_flag[bestFet] = 0;//每次返回上一节点时，把之前分裂的属性从新回收，可以用作下次分裂
		tree[node].feat_value = data[0][data[0].size()-1]; //这里设置为第一个样本的label
		tree[node].split_feat = bestFet;
		tree[node].parent = bbestFet;
		tree[node].label = "-1";
		tree[node].size = data.size();
		tree[node].id = node;
		node++;
		return tree[node];
	}
	int DTree::createTree()
	{
		int i;
		int len;
		Tree *child;
		Tree *p;
		for(i=0;i<node;i++)
		{
			if(tree[i].split_feat == -1)
			{
				s.push(tree[i]);
			}
			else
			{
				len = tree[i].size;
				p = &tree[i];
				while((len))
				{
					len -= s.top().size;
					child = new Tree;
					*child = s.top();
					s.pop();
					p->next = child;
					p = p->next;
				}
				s.push(tree[i]);
			}
		}
		s.pop();
		return 0;
	}

	DTree::DTree(const std::string &file, const std::string &type)
	{
		LoadData(data,file);
		for(size_t i = 0; i < data[0].size(); i++)
			feat_flag.push_back(0);
		dataToTree(data,type,0);//初值很重要，当返回到根节点时，如取-1，则会无法访问根节点上的属性特征值
		createTree();
		Tree *p;
		for(int i = 0; i<node; i++)
		{
			p = &tree[i];
			while(p != NULL)
			{
				std::cout<< p->id << "  ";
				p=p->next;
			}
			std::cout<<std::endl;
		}
	}
}

