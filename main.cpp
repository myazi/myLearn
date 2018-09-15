#include "LineReg.h"
#include "LogReg.h"
#include "SoftMaxReg.h"
#include "ME.h"//
#include "DNN.h"
#include "SVM.h"
#include "Bayes.h"
#include "HMM.h"
#include "MEMM.h"
#include "CRF.h"
#include "MDP.h"
#include "DTree.h"
#include "CART.h"
#include "Adaboost.h"
#include "RF.h"
#include "kMeans.h"
#include "GMM.h"
#include "PCA.h"//
#include "SVD.h"//
#include "KNN.h"
#include "Hash.h"//
int main()
{
    int i;
    kmeans();

    LineReg();
    LogReg();
    SoftMaxReg();

    //ME();
    trainDNN();
    SVM();

    Bayes();
    HMM();
    HMM_CWS();
    MEMM_CWS();
    CRF_CWS();
    MDP();

    DTree();
    CART();
    Adaboost();
    RF();

    kmeans();
    GMM();
    //PCA();
    //SVD();

    KNN();
    //Hash();

    return 0;
}
