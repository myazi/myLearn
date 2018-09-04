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
#include "MDP.h"
#define size 12
int MDP()
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
    int action[size]= {4,0,1,-1,8,-1,10,-1,9,8,9,10};//直接表示可到节点的下标
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
    int action[size]= {4,0,1,-1,8,-1,10,-1,9,8,9,10};
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

/**
动态规划求解马尔科夫决策过程

**/
/**

1 简单递归实现

**/

/*
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define size 12
using namespace std;
typedef struct arccell
{
    int adj;//结点间的关系即权
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//各顶点名称
    adjmatrix arcs;//邻接矩阵
    int vexnum,arcnum;//图当前的顶点数和弧数
    double reward[max_vertex_num];//每个状态的回报
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,1},
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
double sumreward[size];
double maxreward[size]={0,0,0,0,0,0,0,0,0,0,0,0};
int count=0;
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//深度遍历图
void dfs(mgraph &g,int i);
stack<int> s;//该栈定义为全局变量的原因是如果定义在有递归函数的体内部，
//则每一次都会申请一次栈，每一次操作也不是对同一个栈进行操作
stack<int>copys;
int main()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//深度遍历和广度遍历而选其一，因为公用了全局变量
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
        cout<<"count="<<count<<endl;
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12}; //顶点名称
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//初始化顶点名称
    {
        g.vexs[i]=a[i];
    }
    cout<<endl;
    for(i=0; i<g.vexnum; i++)//初始化没有任何弧和弧的信息
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//初始化顶点间的权值和相关信息
        //默认初始化权值为-1，信息为NULL
        g.reward[i]=reward[i];
    }
    return ok;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;//在以每个状态为初始点做深度遍历之前把所有的状态重新置为未访问的标志
        while(!s.empty())
        {
            s.pop();
        }//由于在上一次使用栈时，防止未能把栈清空，重新清空一下
        dfs(g,i);
        cout<<"*****************************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,flag;
    int lujin[size];
    visited[i]=true;//访问过后，把访问标志置为true
    s.push(g.vexs[i]);
    copys=s;
    flag=0;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//由第一次传递来的i选第一个相邻的顶点做深度遍历
            //遍历一次结束后，再选择另一个相邻的顶点做深度遍历，直到主调函数传递来的顶点无相邻的
            //顶点再返回主调函数做下一次循环
        {
            flag=1;
            dfs(g,j);
            count++;//递归返回一次count++
            visited[s.top()-1]=false;//只是把访问标志重新置为false，并不输出
            s.pop();
        }
    }
    j=0;
    if(flag==0)//如果某次递归进入该函数时，未执行上面的for，那么就是表示该节点没有其他可到达的节点了，或者是到达了奖励节点
    //此时，可以进行路径输出，并且计算栈底起点节点经过该路径的累积回报，统计所有的可能路径，不断的更新最大的累积回报，以及该累积回报下的路径
    {
        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        sumreward[lujin[start]-1]=0;
        for(j; j>0; j--)
        {
            sumreward[lujin[start]-1]+=g.reward[lujin[j-1]-1];
            cout<<lujin[j-1]<<"  ";
        }
        //用sumreward，栈底元素为下的数组标作记录栈底元素的累积回报
        cout<<sumreward[lujin[start]-1]<<endl<<"-----------------------------------"<<endl;
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])//更新累积回报
            {
                maxreward[lujin[start]-1]=sumreward[lujin[start]-1];
            }
    }
}
*/

/***
2 状态为16个情况

*/

