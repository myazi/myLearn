#include "MatrixOpe.h"
#define MAX_SIZE_OF_TRAINING_SET 1000
#define MAX 100000

namespace MLL {

	class DTree
	{
		struct Tree
		{
			int id = 0;
			int split_feat = 0;//分裂特征
			std::string feat_value = "0";//特征的值
			std::string label = "0";//树的类别
			int size = 0;
			int kind = 0;
			int parent = 0;
			Tree *next;
		};
		private:
			Tree tree[MAX_SIZE_OF_TRAINING_SET];//用作存储后序遍历生成树的序列
			std::stack<Tree> s;
			int node = 0;//用作存储后序遍历生成树的序列数组的下标
			std::vector<int> feat_flag;//初始化为0,最后一个用于统计还剩多少特征未使用
			DataStr data;

		public:
			std::vector<std::string> getkindattr(const DataStr &data, const int &axis);//获得axis属性的所有不同取值

			double calcShannonEntOrGini(const DataStr &data, const std::string &type);

			DataStr splitDataSet(const DataStr &data,const int &axis, const std::string &value);

			int chooseBestFectureTosplit(const DataStr &data, const std::string &type, const double &epsilon, const int &minLen);

			Tree dataToTree(const DataStr &data,const std::string &type, const int &bbestFet);

			int createTree();

			DTree(const std::string &file, const std::string &type);
			
			typedef std::shared_ptr<DTree> DTreePtr;
	};
}
