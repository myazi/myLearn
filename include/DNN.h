#ifndef DNN_H
#define DNN_H
#include "MatrixOpe.h"
#define MAX_LAYER_N 1000
namespace MLL{
    
    class DNN{
    /**
    网络参数W,b其中Z,A是为了求解残差方便
    **/
    struct parameters
    {
        Matrix W;
        Matrix b;
        Matrix Z;
        Matrix A;
        Matrix D;
        parameters *next = NULL;
        parameters *pre = NULL;
    };

    /***
    复合函数中对应目标函数对相应变量的偏导
    **/
    struct grad
    {
        Matrix grad_W;
        Matrix grad_b;
        Matrix grad_Z;
        Matrix grad_A;
        Matrix V_dw;
        Matrix V_db;
        Matrix S_dw;
        Matrix S_db;
        Matrix V_dw_corrected;
        Matrix V_db_corrected;
        Matrix S_dw_corrected;
        Matrix S_db_corrected;
        grad *next = NULL;
        grad *pre = NULL;
    };

    /**
    神经网络超参数
    **/
    struct sup_parameters
    {
        int layer_dims;//神经网络层数
        int layer_n[MAX_LAYER_N];//每层神经元个数
		std::string layer_active[MAX_LAYER_N];//每层激活函数
    };

        private:
            Matrix _x;//input

            Matrix _y;//label

            parameters _par;// 定义网络参数全局变量

            grad _gra;//定义梯度全局变量

            sup_parameters _sup_par;//定义网络架构参数全局变量

            double _learn_rateing = 0.1;

            int _iter = 1000;

            double _lambd = 0.1;

            double _keep_prob = 1;

            double _beta1 =0.9;

            double _beta2 = 0.99;

            double _epsilon = 0.00001;

            bool _print_cost = true;

            std::string _optimizer = "gd";

            std::string _initialization = "he";

            int init_parameters();

            void line_forward(parameters *p,double keep_prob);

            void sigmoid_forward(parameters *p);

            void relu_forward(parameters *p);

            void line_active_forward(parameters *p,std::string active, double keep_prob);

            Matrix model_forward(double *keep_probs);

            void sigmoid_backword(parameters *p,grad *g);

            void relu_backword(parameters *p,grad *g);

            void line_backword(parameters *p,grad *g, double keep_prob);

            void line_active_backword(parameters *p,grad *g,std::string active, double keep_prob);

            void model_backword(Matrix AL,double *keep_probs);

            double cost_cumpter(Matrix AL);

            int updata_parameters_with_gd(int t);

            int updata_parameters_with_momentum(int t);

            int updata_parameters_with_adam(int t);

            int updata_parameters(int t);

            int predict();
        public:
            DNN(const std::string &file, const char *optimizer,double learn_rateing,const char *initialization, double lambd, double keep_prob, \
                int mini_batch_size,double beta1, double beta2, double epsilon, int iter, bool _print_cost);

    typedef std::shared_ptr<DNN> DNNPtr;
    };
}
#endif
