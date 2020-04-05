/**
极大似然+Veiterbi算法
**/

#include "HMM_CWS.h"

namespace MLL
{
	int HMM_CWS::getPos(const std::string &str)
	{
		int i = 0;
		for(i = 0; i < dicos.len; i++)
		{
			if(!str.compare(dicos.dic[i]))
				return i;
		}
		std::cout<<str<<std::endl;
		std::cout<<"状态字典中不存在该状态"<<std::endl;
		return -1;
	}
	int HMM_CWS::getState(const std::string &str)
	{
		if(str == "0")
			return 0;
		if(str == "1")
			return 1;
		if(str == "2")
			return 2;
		if(str == "3")
			return 3;
		return 0;
	}
	/**
		映射策略对分词的质量影响非常之大
	**/
	int HMM_CWS::wordToState(const RowDataStr &str, int word, int j, int vl, std::string &state)
	{
		char ch;
		if(str[j].length()==2)
		{
			dicos.b[3][vl]++;
			state.append(1,'3');
			state.append(1,' ');
		}
		if(str[j].length()==4)
		{
			if(word == 0)
			{
				dicos.b[0][vl]++;
				state.append(1,'0');
				state.append(1,' ');
			}
			else
			{
				dicos.b[2][vl]++;
				state.append(1,'2');
				state.append(1,' ');
			}
		}
		if(str[j].length() == 6)
		{
			dicos.b[word / 2][vl]++;
			ch=(char)(word / 2 + 48);
			state.append(1, ch);
			state.append(1, ' ');
		}
		if(str[j].length()==8)
		{
			if(word == 0)
			{
				dicos.b[0][vl]++;
				state.append(1, '0');
				state.append(1, ' ');
			}
			if(word == 6)
			{
				dicos.b[2][vl]++;
				state.append(1, '2');
				state.append(1, ' ');
			}
			else
			{
				dicos.b[1][vl]++;
				state.append(1, '1');
				state.append(1, ' ');
			}
		}
	}
	void HMM_CWS::createVocabList(const DataStr &data)
	{
		std::ofstream ofile;
		std::ofstream ofile_arg;
		std::ofstream ofile_state;
		ofile.open("data/hmm_dic.txt");
		ofile_state.open("data/hmm_state.txt");
		int i,j,k,vl;
		int dic_len=0;
		int length=0;
		int word;
		std::string state;
		char ch;
		double sum;
		for(i = 0; i < data.size(); i++)
		{
			state = "";
			for(j = 0; j < data[i].size(); j++)
			{
				if(data[i][j].length() % 2 != 0 || data[i][j].length() > 9)//训练预料词长度检测,最大四字长
					continue;
				for(word = 0; word < data[i][j].length(); word += 2)
				{
					for(vl = 0; vl < dic_len; vl++)
					{
						if(!data[i][j].substr(word,2).compare(dicos.dic[vl]))
						{
							wordToState(data[i], word, j, vl, state);
							break;
						}
					}
					if(vl == dic_len)
					{
						dicos.dic[vl] = data[i][j].substr(word,2);//对字典进行扩展
						wordToState(data[i],word,j,vl,state);
						ofile<< data[i][j].substr(word,2) <<"  ";
						dic_len++;
					}
				}
			}
			state.append(1,'\n');
			ofile_state<<state;
		}
		ofile.close();
		ofile_state.close();
		dicos.len = dic_len;
		ofile_arg.open("data/hmm_arg.txt");
		for(j = 0; j < dicos.len; j++)
		{
			ofile_arg<<dicos.dic[j]<<"  ";
			sum = 0;
			for(i = 0; i < STATE; i++)
			{
				sum += dicos.b[i][j];
			}
			for(i = 0; i < STATE; i++)
			{
				dicos.b[i][j] /= sum;
				ofile_arg<< dicos.b[i][j] <<"  ";
			}
			ofile_arg<<'\n';
		}
		ofile_arg.close();
		std::cout<<"row="<<data.size()<<std::endl;
		std::cout<<"vec_len="<<dic_len<<std::endl;

		DataStr state_data;
		LoadData(state_data,"data/hmm_state.txt");
		for(i = 0; i < state_data.size(); i++)
		{
			dicos.pi[getState(state_data[i][0])]++;//统计参数pi
			for(j = 1; j < state_data[i].size(); j++)
			{
				dicos.a[getState(state_data[i][j - 1])][getState(state_data[i][j])]++;//统计参数a
			}
		}
		std::cout<< "i=" << i <<std::endl;
		std::cout<<"A:--------------"<<std::endl;
		for(i = 0; i < STATE; i++)
		{
			sum = 0;
			for(j = 0; j < STATE; j++)
			{
				sum += dicos.a[i][j];
			}
			for(j = 0; j < STATE; j++)
			{
				dicos.a[i][j] /= sum;
				std::cout<< dicos.a[i][j] <<"  ";
			}
			std::cout<<std::endl;
		}
		std::cout<<"PI:--------------"<<std::endl;
		sum = 0;
		for(i = 0; i < STATE; i++)
		{
			sum += dicos.pi[i];
		}
		for(i = 0; i < STATE; i++)
		{
			dicos.pi[i] /= sum;
			std::cout<< dicos.pi[i] <<"  ";
		}
		std::cout<<std::endl;
	}

