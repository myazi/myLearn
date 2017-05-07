/***
马尔科夫决策过程值迭代，关键在于第一次迭代要例外，
因为目标状态是一个终止状态，放到迭代循环里面会出现
临近的状态回报函数无限的，发散。
迭代过程采用的是异步迭代，即每一次内层循环找到更优的
回报就立即更新最大回报，以便与之相邻的状态能立即更新到最优
*/

/****
值迭代
同步更新
12*12*7

*/


#include <iostream>
using namespace std;
#define size 12
int main()
{
    int matrix[size][size]=
    {
        {0,1,0,0,1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0},
        {0,0,0,0,0,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,1,0,0,1,0}
    };
    double reward[size]=
    {
        -0.02,-0.02,-0.02,1,
        -0.02,0,-0.02,-1,
        -0.02,-0.02,-0.02,-0.02
    };
    double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
    int action[size]= {4,0,1,-1,8,-1,10,-1,9,8,9,10}; //上右下左{1,2,3,4}
    int i=0,j=0,count=0;
    bool flag=0;
    for(i=0;i<size;i++)
        maxreward[i]=reward[i];
    while(!flag)
    {
        flag=1;
        for(i=0; i<size; i++)
        {
            if(action[i]>0||action[i]==0)
                maxreward[i]=reward[i]+maxreward[action[i]];
            else
                maxreward[i]=reward[i];
        }//放到这意味着同步更新，count=1008是12*12的7倍，即扫了7遍
        for(i=0; i<size; i++)//对每一个状态求最大的V(s)
        {
            for(j=0; j<size; j++)//策略迭代的话这里其实可以换做扫一遍策略集，这也就是和值迭代不同的地方
            {
                //cout<<"i="<<i<<"  "<<maxreward[i]<<"  "<<endl;
                if(matrix[i][j]==1&&maxreward[j]>maxreward[i]-reward[i]+0.0001)//更新累积回报
                {
                    action[i]=j;
                    //if(action[i]>0||action[i]==0)
                        //maxreward[i]=reward[i]+maxreward[action[i]];//放到这是异步更新，
                    //else
                      //  maxreward[i]=reward[i];
                    flag=0;//当累积回报不再更新，即不进入该if，那么就结束迭代
                }
                count++;
            }
        }
    }
    for(i=0; i<size; i++)
    {
        cout<<action[i]+1<<"    ";
    }
    cout<<endl;
    for(i=0; i<size; i++)
    {
        cout<<maxreward[i]<<"  ";
    }
    cout<<endl<<"count="<<count<<endl;
    return 0;
}


/*
值迭代 异步更新 12*12*4
*/
/*
#include <iostream>
using namespace std;
#define size 12
int main()
{
    int matrix[size][size]=
    {
        {0,1,0,0,1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0},
        {0,0,0,0,0,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,1,0,0,1,0}
    };
    double reward[size]=
    {
        -0.02,-0.02,-0.02,1,
        -0.02,0,-0.02,-1,
        -0.02,-0.02,-0.02,-0.02
    };
    double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
    //double sumreward[size]={0,0,0,0,0,0,0,0,0,0,0,0};
    int i=0,j=0,count=0;
    bool flag=0;
    for(i=0; i<size; i++)
        maxreward[i]=reward[i];
    while(!flag)
    {
        flag=1;
        for(i=0; i<size; i++)//对每一个状态求最大的V(s)
        {
            for(j=0; j<size; j++) //由于不是策略迭代，只能遍历所有的状态，找出能到的，且更优的
            {
                if(matrix[i][j]==1&&maxreward[j]>maxreward[i]-reward[i]+0.0001)//double类型比较大小的偏差，加上一个小数作为精度
                {
                    maxreward[i]=reward[i]+maxreward[j];//异步更新
                    flag=0;
                }
                count++;
            }
        }
    }
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    cout<<endl<<"count="<<count<<endl;
    return 0;
}
*/

/***

策略迭代+异步更新

12*4*4
*/
/*
#include <iostream>
using namespace std;
#define size 12
#define ACTION 4
int main()
{
    int matrix[size][size]=
    {
        {0,1,0,0,1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0},
        {0,0,0,0,0,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,1,0,0,1,0}
    };
    double reward[size]=
    {
        -0.02,-0.02,-0.02,1,
        -0.02,0,-0.02,-1,
        -0.02,-0.02,-0.02,-0.02
    };
    double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
    int action[size]= {4,0,1,-1,8,-1,10,-1,9,8,9,10}; //上右下左{1,2,3,4}
    int ac[ACTION]={-4,1,4,-1};
    int i=0,j=0,count=0;
    bool flag=0;
        for(i=0;i<size;i++)
        maxreward[i]=reward[i];
    while(!flag)
    {
        flag=1;
        for(i=0; i<size; i++)//对每一个状态求最大的V(s)
        {
            for(j=0; j<ACTION; j++)//策略迭代的话这里其实可以换做扫一遍策略集，这也就是和值迭代不同的地方
            {
                //cout<<"i="<<i<<"  "<<maxreward[i]<<"  "<<endl;
                if(matrix[i][ac[j]+i]==1&&maxreward[ac[j]+i]>maxreward[i]-reward[i]+0.0001)
                {
                    action[i]=j;
                    //if(reward[i]!=1&&reward[i]!=-1)
                        maxreward[i]=reward[i]+maxreward[ac[j]+i];
                    //else
                    //    maxreward[i]=reward[i];
                    flag=0;
                }
                count++;
            }
        }
    }
    for(i=0; i<size; i++)
    {
        cout<<action[i]+1<<"    ";
    }
    cout<<endl;
    for(i=0; i<size; i++)
    {
        cout<<maxreward[i]<<"  ";
    }
    cout<<endl<<"count="<<count<<endl;
    return 0;
}
*/
