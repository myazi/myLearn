#ifndef MEMM_CWS_H
#define MEMM_CWS_H
#include "MatrixOpe.h"
#define VEC_LEN 5000
#define STATE 4
#define double_min -1

namespace MLL
{
	class MEMM_CWS
	{
		struct DICOS
		{
			std::string dic[VEC_LEN];//观测值字典
			double pos[VEC_LEN][STATE];//参数p(si|oi)
			double pss[STATE][STATE];//参数p(si|si-1)
			double ps0[STATE];//参数p(s0)            //p(s1|o1,s0)=p(s1|o1)*p(s0)=p(s1|o1)*p(s0)
			int len = 0;
		};
		private:
			DICOS dicos;
		public:
			int getPos(const std::string &str);
			int getState(const std::string &str);
			int wordToState(const RowDataStr &str,int word,int j,int vl,std::string &state);
			void createVocabList(const DataStr &data);
			int Viterbi(const DataStr &testdata);
			void init_DICOS();
			MEMM_CWS(const std::string &file);
			typedef std::shared_ptr<MEMM_CWS> MEMM_CWSPtr;
	};
}
#endif
