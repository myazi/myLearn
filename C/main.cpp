#include "LineReg.h"
#include "LogReg.h"
#include "SoftMaxReg.h"
#include "ME.h"//
#include "DNN.h"
#include "SVM.h"
#include "Bayes.h"
#include "HMM.h"//
#include "MEMM.h"//
#include "CRF.h"//
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
    //LineReg("data\\lineReg.txt","regression",0.01);
    //cin>>i;
    //LogReg("data\\LogReg.txt","gradAscent",0.01);
    //cin>>i;
    //SoftMaxReg("data\\LogReg.txt","gradAscent",0.01);

    //cin>>i;
    //ME();
    //trainDNN();
    //cin>>i;
    //SVM();
    //cin>>i;
    Bayes();
    cin>>i;
    //HMM();
    //MEMM();
    //CRF();
    MDP();
    cin>>i;
    DTree("ID3");
    cin>>i;
    CART();
    cin>>i;
    Adaboost(20);
    cin>>i;
    RF(50,5,0.01);
    cin>>i;
    kmeans();
    cin>>i;
    GMM(3,5);

    //PCA();
    //SVD();

    KNN();
    //Hash();

    return 0;
}
