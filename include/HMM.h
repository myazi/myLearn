#ifndef HMM_H
#define HMM_H
#include "MatrixOpe.h"
#define VEC_LEN 10000
#define STATE 3
#define T 1000
#define double_min -1

namespace MLL 
{
	class HMM
	{
		struct HMM_Pra
		{
			std::string dic[VEC_LEN];
			double arg[VEC_LEN];//词频
			double a[STATE][STATE];//参数A
			double b[STATE][VEC_LEN];//参数B
			double pi[STATE];//参数pi
			int len = 0;
		};
		private:
			double alpha[T][STATE];
			double bata[T][STATE];
			double epsion[T][STATE][STATE];
			double Epsion[STATE][STATE];
			double Gam[STATE];
			double Gamvk[STATE][T];
			double sum[STATE];
			double sumall = 0;
			HMM_Pra hmm_pra;
		public:
			void createOset(const DataStr &data);
			int getPos(const std::string str);
			void init_test();
			void forwardBack(const DataStr &data);
			void Baum_Weach(const DataStr &data);
			void Viterbi(const DataStr &data);
			HMM(const std::string file);
		typedef std::shared_ptr<HMM> HMMPtr;
	};
}
#endif
