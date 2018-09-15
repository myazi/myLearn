#include "MatrixOpe.h"
#define VEC_LEN 5000
#define STATE 4
#define double_min -1
using namespace std;

struct DICOS_M
{
    string dic[VEC_LEN];//观测值字典
    double pos[VEC_LEN][STATE];//参数p(si|oi)
    double pss[STATE][STATE];//参数p(si|si-1)
    double ps0[STATE];//参数p(s0)            //p(s1|o1,s0)=p(s1|o1)*p(s0)=p(s1|o1)*p(s0)
    int len;
};
DICOS_M dicos_m;


int getPos(string str)
{
    int i=0;
    for(i=0; i<dicos_m.len; i++)
    {
        if(!str.compare(dicos_m.dic[i]))
            return i;
    }
    cout<<"状态字典中不存在该状态"<<endl;
    return -1;
}

int getState_M(string str)
{
    if(str=="0")
        return 0;
    if(str=="1")
        return 1;
    if(str=="2")
        return 2;
    if(str=="3")
        return 3;
    return 0;
}

int wordToState_M(const RowDataStr &str,int word,int j,int vl,string &state)
{
    char ch;
    if(str[j].length()==2)
    {
        dicos_m.pos[vl][3]++;
        state.append(1,'3');
        state.append(1,' ');
    }
    if(str[j].length()==4)
    {
        if(word==0)
        {
            dicos_m.pos[vl][0]++;
            state.append(1,'0');
            state.append(1,' ');
        }
        else
        {
            dicos_m.pos[vl][2]++;
            state.append(1,'2');
            state.append(1,' ');
        }
    }
    if(str[j].length()==6)
    {
        dicos_m.pos[vl][word/2]++;
        ch=(char)(word/2+48);
        state.append(1,ch);
        state.append(1,' ');
    }
    if(str[j].length()==8)
    {
        if(word==0)
        {
            dicos_m.pos[vl][0]++;
            state.append(1,'0');
            state.append(1,' ');
        }
        if(word==6)
        {
            dicos_m.pos[vl][2]++;
            state.append(1,'2');
            state.append(1,' ');
        }
        else
        {
            dicos_m.pos[vl][1]++;
            state.append(1,'1');
            state.append(1,' ');
        }
    }
}