/*
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define overflow -2
#define int_max 9999
#define size 16
using namespace std;
typedef struct arccell
{
    int adj;//结点间的关系即权
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//各顶点名称
    adjmatrix arcs;//邻接矩阵
    int vexnum,arcnum;//图当前的顶点数和弧数
    double reward[max_vertex_num];//每个状态的回报
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0},
    {0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0},
    {0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0}

};
double reward[size]=
{
    -0.02,-0.02,-0.02,1,
    -0.02,0,-0.02,-1,
    -0.02,-0.02,-0.02,-0.02,
    -0.02,-0.02,-0.02,-0.02
};
double sumreward[size];
double maxreward[size]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//深度遍历图
void dfs(mgraph &g,int i);
stack<int> s;//该栈定义为全局变量的原因是如果定义在有递归函数的体内部，
//则每一次都会申请一次栈，每一次操作也不是对同一个栈进行操作
stack<int>copys;
int main()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//深度遍历和广度遍历而选其一，因为公用了全局变量
    //bfstraverse(g);
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    //int v0=0;//选择到各点的初始点
    //shortpath_dij(g,v0);
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; //顶点名称
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//初始化顶点名称
    {
        g.vexs[i]=a[i];
        cout<<g.vexs[i]<<"  ";
    }
    cout<<endl;
    for(i=0; i<g.vexnum; i++)//初始化没有任何弧和弧的信息
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//初始化顶点间的权值和相关信息
        //默认初始化权值为-1，信息为NULL
        g.reward[i]=reward[i];
    }
    return ok;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
        visited[i]=false;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;
        while(!s.empty())
        {
            s.pop();
        }
        dfs(g,i);
        cout<<"***************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,flag;
    int lujin[size];
    visited[i]=true;
    s.push(g.vexs[i]);
    copys=s;
    flag=0;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//由第一次传递来的i选第一个相邻的顶点做深度遍历
            //遍历一次结束后，再选择另一个相邻的顶点做深度遍历，直到主调函数传递来的顶点无相邻的
            //顶点再返回主调函数做下一次循环
        {
            flag=1;
            dfs(g,j);
            visited[s.top()-1]=false;
            s.pop();
        }
    }
    j=0;
    if(flag==0)
    {

        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        sumreward[lujin[start]-1]=0;
        for(j; j>0; j--)
        {
            sumreward[lujin[start]-1]+=g.reward[lujin[j-1]-1];
            cout<<lujin[j-1]<<"  ";
        }
        cout<<sumreward[lujin[start]-1]<<endl<<"-----------------------------------"<<endl;
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])
            {
                maxreward[lujin[start]-1]=sumreward[lujin[start]-1];
                    //cout<<sumreward[lujin[start]-1]<<"&"<<maxreward[lujin[start]-1]<<endl;
            }
    }
}

*/

