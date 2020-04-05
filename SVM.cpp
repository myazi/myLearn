#include "SVM.h"
namespace MLL
{
    /**
    根据参数，来生成不同的核函数
    */
    Matrix SVM::kernelTran(Matrix &xOnerow)
    {
        Matrix K(_x._row,1,0);
        Matrix xOnerowT = xOnerow.transposeMatrix();
        if(_ktup.type==1)
        {
            K = K.multsMatrix(_x,xOnerowT);
        }
        if(_ktup.type==2)
        {
            //高斯核
        }
        return K.transposeMatrix();
    }
    /**
    上下限剪辑函数
    */
    double SVM::clipAlpha(const double &alpha, const double &L, const double &H)
    {
        if(alpha>H)
            return H;
        if(alpha<L)
            return L;
        return alpha;
    }
    /**
    误差值的计算也是根据数学表达式来实现的

    **/
    double SVM::calcEk(const int &k)
    {
        Matrix ay(_x._row,1,0);
        for(size_t i = 0; i < ay._row; i++)
        {
            ay._data[i][0] = _alphas._data[i][0] * _y._data[i][0];
        }
        Matrix ayT = ay.transposeMatrix();
        Matrix f(1,1,0);
        if(!_k)
        {
            Matrix ayTx = ayTx.multsMatrix(ayT,_x);
            Matrix xOnerow = _x.getOneRow(k);
            Matrix xOnerowT = xOnerow.transposeMatrix();
            f = f.multsMatrix(ayTx,xOnerowT);//有待商榷
        }
        else
        {
            Matrix kOnecol = _kernel.getOneCol(k);
            f = f.multsMatrix(ayT,kOnecol);//有待商榷
        }
        double fXK = f._data[0][0]+ _b - _y._data[k][0];
        return fXK;
    }
    ///更新误差矩阵
    void SVM::updataEk(const int &k)
    {
        double Ek;
        Ek=calcEk(k);//计算误差值
        _eCache._data[k][0] = 1;
        _eCache._data[k][1] = Ek;
    }
    /**
    对于第二个alpha的选择采用Ei-Ej的大小进行衡量，同时这里引用一个矩阵来保存不变的E，减少计算量
    选择出合适第二个alpha2，对于第一次进来，保存误差值E的矩阵必然没有一个有效的，所以采用随机策略
    随机选择策略其实可以任意选择，具体没有实现，而是采用等价方式实现，但需要注意数据越界问题
    **/
    int SVM::selectJ(const int &i, const double &Ei)
    {
        int maxK = -1;
        double Ek = 0;
        double Ej = 0;
        double deltaE = 0;
        double maxDeltaE = 0;
        for(size_t k = 0; k < _alphas._col; k++)
        {
            if(_eCache._data[k][0] == 1 && k != i)
            {
                Ek = calcEk(k);
                deltaE = fabs(Ei-Ek);
                if(deltaE > maxDeltaE)
                {
                    maxK = k;
                    maxDeltaE = deltaE;
                    Ej = Ek;
                }
            }
            //随机选择一个j, 理论上只有第一次时所有的ek都没计算过，所以任选一个即可
            if(maxK == -1)
            {
                maxK = (i * 2 + 1) % 100;
                Ej = calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    内层循环实现两个alpha的优化，由外层循环函数提供alpha1的可选集合（所有样本或者是支持向量），
    遍历集合上的每一个alpha，对其进行判断是否违反KKT条件，如果违反则选为第一个alpha1，
    同样调用selctJ函数来启发式选择使得alpha1改变最大的alpha2，
    选择出合适ij之后，还得根据当前alpha1和alpha2来确定优化的上下限。
    然后通过迭代公式，来计算alpha1和alpha2，计算的时候判断是否采用核方法。
    并对计算结果进行上下限剪辑，最终确定优化后的alpha1和alpha2，同时对当前优化后结果对b也进行修改
    考虑到每一次选择第二个alpha2时，都需要比较一个误差值，而这个误差值在每次优化过程中都只有对应修改alpha的误差值有变化
    而其他的是不变的，所以用一个矩阵来保存有效的误差值。
    */
    int SVM::innerL(const int &i)
    {
        double Ei = 0;
        double Ej = 0;
        int j = 0;
        double alphaIOld = 0;
        double alphaJOld = 0;
        double L = 0;
        double H = 0;
        double eta = 0;
        int n = 0;
        double temp[3];//ii，jj，ij
        double b1 = 0, b2 = 0;
        if(_y._data[i][0] * (Ei - _y._data[i][0] * _soft) < 0 && _alphas._data[i][0] < _C || _y._data[i][0] * (Ei- _y._data[i][0] * _soft) > 0 && _alphas._data[i][0] > 0)
        {
            Ei = calcEk(i);
            j = selectJ(i,Ei);
            Ej = calcEk(j);
            alphaIOld = _alphas._data[i][0];
            alphaJOld = _alphas._data[j][0];
            if(_y._data[i][0] != _y._data[j][0])
            {
                L = std::max<double>(0.0, _alphas._data[j][0] - _alphas._data[i][0]);
                H = std::min<double>(_C, _alphas._data[j][0] - _alphas._data[i][0] + _C);
            }
            else
            {
                L = std::max<double>(0.0, _alphas._data[j][0] + _alphas._data[i][0] - _C);
                H = std::min<double>(_C, _alphas._data[j][0] + _alphas._data[i][0]);
            }
            if(L == H)
            {
                std::cout<<"l=h------------------"<<std::endl;
                return 0;
            }
            if(!_k)
            {
                temp[0] = 0;
                temp[1] = 0;
                temp[2] = 0;
                for(n = 0; n < _x._row; n++)
                {
                    temp[0] += pow(_x._data[i][n],2);
                    temp[1] += pow(_x._data[j][n],2);
                    temp[2] += _x._data[i][n] * _x._data[j][n];
                }
            }
            else
            {
                temp[0] = _kernel._data[i][i];
                temp[1] = _kernel._data[j][j];
                temp[2] = _kernel._data[i][j];
                eta = temp[0] + temp[1] - 2 * temp[2];
            }
            eta = temp[0] + temp[1] - 2 * temp[2];
            if(eta < 0)
            {
                std::cout<<"eta<0-----------------"<<std::endl;
                return 0;
            }
            _alphas._data[j][0] += _y._data[j][0]*(Ei-Ej)/eta;
            _alphas._data[j][0] = clipAlpha(_alphas._data[j][0],L,H);//对alpha进行剪辑
            updataEk(j);//更新误差值
            if(fabs(_alphas._data[j][0]-alphaJOld) < 0.00000000001)
            {
                std::cout<<"j not moving enough---------------"<<std::endl;
                return 0;
            }
            _alphas._data[i][0] += _y._data[i][0] * _y._data[j][0] * (alphaJOld - _alphas._data[j][0]);
            updataEk(i);//更新误差值

            b1 = _b - Ei - _y._data[i][0] * (_alphas._data[i][0] - alphaIOld) * temp[0] - _alphas._data[j][0] * (_alphas._data[j][0]-alphaJOld) * temp[2];
            b2 = _b - Ej - _y._data[i][0] *(_alphas._data[i][0] - alphaIOld) * temp[2] - _alphas._data[j][0] * (_alphas._data[j][0] - alphaJOld) * temp[1];
            if(0 < _alphas._data[i][0] && _C > _alphas._data[i][0])
                _b = b1;
            else if(0 < _alphas._data[j][0] && _C > _alphas._data[j][0])
                _b = b2;
            else
                _b = (b1 + b2) / 2;
            return 1;
        }
        std::cout<<"kkt--------------------------"<<std::endl;
        return 0;
    }

    /**
    SMO算法的入口函数，其主要功能是初始化SMO所有的参数到结构体OS
    确定迭代结束标志，并在所有样本和支持向量上循环的选择合适alpha1，调用inner确定alpha1，
    外层循环每一次是迭代优化一对alpha,内层循环才是正真实现迭代优化一对alpha
    最后，对smo算法的实现进行检查，通过alpha求解出w，b，并在训练样本上比较其预测值与实际值的差异。
    该办法只能检查SMO算法实现的正确性，不能检查SVM的性能。
    */

    int SVM::smoP()
    {
        int it = 0;
        int i = 0;
        bool entireSet=true;//标志用于判断当前迭代是针对所有alpha，还是针对0-C之间的部分alpha，这里其实第一个alpha的启发式选择，即选择落在支持向量上的点
        int alphaPairsChanged = 0;//该参数判断在确定第一个参数之后，是否能选择出符合要求的第二alpha，返回值为1表示能，0为不能
        for(it = 0; it < _iter && (alphaPairsChanged > 0 || entireSet); it++)
        {
            //循环结束标志为迭代次数已到预设值，或者是不能再继续优化（对于所有的支持向量上的点都找不到第二个alpha对第一个alpha进行优化后，重新再遍历所有的点寻找可优化的参数对）
            //还是找不到则再次遍历支持向量上的点，这次必然也是找不到，才结束迭代
            alphaPairsChanged = 0;
            if(entireSet)
            {
                for(i = 0; i < _x._row; i++)
                {
                    alphaPairsChanged += innerL(i);
                    std::cout<<"it="<< it <<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<"  entireSet="<<entireSet<<std::endl;
                }
                it++;
            }
            else
            {
                for(i = 0; i <  _x._row; i++)
                {
                    if(_alphas._data[i][0]>0 && _alphas._data[i][0] < _C)//只选择支持向量上的点
                        continue;
                    alphaPairsChanged+=innerL(i);
                    std::cout<<"it="<<it<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<alphaPairsChanged<<"  entireSet="<<entireSet<<std::endl;
                }
                it++;
            }
            if(entireSet)
                entireSet = false;
            else if(alphaPairsChanged == 0)
                entireSet=true;
        }
        ///对SMO算法实现的正确性进行验证，输出预测值与实际值的差别，全为0表示在训练样本上预测全对
        Matrix ay(_x._row,1,0);
        for(i = 0; i < _x._row; i++)
        {
            ay._data[i][0] = _alphas._data[i][0] * _y._data[i][0];
        }

        Matrix xT = _x.transposeMatrix();

        Matrix w = w.multsMatrix(xT,ay);

        Matrix pre_label = pre_label.multsMatrix(_x,w);
        std::cout<<_b<<"  ";
        for(i = 0; i < _x._col; i++)
        {
            std::cout<<w._data[i][0]<<"  ";
        }
        std::cout<<std::endl;
        std::cout<<"----测试结果-------"<<std::endl;
        ///验证训练样本数据，验证SVM实现的正确性
        for(i = 0; i < _x._row; i++)
        {
            if((pre_label._data[i][0] + _b) > 0)
                std::cout<<"pre_label: 1" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
            else
                std::cout<<"pre_label: -1" << "\t" << "fact_label: " << _y._data[i][0] <<std::endl;
        }
        return 0;
    }
    
	/**
    结构体OS的初始化，用于保存所以SMO算法中需要用到的参数
    */
    SVM::SVM(const std::string &file, const double C, const double soft, const double b, const int iter,kTup ktup)
    {
        _x.init_by_data(file);
        _y=_x.getOneCol(_x._col-1);
        _x.deleteOneCol(_x._col-1);
        _eCache.initMatrix(_x._row,2,0);
        _alphas.initMatrix(_x._row,1,0);
        _kernel.initMatrix(_x._row,_x._row,0);
        _b = b;
        _C = C;
        _iter = iter;
        _soft = soft;
        if(ktup.type != 0)
        {
            int i = 0, j = 0;
            Matrix xOnerow(1,_x._col,0);
            Matrix kOnecol(_x._row,1,0);
            for(i = 0; i < _x._row; i++)
            {
                xOnerow = _x.getOneRow(i);
                kOnecol = SVM::kernelTran(xOnerow);
                for(j = 0; j < _x._row; j++)
                    _kernel._data[i][j] = kOnecol._data[0][j];
            }
            _k = true;
        }
        _k = false;
    }
}
