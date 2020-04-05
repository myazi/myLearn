#ifndef Adaboost_H
#define Adaboost_H
#include "MatrixOpe.h"
namespace MLL 
{
	class Adaboost
	{
		struct twoSubData
		{
			Data left;
			Data right;
		};
		struct Stump
		{
			double minErr = 0 ;
			int bestIndex = 0;//表示最好的分类属性，当非叶子节点时，即表示分裂属性下标，否则为-1，表示为叶子节点标记
			std::string ltOrgt = "lt";//大于阈值的类别为-1，还是小于阈值的类别为-1
			double threshVal = 0;
			std::vector<int> label;
			double alpha = 1.0;//基学习器投票权重
			twoSubData twosubdata;//树桩包括两个分支的数据
		};
		private:
			std::vector<Stump> stump;
			Data data;
			int numIt;
		public:
			double* rangeSize(const int &axis);
			std::vector<int> Classify(const int &axis, const double &threshVal, const std::string &threshIneq);
			Stump buildStump(std::vector<double> &weight);
			int AdaboostTrain();
			Adaboost(const std::string &filename, const int &numIt);
			typedef std::shared_ptr<Adaboost> AdaboostPtr;
	};
}
#endif
