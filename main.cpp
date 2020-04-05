#include "LineReg.h"
#include "LogReg.h"
#include "SoftMaxReg.h"
#include "DNN.h"
#include "SVM.h"
#include "DTree.h"
#include "Adaboost.h"
#include "CART.h"
#include "RF.h"
#include "kMeans.h"
#include "KNN.h"
#include "Bayes.h"
#include "GMM.h"
#include "HMM.h"
#include "HMM_CWS.h"
#include "MEMM_CWS.h"
#include "CRF_CWS.h"

std::string path_file = "data/";
std::map<std::string, std::map<std::string, std::string> > conf_map;

int LogReg(const char *file = "logReg.txt")
{
    path_file.append(file);
	std::string model = "gradAscent";
    double alpha = 0.001;
    double iter = 100;
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("LogReg");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> LogReg_map = it->second;
		std::map<std::string, std::string>::iterator model_it = LogReg_map.find("model");
		if(model_it != LogReg_map.end())
		{
			model = model_it->second;
		}
		std::map<std::string, std::string>::iterator alpha_it = LogReg_map.find("alpha");
		if(alpha_it != LogReg_map.end())
		{
			alpha = atof((alpha_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator iter_it = LogReg_map.find("iter");
		if(iter_it != LogReg_map.end())
		{
			iter = atof((iter_it->second).c_str());
		}
	}
	std::cout << model << "\t" << alpha << "\t" << iter << std::endl;
	MLL::LogReg::LogRegPtr logreg = std::make_shared<MLL::LogReg>(path_file,model,alpha,iter);
    if(model=="gradAscent")
        logreg->gradAscent_Log();
    if(model=="stoGradAscent")
       logreg->stoGradAscent_Log();
    return 0;
}

int SoftMaxReg(const char *file = "logReg.txt")
{
    path_file.append(file);
	std::string model = "gradAscent";
    double alpha = 0.001;
    double iter = 100;

	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("SoftMaxReg");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> SoftMaxReg_map = it->second;
		std::map<std::string, std::string>::iterator model_it = SoftMaxReg_map.find("model");
		if(model_it != SoftMaxReg_map.end())
		{
			model = model_it->second;
		}
		std::map<std::string, std::string>::iterator alpha_it = SoftMaxReg_map.find("alpha");
		if(alpha_it != SoftMaxReg_map.end())
		{
			alpha = atof((alpha_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator iter_it = SoftMaxReg_map.find("iter");
		if(iter_it != SoftMaxReg_map.end())
		{
			iter = atof((iter_it->second).c_str());
		}
	}
	std::cout << model << "\t" << alpha << "\t" << iter << std::endl;
    MLL::SoftMaxReg::SoftMaxRegPtr softmaxreg = std::make_shared<MLL::SoftMaxReg>(path_file,model,alpha,iter);
    if(model=="gradAscent")
        softmaxreg->gradAscent_SoftMax();
    if(model=="stoGradAscent")
        softmaxreg->stoGradAscent_SoftMax();

    return 0;
}

int LineReg(const char *file = "lineReg.txt")
{
    path_file.append(file);
	std::string model = "regression";
    double lamd = 0.001;
    double k = 100;
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("LineReg");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> LineReg_map = it->second;
		std::map<std::string, std::string>::iterator model_it = LineReg_map.find("model");
		if(model_it != LineReg_map.end())
		{
			model = model_it->second;
		}
		std::map<std::string, std::string>::iterator lamd_it = LineReg_map.find("lamd");
		if(lamd_it != LineReg_map.end())
		{
			lamd = atof((lamd_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator k_it = LineReg_map.find("k");
		if(k_it != LineReg_map.end())
		{
			k = atof((k_it->second).c_str());
		}
	}
	std::cout << model << "\t" << lamd << "\t" << k << std::endl;
    
	MLL::LineReg::LineRegPtr linereg = std::make_shared<MLL::LineReg>(path_file,model,lamd,k);
    if(model=="regression")
    {
        linereg->regression();
    }
    if(model=="ridgeRegres")
    {
        linereg->ridgeRegres();
    }
    if(model=="lwlrtest")
    {
        linereg->lwlrtest();
    }
    return 0;
}

int trainDNN(const char *file = "logReg.txt")
{
    path_file.append(file);
    const char *initialization="he";
    double learn_rateing=0.1;
    int iter=1000;
    double lambd=0.1;
    double keep_prob=0.5;
    bool print_cost=true;
    const char *optimizer="gd";
    int mini_batch_size=64;
    double beta1=0.9;
    double beta2=0.999;
    double epsilon=0.00000001;
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("DNN");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> DNN_map = it->second;
		std::map<std::string, std::string>::iterator learn_rateing_it = DNN_map.find("learn_rateing");
		if(learn_rateing_it != DNN_map.end())
		{
			learn_rateing = atof((learn_rateing_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator lambd_it = DNN_map.find("lambd");
		if(lambd_it != DNN_map.end())
		{
			lambd = atof((lambd_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator keep_prob_it = DNN_map.find("keep_prob");
		if(keep_prob_it != DNN_map.end())
		{
			keep_prob = atof((keep_prob_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator iter_it = DNN_map.find("iter");
		if(iter_it != DNN_map.end())
		{
			iter = atof((iter_it->second).c_str());
		}
	}
    MLL::DNN::DNNPtr dnn = std::make_shared<MLL::DNN>(path_file,"gd",0.1,"he",0.01,1,64,0.9, 0.999, 0.0000001, 500, true);
    //dnn->predict(dnn->_x,dnn->_y);
    return 0;
}

int SVM(const char *file = "logReg.txt")
{
    path_file.append(file);
    double C = 0.6;
    double soft = 0.001;
    double b = 0;
    int iter = 100;
    MLL::kTup ktup;//核函数的定义，其中type元素为0表示不适用核函数，非0分别对应不同的核函数
    ktup.type = 1;
    ktup.arg = 1.0;

	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("SVM");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> SVM_map = it->second;
		std::map<std::string, std::string>::iterator C_it = SVM_map.find("C");
		if(C_it != SVM_map.end())
		{
			C = atof((C_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator soft_it = SVM_map.find("soft");
		if(soft_it != SVM_map.end())
		{
			soft = atof((soft_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator b_it = SVM_map.find("b");
		if(b_it != SVM_map.end())
		{
			b = atof((b_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator iter_it = SVM_map.find("iter");
		if(iter_it != SVM_map.end())
		{
			iter = atof((iter_it->second).c_str());
		}
	}
	std::cout << C << "\t" << soft << "\t" << b << iter << std::endl;
    MLL::SVM::SVMPtr svm = std::make_shared<MLL::SVM>(path_file,C,soft,b,iter,ktup);
    svm->smoP();
    return 0;
}

int Adaboost(const char *file = "adaboost.txt")
{
    path_file.append(file);
	int numIt = 10;
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("Adaboost");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> Adaboost_map = it->second;
		std::map<std::string, std::string>::iterator numIt_it = Adaboost_map.find("numIt");
		if(numIt_it != Adaboost_map.end())
		{
			numIt = atof((numIt_it->second).c_str());
		}
	}
	std::cout << numIt << std::endl;
    
	MLL::Adaboost::AdaboostPtr adaboost = std::make_shared<MLL::Adaboost>(path_file,numIt);
	adaboost->AdaboostTrain();
	return 0;
}

int DTree(const char *file = "id3.txt")
{
    path_file.append(file);
	std::string model = "ID3";
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("DTree");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> DTree_map = it->second;
		std::map<std::string, std::string>::iterator model_it = DTree_map.find("model");
		if(model_it != DTree_map.end())
		{
			model = model_it->second;
		}
	}
	std::cout << model << std::endl;

	MLL::DTree::DTreePtr dtree = std::make_shared<MLL::DTree>(path_file,model);
	return 0;
}

int CART(const char *file = "cart.txt")
{
    path_file.append(file);
	MLL::CART::CARTPtr cart = std::make_shared<MLL::CART>(path_file);
	return 0;
}

int RF(const char *file = "rf.txt")
{
    path_file.append(file);
	int numIt = 50;
	int deep = 5;
	double epsilon = 0.01;
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("RF");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> RF_map = it->second;
		std::map<std::string, std::string>::iterator numIt_it = RF_map.find("numIt");
		if(numIt_it != RF_map.end())
		{
			numIt = atof((numIt_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator deep_it = RF_map.find("deep");
		if(deep_it != RF_map.end())
		{
			deep = atof((deep_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator epsilon_it = RF_map.find("epsilon");
		if(epsilon_it != RF_map.end())
		{
			epsilon = atof((epsilon_it->second).c_str());
		}
	}
	std::cout << numIt << deep << epsilon << std::endl;
	MLL::RF::RFPtr rf = std::make_shared<MLL::RF>(path_file, numIt, deep, epsilon);
	return 0;
}

int KMeans(const char *file = "kmeans.txt")
{
    path_file.append(file);
	int K = 3;
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("KMeans");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> KMeans_map = it->second;
		std::map<std::string, std::string>::iterator K_it = KMeans_map.find("K");
		if(K_it != KMeans_map.end())
		{
			K = atof((K_it->second).c_str());
		}
	}
	std::cout << K << std::endl;
	MLL::KMeans::KMeansPtr kmeans = std::make_shared<MLL::KMeans>(path_file, K);
	return 0;

}

int KNN(const char *file = "knn.txt")
{
    path_file.append(file);
	int K = 10;
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("KNN");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> KNN_map = it->second;
		std::map<std::string, std::string>::iterator K_it = KNN_map.find("K");
		if(K_it != KNN_map.end())
		{
			K = atof((K_it->second).c_str());
		}
	}
	std::cout << K << std::endl;
	MLL::KNN::KNNPtr knn = std::make_shared<MLL::KNN>(path_file, K);
	return 0;
}

int Bayes(const char *file = "bayes.txt")
{
    path_file.append(file);
	MLL::Bayes::BayesPtr bayes = std::make_shared<MLL::Bayes>(path_file);
	return 0;
}

int GMM(const char *file = "GMM.txt")
{
    path_file.append(file);
	int K = 10;
	int iter = 10;
	
	std::map<std::string, std::map<std::string, std::string> >::iterator it = conf_map.find("GMM");
	if(it != conf_map.end())
	{
		std::map<std::string, std::string> GMM_map = it->second;
		std::map<std::string, std::string>::iterator K_it = GMM_map.find("K");
		if(K_it != GMM_map.end())
		{
			K = atoi((K_it->second).c_str());
		}
		std::map<std::string, std::string>::iterator iter_it = GMM_map.find("iter");
		if(iter_it != GMM_map.end())
		{
			iter = atoi((iter_it->second).c_str());
		}
	}
	std::cout << K << iter<< std::endl;
	MLL::GMM::GMMPtr gmm = std::make_shared<MLL::GMM>(path_file,K,iter);
	return 0;
}

int HMM(const char *file = "Ntest.txt")
{
    path_file.append(file);
	MLL::HMM::HMMPtr hmm = std::make_shared<MLL::HMM>(path_file);
	return 0;
}

int HMM_CWS(const char *file = "pku_training.txt")
{
    path_file.append(file);
	MLL::HMM_CWS::HMM_CWSPtr hmm = std::make_shared<MLL::HMM_CWS>(path_file);
	return 0;
}

int MEMM_CWS(const char *file = "pku_training.txt")
{
    path_file.append(file);
	MLL::MEMM_CWS::MEMM_CWSPtr memm = std::make_shared<MLL::MEMM_CWS>(path_file);
	return 0;
}

int CRF_CWS(const char *file = "pku_training.utf8")
{
    path_file.append(file);
	MLL::CRF_CWS::CRF_CWSPtr crf = std::make_shared<MLL::CRF_CWS>(path_file);
	return 0;
}

int load_conf(char *file = "model.conf")
{
	std::string model = "";
	std::map<std::string, std::string> model_map;
	std::ifstream conf_file;
	conf_file.open(file);
	if(conf_file)
	{
		std::string tmpstrline = "";
		while(conf_file.peek() != EOF)
		{
			getline(conf_file, tmpstrline, '\n');
			if('#' != tmpstrline[0] && '\0' != tmpstrline[0])
			{
				if ('[' == tmpstrline[0])
				{
					if(model != "")
						conf_map[model] = model_map;
					for(std::map<std::string, std::string>::iterator it = model_map.begin(); it != model_map.end();)
					{
						std::string name = it->first;
						std::string value = it->second;
						model_map.erase(it++);
					}
					model = tmpstrline.substr(1,tmpstrline.size()-2);
		            std::cout << "model_conf" << model << std::endl;
				}
				else
				{
					int pos = tmpstrline.find(":");
					std::string name = tmpstrline.substr(0,pos);
					std::string value = tmpstrline.substr(pos+1);
					model_map[name] = value;
				}
			}
		}
		conf_map[model] = model_map;
	}
	/*
	for(std::map<std::string, std::map<std::string, std::string> >::iterator it1 = conf_map.begin(); it1 != conf_map.end(); it1++)
	{
		std::string model = it1->first;
		std::map<std::string, std::string> model_map = it1->second;
		std::cout << "model:" << model << std::endl;
		for(std::map<std::string, std::string>::iterator it2 = model_map.begin(); it2 != model_map.end(); it2++)
		{
			std::string name = it2->first;
			std::string value = it2->second;
			std::cout << name << "\t" << value << std::endl;
		}
	}
	*/
	conf_file.close();
	return 0;
}

int main(int argc, char* argv[])
{
	int ret = 0;
    if(argc < 2)
    {
		std::cout<<"arg is error"<<std::endl;
        return -1;
    }
	std::string type = argv[1];
	char *file_name = argv[2];
	
	std::cout<< "---------------------- start load model conf ------------------------" << std::endl;	
	load_conf();
	std::cout<< "---------------------- end load model conf --------------------------" << std::endl;	
	
	std::cout<< "model is " << type << ", " << "filename is " << file_name << std::endl;
	std::string models[20] ={"LineReg","LogReg","SoftMaxReg","DNN","SVM","DTree","CART","Adaboost","RF","KMeans","KNN","Bayes","GMM","HMM","HMM_CWS","MEMM_CWS","CRF_CWS"};
	std::vector<std::string> models_pos(models, models + 20); 
    int (*models_list[])(const char *file) = {LineReg, LogReg,SoftMaxReg,trainDNN,SVM,DTree,CART,Adaboost,RF,KMeans,KNN,Bayes,GMM,HMM,HMM_CWS,MEMM_CWS,CRF_CWS};
    int (*models_ptr) (const char *file) = LogReg;
    
	for(int i = 0; i < models_pos.size(); i++)
    {
        if(type == models_pos[i])
        {
            models_ptr = models_list[i];
		    ret = models_ptr(file_name);
			return ret;
        }
    }
	std::cout<<"model is not exist"<<std::endl;
    return ret;
}
