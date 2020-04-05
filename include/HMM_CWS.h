#ifndef HMM_CWS_H
#define HMM_CWS_H
#include "MatrixOpe.h"
#define VEC_LEN 5000
#define STATE 4
#define double_min -1

namespace MLL 
{
	class HMM_CWS
	{
		struct DICOS
		{
			std::string dic[VEC_LEN];
			double a[STATE][STATE];//参数A
			double b[STATE][VEC_LEN];//参数B
			double pi[STATE];//参数pi
			int len;
		};
		private:
			DICOS dicos;
		public:
			int getPos(const std::string &str);
			int getState(const std::string &str);
			int wordToState(const RowDataStr &str,int word,int j,int vl, std::string &state);
			void createVocabList(const DataStr &data);
			int Viterbi(const DataStr &testdata);
			void init_DICOS();
			HMM_CWS(const std::string &file);
		typedef std::shared_ptr<HMM_CWS> HMM_CWSPtr;
	};
}
#endif
