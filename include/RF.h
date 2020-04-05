#ifndef RF_H
#define RF_H

#include "MatrixOpe.h"

namespace MLL
{
	class RF
	{
		struct twoSubData
		{
			Data left;
			Data right;
		};
		struct split
		{
			int bestIndex;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
			double value;//若为分裂节点，则表示分裂阈值，否则为叶子节点，用来记录叶子节点的均值
		};
		typedef struct bitnode
		{
			struct bitnode *left;//小于等于阈值的左子树
			struct bitnode *right;//大于阈值的右子树
			int feature;//只有非叶子节点才有分裂属性，叶子节点feature=-1
			double meanValue;//如果是叶子节点为值型,非叶子节点为阈值
			Data data;//每颗树都存储了该树下的数据
		} bitnode,*bitree;
		public:
			twoSubData binSplitDataSet(const Data &data, const int &axis, const double &value);

			double mean(const Data &data);

			double MeanErr(const Data &data);

			split chooseBestSplit(const Data &data, const int &deep, const double &minErr, const int &minLen);

			int createBinTree(bitree &t,const Data &data, const int &deep, const int &epsilon);

			int preorder(bitree &t);//递归先序遍历二叉树

			double predict(bitree t, const RowData &data);//bitree &t，不能引用，不能改变树根

			Data Bootstrap(const Data &data);

			RF(const std::string &file, const int &numIt, const int &deep, const int &epsilon);

			typedef std::shared_ptr<RF> RFPtr;
	};
}
#endif