void createVocabList_M(const DataStr &data)
{
    ofstream ofile;
    ofstream ofile_arg;
    ofstream ofile_state;
    ofile.open("data\\memm_dic.txt");
    ofile_state.open("data\\memm_state.txt");
    int i,j,k,vl;
    int dic_len=0;
    int word;
    string state;
    char ch;
    double sum;
    /**
    字典建立
    */
    for(i=0; i<data.size()-1; i++)
    {
        state="";
        for(j=0; j<data[i].size()-1; j++)
        {
            if(data[i][j].length()%2!=0||data[i][j].length()>9)
                continue;
            for(word=0; word<data[i][j].length(); word+=2)
            {
                for(vl=0; vl<dic_len; vl++)
                {
                    if(!data[i][j].substr(word,2).compare(dicos_m.dic[vl]))
                    {
                        wordToState_M(data[i],word,j,vl,state);//p是句子，j是第几个词，word是词中的第几个字，vl是字在字典中的位置
                        break;
                    }
                }
                if(vl==dic_len)
                {
                    dicos_m.dic[vl]=data[i][j].substr(word,2);//对字典进行扩展
                    wordToState_M(data[i],word,j,vl,state);
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
    dicos_m.len=dic_len;
    cout<<"row="<<data.size()<<endl;
    cout<<"vec_len="<<dic_len<<endl;

    /**
    参数估计
    **/

    ofile_arg.open("data\\memm_arg.txt");
    for(i=0; i<dicos_m.len; i++)
    {
        ofile_arg<<dicos_m.dic[i]<<"  ";
        sum=0;
        for(j=0; j<STATE; j++)
        {
            sum+=dicos_m.pos[i][j];
        }
        for(j=0; j<STATE; j++)
        {
            dicos_m.pos[i][j]/=sum;
            ofile_arg<<dicos_m.pos[i][j]<<"  ";
        }
        ofile_arg<<'\n';
    }
    ofile_arg.close();

    DataStr state_data;
    LoadDataStr(state_data,"data\\memm_state.txt");
    for(i=0; i<state_data.size(); i++)
    {
        dicos_m.ps0[getState_M(state_data[i][0])]++;//统计参数ps0
        for(j=1; j<state_data[i].size(); j++)
        {
            dicos_m.pss[getState_M(state_data[i][j-1])][getState_M(state_data[i][j])]++;//统计参数pss
        }
    }
    cout<<"i="<<i<<endl;
    cout<<"pss:--------------"<<endl;
    for(i=0; i<STATE; i++)
    {
        sum=0;
        for(j=0; j<STATE; j++)
        {
            sum+=dicos_m.pss[i][j];
        }
        for(j=0; j<STATE; j++)
        {
            dicos_m.pss[i][j]/=sum;
            cout<<dicos_m.pss[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"PI:--------------"<<endl;
    sum=0;
    for(i=0; i<STATE; i++)
    {
        sum+=dicos_m.ps0[i];
    }
    for(i=0; i<STATE; i++)
    {
        dicos_m.ps0[i]/=sum;
        cout<<dicos_m.ps0[i]<<"  ";
    }
    cout<<endl;

}

int Viterbi_M(const DataStr &testdata)
{
    int t,i,j,k;
    int pos;
    double deta[VEC_LEN][STATE];
    int fai[VEC_LEN][STATE];
    double max_deta;
    double max_fai;
    int max_i;
    for(i=0; i<STATE; i++)
    {
        pos=getPos(testdata[0][0]);
        deta[0][i]=dicos_m.ps0[i]*dicos_m.pos[pos][i];
        fai[0][i]=0;
    }
    for(k=0; k<testdata.size(); k++)
    {
        for(t=1; t<testdata[k].size(); t++)
        {
            for(i=0; i<STATE; i++)
            {
                max_deta=double_min;
                max_fai=double_min;
                for(j=0; j<STATE; j++)
                {
                    pos=getPos(testdata[k][t]);
                    if(deta[t-1][j]*dicos_m.pss[j][i]*dicos_m.pos[pos][i]>max_deta)
                    {
                        max_deta=deta[t-1][j]*dicos_m.pss[j][i]*dicos_m.pos[pos][i];
                    }
                    if(deta[t-1][j]*dicos_m.pss[j][i]>max_fai)
                    {
                        max_fai=deta[t-1][j]*dicos_m.pss[j][i];
                        max_i=j;
                    }
                }
                deta[t][i]=max_deta;
                fai[t][i]=max_i;
            }
        }
        max_deta=double_min;
        for(i=0; i<STATE; i++)
        {
            if(deta[testdata[k].size()-1][i]>max_deta)
            {
                max_deta=deta[testdata[k].size()-1][i];
                max_i=i;
            }
        }
        cout<<max_i;
        for(t=testdata[k].size()-2; t>=0; t--)
        {
            max_deta=double_min;
            cout<<fai[t+1][max_i];
            for(i=0; i<STATE; i++)
            {
                if(deta[t][i]>max_deta)
                {
                    max_deta=deta[t][i];
                    max_i=i;
                }
            }
        }
        cout<<endl;
    }
}
void init_DICOS_M()
{
    int i,j;
    for(i=0; i<STATE; i++)
    {
        for(j=0; j<VEC_LEN; j++)
        {
            dicos_m.pos[j][i]=0;
        }
        for(j=0; j<STATE; j++)
        {
            dicos_m.pss[i][j]=0;
        }
    }
}
int MEMM_CWS()
{
    DataStr data;
    LoadDataStr(data,"data\\pku_training.txt");
    init_DICOS_M();
    createVocabList_M(data);//创建状态字典，用于存放参数值
    DataStr testdata;
    LoadDataStr(testdata,"data\\hmm_test.txt");
    Viterbi_M(testdata);
    return 0;
}

