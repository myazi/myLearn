#ifndef CRF_CWS_H
#define CRF_CWS_H
#include "MatrixOpe.h"
#define VEC_LEN 8000 //最大字典数
#define CI_LEN 1000000
#define INDEX 8
#define STATE 4
#define QJBJ 30
#define ZM 54
#define BD 30
#define double_max 10000
#define ADD_ONE 0.03
#define MUL 2

namespace MLL
{
	class CRF_CWS
	{
		typedef struct Word3
		{
			short int word;
			Word3 *next;
			int arg[INDEX*2];
			bool flag;
		}*three;

		/**
		连续两个字中的第二个字
		*/
		typedef struct Word2
		{
			short int word;
			Word2 *next;
			int arg[INDEX];
			bool flag;
			three word3;
		}*two;
		/**
		连续两个字中的第一个字
		**/
		typedef struct DICOS
		{
			short int word;
			DICOS *next;
			int arg[INDEX*2];
			bool flag;
			two word2;
		}*one;
		private:
			std::string dic_ci[CI_LEN];
			int dic_ci_num[CI_LEN];
			int dic_ci_len=0;
			std::string dic[VEC_LEN];///utf8字典
			std::string dic_ansi[VEC_LEN];///ansi字典
			int dic_len=2;
			DICOS *dicos;
			int NONO=0;
			int word_no=0;
			int word12_no=0;
			int word12_yes=0;
			int word123_yes=0;
			int stateTostate[INDEX];
			double stateTostate_two[INDEX][INDEX];
			int no_state[INDEX]= {0,0,0,0,0,0,0,0};
			//double ADD_ONE=0.01;
			//double MUL=2;
			bool flag=0;
			std::string bd[BD]= {"，","。","“","”","：","、","《","》","（","）","？","『","』","℃","●","！","·",":","‘","’","—",",","×","/","(",")","[","]"};
			std::string zm[ZM]= {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
						"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"
			};
		public:
			int getPos(const std::string &str);
			std::string getWord(const int &pos);
			int getState(const char &str);
			int senToword(const std::string &sen,std::string *word);
			int stateToindex(const int &state1,const int &state2);
			int stateToindex3(const int &state1,const int &state2,const int &state3);
			void createVocabList(const DataStr &data);
			void bdzm(const std::string &pre_word, const std::string &next_word, double *word_times);
			int FenCi(DataStr &testdata);
			int PingCe(const DataStr &resultdata,const DataStr &golddata);
			CRF_CWS(const std::string &file);
			typedef std::shared_ptr<CRF_CWS> CRF_CWSPtr;
	};
}
#endif

