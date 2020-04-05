#ifndef GMM_H
#define GMM_H
#include "MatrixOpe.h"
#define PI 3.1415926

namespace MLL
{
	class GMM
	{
		private:
			std::vector<double> Phi;
			std::vector<Matrix> Sigma;
			std::vector<Matrix> Mu;
			int K;

		public:

			double COV2(const Matrix &Sigma);

			Matrix E_step(const Matrix &x);

			int M_step(const Matrix &w, const Matrix &x);

			GMM(const std::string &file, const int &K, const int &Iter);
			
			typedef std::shared_ptr<GMM> GMMPtr;
	};
}
#endif
