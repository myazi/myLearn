#ifndef Bayes_H
#define Bayes_H
#include "MatrixOpe.h"
#include <memory>
namespace MLL
{
	class Bayes
	{
		struct Par
		{
			Matrix pX_1Y;
			Matrix pY;
			Matrix pX;
		};
		Par bayes;//保存bayes分类器所有的参数
		
		public:

			typedef std::vector<std::string> DIC;
			
			DIC createVocabList(const std::vector<DataStr> &dataClass);//生成单词字典

			Matrix createFectVec(const std::vector<DataStr> &dataClass, const DIC &dic);//生成训练样本矩阵

			int trainNB(const Matrix &X, const Matrix &Y);//训练函数的实现，注意对参数进行平滑处理

			int classifyNB(const Matrix &testVecX);//分类决策函数

			void getAllFiles(const std::string &path, std::vector <std::string> &files);

			Bayes(const std::string &file);

			typedef std::shared_ptr<Bayes> BayesPtr;
	};
}
#endif

