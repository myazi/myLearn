/**

极大似然+Veiterbi算法

**/

#include "MatrixOpe.h"
#define VEC_LEN 5000
#define STATE 4
#define double_min -1
struct DICOS_H
{
    string dic[VEC_LEN];
    double a[STATE][STATE];//参数A
    double b[STATE][VEC_LEN];//参数B
    double pi[STATE];//参数pi
    int len;
};
DICOS_H dicos_h;

int getPos_H(string str)
{
    int i=0;
    for(i=0; i<dicos_h.len; i++)
    {
        if(!str.compare(dicos_h.dic[i]))
            return i;
    }
    cout<<str<<endl;
    cout<<"状态字典中不存在该状态"<<endl;
    return -1;
}
int getState(string str)
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
/**
    映射策略对分词的质量影响非常之大
**/
int wordToState(const RowDataStr &str,int word,int j,int vl, string &state)
{
    char ch;
    if(str[j].length()==2)
    {
        dicos_h.b[3][vl]++;
        state.append(1,'3');
        state.append(1,' ');
    }
    if(str[j].length()==4)
    {
        if(word==0)
        {
            dicos_h.b[0][vl]++;
            state.append(1,'0');
            state.append(1,' ');
        }
        else
        {
            dicos_h.b[2][vl]++;
            state.append(1,'2');
            state.append(1,' ');
        }
    }
    if(str[j].length()==6)
    {
        dicos_h.b[word/2][vl]++;
        ch=(char)(word/2+48);
        state.append(1,ch);
        state.append(1,' ');
    }
    if(str[j].length()==8)
    {
        if(word==0)
        {
            dicos_h.b[0][vl]++;
            state.append(1,'0');
            state.append(1,' ');
        }
        if(word==6)
        {
            dicos_h.b[2][vl]++;
            state.append(1,'2');
            state.append(1,' ');
        }
        else
        {
            dicos_h.b[1][vl]++;
            state.append(1,'1');
            state.append(1,' ');
        }
    }
}
void createVocabList(const DataStr &data)
{
    ofstream ofile;
    ofstream ofile_arg;
    ofstream ofile_state;
    ofile.open("data\\hmm_dic.txt");
    ofile_state.open("data\\hmm_state.txt");
    int i,j,k,vl;
    int dic_len=0;
    int length=0;
    int word;
    string state;
    char ch;
    double sum;
    for(i=0; i<data.size(); i++)
    {
        state="";
        for(j=0; j<data[i].size(); j++)
        {
            if(data[i][j].length()%2!=0||data[i][j].length()>9)//训练预料词长度检测,最大四字长
                continue;
            for(word=0; word<data[i][j].length(); word+=2)
            {
                for(vl=0; vl<dic_len; vl++)
                {
                    if(!data[i][j].substr(word,2).compare(dicos_h.dic[vl]))
                    {
                        wordToState(data[i],word,j,vl,state);
                        break;
                    }
                }
                if(vl==dic_len)
                {
                    dicos_h.dic[vl]=data[i][j].substr(word,2);//对字典进行扩展
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
    dicos_h.len=dic_len;
    ofile_arg.open("data\\hmm_arg.txt");
    for(j=0; j<dicos_h.len; j++)
    {
        ofile_arg<<dicos_h.dic[j]<<"  ";
        sum=0;
        for(i=0; i<STATE; i++)
        {
            sum+=dicos_h.b[i][j];
        }
        for(i=0; i<STATE; i++)
        {
            dicos_h.b[i][j]/=sum;
            ofile_arg<<dicos_h.b[i][j]<<"  ";
        }
        ofile_arg<<'\n';
    }
    ofile_arg.close();
    cout<<"row="<<data.size()<<endl;
    cout<<"vec_len="<<dic_len<<endl;

    DataStr state_data;
    LoadDataStr(state_data,"data\\hmm_state.txt");
    for(i=0; i<state_data.size(); i++)
    {
        dicos_h.pi[getState(state_data[i][0])]++;//统计参数pi
        for(j=1; j<state_data[i].size(); j++)
        {
            dicos_h.a[getState(state_data[i][j-1])][getState(state_data[i][j])]++;//统计参数a
        }
    }
    cout<<"i="<<i<<endl;
    cout<<"A:--------------"<<endl;
    for(i=0; i<STATE; i++)
    {
        sum=0;
        for(j=0; j<STATE; j++)
        {
            sum+=dicos_h.a[i][j];
        }
        for(j=0; j<STATE; j++)
        {
            dicos_h.a[i][j]/=sum;
            cout<<dicos_h.a[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"PI:--------------"<<endl;
    sum=0;
    for(i=0; i<STATE; i++)
    {
        sum+=dicos_h.pi[i];
    }
    for(i=0; i<STATE; i++)
    {
        dicos_h.pi[i]/=sum;
        cout<<dicos_h.pi[i]<<"  ";
    }
    cout<<endl;
}

int Viterbi(const DataStr &testdata)
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
        pos=getPos_H(testdata[0][0]);
        deta[0][i]=dicos_h.pi[i]*dicos_h.b[i][pos];
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
                    pos=getPos_H(testdata[k][t]);
                    if(deta[t-1][j]*dicos_h.a[j][i]*dicos_h.b[i][pos]>max_deta)
                    {
                        max_deta=deta[t-1][j]*dicos_h.a[j][i]*dicos_h.b[i][pos];
                    }
                    if(deta[t-1][j]*dicos_h.a[j][i]>max_fai)
                    {
                        max_fai=deta[t-1][j]*dicos_h.a[j][i];
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
void init_DICOS_H()
{
    int i,j;
    for(i=0; i<STATE; i++)
    {
        for(j=0; j<VEC_LEN; j++)
        {
            dicos_h.b[i][j]=0;
        }
        for(j=0; j<STATE; j++)
        {
            dicos_h.a[i][j]=0;
        }
    }
}
int HMM_CWS()
{
    DataStr data;
    LoadDataStr(data,"data\\pku_training.txt");
    init_DICOS_H();
    createVocabList(data);//创建状态字典，用于存放参数值
    DataStr testdata;
    LoadDataStr(testdata,"data\\hmm_test.txt");
    Viterbi(testdata);
    return 0;
}