	int HMM_CWS::Viterbi(const DataStr &testdata)
	{
		int t,i,j,k;
		int pos;
		double deta[VEC_LEN][STATE];
		int fai[VEC_LEN][STATE];
		double max_deta;
		double max_fai;
		int max_i;
		for(i = 0; i < STATE; i++)
		{
			pos = getPos(testdata[0][0]);
			deta[0][i] = dicos.pi[i] * dicos.b[i][pos];
			fai[0][i] = 0;
		}
		for(k = 0; k < testdata.size(); k++)
		{
			for(t = 1; t < testdata[k].size(); t++)
			{
				for(i = 0; i <STATE; i++)
				{
					max_deta = double_min;
					max_fai = double_min;
					for(j = 0; j < STATE; j++)
					{
						pos = getPos(testdata[k][t]);
						if(deta[t - 1][j] * dicos.a[j][i] * dicos.b[i][pos] > max_deta)
						{
							max_deta = deta[t-1][j] * dicos.a[j][i] * dicos.b[i][pos];
						}
						if(deta[t - 1][j] * dicos.a[j][i] > max_fai)
						{
							max_fai = deta[t - 1][j] * dicos.a[j][i];
							max_i = j;
						}
					}
					deta[t][i] = max_deta;
					fai[t][i] = max_i;
				}
			}
			max_deta = double_min;
			for(i = 0; i < STATE; i++)
			{
				if(deta[testdata[k].size() - 1][i] > max_deta)
				{
					max_deta = deta[testdata[k].size() - 1][i];
					max_i = i;
				}
			}
			std::cout<<max_i;
			for(t = testdata[k].size() - 2; t >= 0; t--)
			{
				max_deta = double_min;
				std::cout<<fai[t + 1][max_i];
				for(i = 0; i < STATE; i++)
				{
					if(deta[t][i] > max_deta)
					{
						max_deta = deta[t][i];
						max_i = i;
					}
				}
			}
			std::cout<<std::endl;
		}
	}
	void HMM_CWS::init_DICOS()
	{
		int i = 0, j = 0;
		for(i = 0; i < STATE; i++)
		{
			for(j = 0; j < VEC_LEN; j++)
			{
				dicos.b[i][j] = 0;
			}
			for(j = 0; j < STATE; j++)
			{
				dicos.a[i][j] = 0;
			}
		}
	}
	HMM_CWS::HMM_CWS(const std::string &file)
	{
		DataStr data;
		LoadData(data,file);
		init_DICOS();
		createVocabList(data);//创建状态字典，用于存放参数值
		DataStr testdata;
		LoadData(testdata,"data/hmm_test.txt");
		Viterbi(testdata);
	}
}
