#include "MEMM_CWS.h"

namespace MLL
{
	int MEMM_CWS::getPos(const std::string &str)
	{
		int i = 0;
		for(i = 0; i < dicos.len; i++)
		{
			if(!str.compare(dicos.dic[i]))
				return i;
		}
		std::cout<<"状态字典中不存在该状态"<<std::endl;
		return -1;
	}

	int MEMM_CWS::getState(const std::string &str)
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

	int MEMM_CWS::wordToState(const RowDataStr &str, int word, int j, int vl, std::string &state)
	{
		char ch;
		if(str[j].length() == 2)
		{
			dicos.pos[vl][3]++;
			state.append(1,'3');
			state.append(1,' ');
		}
		if(str[j].length() == 4)
		{
			if(word == 0)
			{
				dicos.pos[vl][0]++;
				state.append(1,'0');
				state.append(1,' ');
			}
			else
			{
				dicos.pos[vl][2]++;
				state.append(1,'2');
				state.append(1,' ');
			}
		}
		if(str[j].length() == 6)
		{
			dicos.pos[vl][word/2]++;
			ch=(char)(word / 2 + 48);
			state.append(1,ch);
			state.append(1,' ');
		}
		if(str[j].length() == 8)
		{
			if(word == 0)
			{
				dicos.pos[vl][0]++;
				state.append(1,'0');
				state.append(1,' ');
			}
			if(word == 6)
			{
				dicos.pos[vl][2]++;
				state.append(1,'2');
				state.append(1,' ');
			}
			else
			{
				dicos.pos[vl][1]++;
				state.append(1,'1');
				state.append(1,' ');
			}
		}
	}

	void MEMM_CWS::createVocabList(const DataStr &data)
	{
		std::ofstream ofile;
		std::ofstream ofile_arg;
		std::ofstream ofile_state;
		ofile.open("data/memm_dic.txt");
		ofile_state.open("data/memm_state.txt");
		int i,j,k,vl;
		int dic_len = 0;
		int word;
		std::string state;
		char ch;
		double sum;
		/**
		字典建立
		*/
		for(i = 0; i < data.size() - 1; i++)
		{
			state = "";
			for(j = 0; j < data[i].size() - 1; j++)
			{
				if(data[i][j].length() % 2 != 0 || data[i][j].length() > 9)
					continue;
				for(word = 0; word < data[i][j].length(); word += 2)
				{
					for(vl = 0; vl < dic_len; vl++)
					{
						if(!data[i][j].substr(word,2).compare(dicos.dic[vl]))
						{
							wordToState(data[i], word, j, vl, state);//p是句子，j是第几个词，word是词中的第几个字，vl是字在字典中的位置
							break;
						}
					}
					if(vl == dic_len)
					{
						dicos.dic[vl] = data[i][j].substr(word,2);//对字典进行扩展
						wordToState(data[i],word,j,vl,state);
						ofile<<data[i][j].substr(word,2)<<"  ";
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
		std::cout<<"row="<<data.size()<<std::endl;
		std::cout<<"vec_len="<<dic_len<<std::endl;

		/**
		参数估计
		**/

		ofile_arg.open("data/memm_arg.txt");
		for(i = 0; i < dicos.len; i++)
		{
			ofile_arg<<dicos.dic[i]<<"  ";
			sum = 0;
			for(j = 0; j < STATE; j++)
			{
				sum += dicos.pos[i][j];
			}
			for(j = 0; j < STATE; j++)
			{
				dicos.pos[i][j] /= sum;
				ofile_arg<<dicos.pos[i][j]<<"  ";
			}
			ofile_arg<<'\n';
		}
		ofile_arg.close();

		DataStr state_data;
		LoadData(state_data, "data/memm_state.txt");
		for(i = 0; i < state_data.size(); i++)
		{
			dicos.ps0[getState(state_data[i][0])]++;//统计参数ps0
			for(j = 1; j < state_data[i].size(); j++)
			{
				dicos.pss[getState(state_data[i][j-1])][getState(state_data[i][j])]++;//统计参数pss
			}
		}
		std::cout<< "i=" << i <<std::endl;
		std::cout<<"pss:--------------"<<std::endl;
		for(i = 0; i < STATE; i++)
		{
			sum = 0;
			for(j = 0; j < STATE; j++)
			{
				sum += dicos.pss[i][j];
			}
			for(j = 0; j < STATE; j++)
			{
				dicos.pss[i][j] /= sum;
				std::cout<<dicos.pss[i][j]<<"  ";
			}
			std::cout<<std::endl;
		}
		std::cout<<"PI:--------------"<<std::endl;
		sum = 0;
		for(i = 0; i < STATE; i++)
		{
			sum += dicos.ps0[i];
		}
		for(i = 0; i < STATE; i++)
		{
			dicos.ps0[i] /= sum;
			std::cout<<dicos.ps0[i]<<"  ";
		}
		std::cout<<std::endl;

	}

	int MEMM_CWS::Viterbi(const DataStr &testdata)
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
			deta[0][i] = dicos.ps0[i]*dicos.pos[pos][i];
			fai[0][i] = 0;
		}
		for(k = 0; k < testdata.size(); k++)
		{
			for(t = 1; t < testdata[k].size(); t++)
			{
				for(i = 0; i < STATE; i++)
				{
					max_deta = double_min;
					max_fai = double_min;
					for(j = 0; j < STATE; j++)
					{
						pos = getPos(testdata[k][t]);
						if(deta[t - 1][j] * dicos.pss[j][i] * dicos.pos[pos][i] > max_deta)
						{
							max_deta = deta[t - 1][j] * dicos.pss[j][i] * dicos.pos[pos][i];
						}
						if(deta[t - 1][j] * dicos.pss[j][i] > max_fai)
						{
							max_fai = deta[t - 1][j]*dicos.pss[j][i];
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
				std::cout<<fai[t+1][max_i];
				for(i = 0; i < STATE; i++)
				{
					if(deta[t][i] > max_deta)
					{
						max_deta = deta[t][i];
						max_i=i;
					}
				}
			}
			std::cout<<std::endl;
		}
	}
	void MEMM_CWS::init_DICOS()
	{
		int i = 0, j = 0;
		for(i = 0; i < STATE; i++)
		{
			for(j = 0; j < VEC_LEN; j++)
			{
				dicos.pos[j][i]=0;
			}
			for(j = 0; j < STATE; j++)
			{
				dicos.pss[i][j]=0;
			}
		}
	}
	MEMM_CWS::MEMM_CWS(const std::string &file)
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