/***

3 递归精简版

***/
/*

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define overflow -2
#define int_max 9999
#define size 12
using namespace std;
typedef struct arccell
{
    int adj;//结点间的关系即权
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//各顶点名称
    adjmatrix arcs;//邻接矩阵
    int vexnum,arcnum;//图当前的顶点数和弧数
    double reward[max_vertex_num];//每个状态的回报
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,1},
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
double sumreward[size];
double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
int count=0;
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//深度遍历图
void dfs(mgraph &g,int i);
stack<int> s;//该栈定义为全局变量的原因是如果定义在有递归函数的体内部，
//则每一次都会申请一次栈，每一次操作也不是对同一个栈进行操作
stack<int>copys;
int maiwSum=0;n()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//深度遍历和广度遍历而选其一，因为公用了全局变量
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    cout<<"count="<<count<<endl;
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12}; //顶点名称
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//初始化顶点名称
    {
        g.vexs[i]=a[i];
    }
    for(i=0; i<g.vexnum; i++)//初始化没有任何弧和弧的信息
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//初始化顶点间的权值和相关信息
        //默认初始化权值为-1，信息为NULL
        g.reward[i]=reward[i];
    }
    return ok;
}
int printf(mgraph &g)
{
    int i=0,j=0;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            cout<<g.arcs[i][j].adj<<"  ";
        cout<<endl;
    }
    return 0;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
        visited[i]=false;
    for(i=0; i<g.vexnum; i++)
    {
        if(maxreward[i]>0+0.001)
            continue;
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;
        while(!s.empty())
        {
            s.pop();
        }
        dfs(g,i);
        cout<<endl<<"***************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,k,flag=0;
    int lujin[size];
    visited[i]=true;
    s.push(g.vexs[i]);
    copys=s;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//由第一次传递来的i选第一个相邻的顶点做深度遍历
            //遍历一次结束后，再选择另一个相邻的顶点做深度遍历，直到主调函数传递来的顶点无相邻的
            //顶点再返回主调函数做下一次循环
        {
            flag=1;
            dfs(g,j);
            count++;
            visited[s.top()-1]=false;
            s.pop();
        }
    }
    j=0;
    if(flag==0)//同上，直到递归到某一个节点走不下去了，那么该路径就到了终点，可以输出该路径的累积回报
    {
        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        int l=0;
        sumreward[lujin[start]-1]=0;
        for(i=0; i<size; i++)
            sumreward[i]=0;
        for(k=0; k<=start; k++)
        {
            for(j=start-k; j>=0; j--)
            {
                sumreward[lujin[start-k]-1]+=g.reward[lujin[j]-1];//记录累积回报
                //cout<<lujin[j]<<"  ";
            }
            //cout<<sumreward[lujin[k]-1]<<endl<<"-----------------------------------"<<endl;
        }
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])//更新累积回报，并且把这条路径上，以每一个状态为起始状态的累积回报保存下来，若对栈底元素为最优的话
        //那么该条路径上所有的节点都以这条路径为决策达到最优累积回报
        {
            //记录这条路径上所有状态为初始状态走该路径的累积回报
            for(l=0; l<=start; l++)
            {
                maxreward[lujin[start-l]-1]=sumreward[lujin[start-l]-1];
                //cout<<maxreward[lujin[start-l]-1]<<"   ";
            }
        }
        for(k=0; k<=start; k++)
        {
            for(j=start-k; j>=0; j--)
            {
                //sumreward[lujin[start-k]-1]+=g.reward[lujin[j]-1];
                cout<<lujin[j]<<"  ";
            }
            cout<<maxreward[lujin[start-k]-1]<<endl;
        }
    }
}

*/

/**

4 非递归动态规划

从最终状态出发，采用广度遍历不断的更新其上一状态的累积回报
*/

/*

#include <iostream>
using namespace std;
#include <stdlib.h>
#define size 12
typedef struct subset
{
    int data;
    struct subset* next;
} subset;
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
    int i=0,j=0,count=0;
    struct subset *maxsubset;
    maxsubset=(subset *)malloc(sizeof(subset)*1);
    maxsubset->data=-1;
    maxsubset->next=NULL;
    struct subset *p,*q;
    for(i=0; i<size; i++)//初始化0步立即回报为累积回报
    {
        maxreward[i]=reward[i];
        if(reward[i]==1)
        {
            p=(subset *)malloc(sizeof(subset)*1);
            p->data=i;
            p->next=NULL;
            q=maxsubset;
            while((q->next)!=NULL)
                q=q->next;
            q->next=p;
        }
    }
    q=maxsubset->next;
    while(q!=NULL)//这里图的广度遍历没有用到队列，但也用到了队列的思想
    //对于当前步能到达的节点用链表连接起来，然后逐渐进行下一步的能到达的节点进行入链（入队列），同样是一种先进先出思想
    {
        for(i=0; i<size; i++) //由于不是策略迭代，只能遍历所有的状态，找出能到的，且更优的
        {
            if(matrix[i][q->data]==1&&maxreward[i]<0)//double类型比较大小的偏差，加上一个小数作为精度
            {
                maxreward[i]=reward[i]+maxreward[q->data];
                p=(subset *)malloc(sizeof(subset)*1);
                p->data=i;
                p->next=NULL;
                q=maxsubset;
                while((q->next)!=NULL)
                    q=q->next;
                q->next=p;
            }
            count++;
        }
        maxsubset->next=maxsubset->next->next;//删除当前节点，即当前步下能到达的节点都已经走完了，可出队列了
        q=maxsubset->next;//
    }
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    cout<<endl<<"count="<<count<<endl;
    return 0;
}

*/


