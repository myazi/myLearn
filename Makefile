INCLUDE = ./include
OBJS = main.o Adaboost.o Bayes.o CART.o CRF_CWS.o DNN.o DTree.o GMM.o Hash.o HMM_CWS.o HMM.o kMeans.o  KNN.o LineReg.o LogReg.o MDP.o ME.o MEMM_CWS.o PCA.o RF.o SoftMaxReg.o SVD.o SVM.o LoadData.o Matrix.o MatrixOpe.o

HH = ./include/LoadData.h ./include/Matrix.h ./include/MatrixOpe.h
CPP = LoadData.cpp Matrix.cpp Matrix.cpp

ifndef CXX
CXX = $(CXX) $(CXXFLAGS)
endif
CXXFLAGS=-pipe \
         -g \
  		 -W \
  		 -Wall \
  		 -fPIC \
  		 -std=c++11 \
  		 -fno-omit-frame-pointer \
  		 -fpermissive \
  		 -O3 \
  		 -ffast-math

INCPATH=-I./include/ \
		-I./include/familia \
  		-I./third_party/include \
		-I$(PYTHON_INCLUDE)

ML : $(OBJS)
	$(CXX) $(CXXFLAGS) -o ML $(OBJS) 
main.o : main.cpp  
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) main.cpp 
Adaboost.o : Adaboost.cpp 
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Adaboost.cpp
Bayes.o : Bayes.cpp 
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Bayes.cpp
CART.o : CART.cpp ./include/CART.h
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) CART.cpp
CRF_CWS.o : CRF_CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) CRF_CWS.cpp
DNN.o : DNN.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) DNN.cpp
DTree.o : DTree.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) DTree.cpp 
GMM.o : GMM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) GMM.cpp
Hash.o : Hash.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Hash.cpp
HMM_CWS.o : HMM_CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) HMM_CWS.cpp
HMM.o : HMM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) HMM.cpp
kMeans.o : kMeans.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) kMeans.cpp
KNN.o : KNN.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) KNN.cpp
LineReg.o : LineReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LineReg.cpp
LogReg.o : LogReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LogReg.cpp
MDP.o : MDP.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MDP.cpp
ME.o : ME.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) ME.cpp
MEMM_CWS.o : MEMM_CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MEMM_CWS.cpp
PCA.o : PCA.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) PCA.cpp
RF.o : RF.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) RF.cpp
SoftMaxReg.o : SoftMaxReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SoftMaxReg.cpp
SVD.o : SVD.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SVD.cpp
SVM.o : SVM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SVM.cpp
LoadData.o : LoadData.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LoadData.cpp
Matrix.o : Matrix.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Matrix.cpp
MatrixOpe.o : MatrixOpe.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MatrixOpe.cpp
.PHONY : clean
clean :
	rm $(OBJS)

