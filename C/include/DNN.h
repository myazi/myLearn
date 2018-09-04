#ifndef DNN_H
#define DNN_H
#include "MatrixOpe.h"
#define MAX_LAYER_N 1000

struct parameters;

struct grad;

struct sup_parameters;

int init_parameters(Matrix X,const char *initialization);

void line_forward(parameters *p,double keep_prob);

void sigmoid_forward(parameters *p);

void relu_forward(parameters *p);

void line_active_forward(parameters *p,string active, double keep_prob);

Matrix model_forward(Matrix X,double *keep_probs);

void sigmoid_backword(parameters *p,grad *g);

void relu_backword(parameters *p,grad *g);

void line_backword(parameters *p,grad *g, double lambd, double keep_prob);

void line_active_backword(parameters *p,grad *g,string active, double lambd, double keep_prob);

void model_backword(Matrix AL,Matrix Y,double lambd,double *keep_probs);

double cost_cumpter(Matrix AL,Matrix Y,double lambd);

int updata_parameters_with_gd(double learn_rateing, int t);

int updata_parameters_with_momentum(double learn_rateing, int t,double beta);

int updata_parameters_with_adam(double learn_rateing, int t, double beta1, double beta2, double epsilon);

int updata_parameters(double learn_rateing, int t, const char *optimizer, double beta1, double beta2, double epsilon);

int predict(Matrix X,Matrix Y);

int DNN(Matrix X,Matrix Y,const char *optimizer,double learn_rateing,const char *initialization, double lambd, double keep_prob, \
        int mini_batch_size,double beta1, double beta2, double epsilon, int iter, bool print_cost);

int trainDNN();
#endif
