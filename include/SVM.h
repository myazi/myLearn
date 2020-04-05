#ifndef SVM_H
#define SVM_H
#include "MatrixOpe.h"
#define MAX 1000000
#define MIN -100000
//SMO参数结构体
namespace  MLL
{
//核函数的结构体
    struct kTup
    {
        int type = 0;//0,1
        double arg = 0;//核函数参数
    };
    class SVM
    {
        private:
            Matrix _x;
            Matrix _y;
            double _C;
            double _soft;
            int _iter;
            Matrix _alphas;
            double _b;
            Matrix _eCache;
            Matrix _kernel;
            bool _k;
            kTup _ktup;
        public:
            Matrix kernelTran(Matrix &xOneRow);
            double clipAlpha(const double &alpha, const double &L, const double &H);
            double calcEk(const int &k);
            void updataEk(const int &k);
            int selectJ(const int &i, const double &Ei);
            int innerL(const int &i);
            int smoP();
            //非常值得注意的是svm中训练样本按列排列，即每一列是一个样本，所以导致w是行向量
            SVM(const std::string &file, const double C, const double soft, const double b, const int iter,kTup ktup);
    typedef std::shared_ptr<SVM> SVMPtr;
};
}
#endif
