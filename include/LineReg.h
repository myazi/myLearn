#ifndef LineReg_H
#define LineReg_H
#include "MatrixOpe.h"
namespace MLL{
    class LineReg{
        private:
            Matrix _x;
            Matrix _y;
            Matrix _w;
            double _lamd = 0.1;
            double _k = 1;
            enum moedl {gradAscent, stoGradAscent};
        public:
            LineReg(const std::string &file, const std::string &model,const double &lamd, const double &k);
            int regression();
            int ridgeRegres();
            int lwlrtest();
            Matrix locWeightLineReg(Matrix &test);
    typedef std::shared_ptr<LineReg> LineRegPtr;
    };
}
#endif
