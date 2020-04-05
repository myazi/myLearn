#ifndef kMeans_H
#define kMeans_H
#include "MatrixOpe.h"

namespace MLL 
{
	class KMeans
	{
		struct CenAndDis
		{
			Matrix cen;//聚类中心
			Matrix dis;//每一个样本到其聚类中心的距离，即为一个m行的列向量
		};
		private:
			Matrix _x;
			int _K;
		public:
			double distances(const Matrix &xOne, const Matrix &kCen);

			Matrix randCent(const Matrix &x,const int &kNum);

			CenAndDis kMeans(const Matrix &x, const int &kNum, const int &Iter);

			Matrix subMatrix(const Matrix &x, const Matrix &clusterAssment, const int &label);

			int biKmeans(const Matrix &x, const int &kNum, const int &Iter);

			KMeans(const std::string &file, const int &K);

			typedef std::shared_ptr<KMeans> KMeansPtr;
	};
}
#endif
