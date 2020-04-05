#ifndef KNN_H
#define KNN_H

#include "MatrixOpe.h"
namespace MLL 
{
	class KNN
	{
		private:
            Matrix _x;
            Matrix _y;
			int _K;
		public:
			int autoNorm(Matrix &x);

			Matrix cdistances(const Matrix &test, const Matrix &x);

			Matrix getK(const Matrix &oneTest, const Matrix &x);

			int classfiy(Matrix &testData,const Matrix &testDatay, Matrix &x, const Matrix &y);

			KNN(const std::string &file, const int &K);

			typedef std::shared_ptr<KNN> KNNPtr;
	};
}
#endif
