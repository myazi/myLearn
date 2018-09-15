struct CenAndDis;

double distances(Matrix xOne,Matrix kCen);

Matrix randCent(Matrix x,int kNum);

CenAndDis kMeans(Matrix &x,const int &kNum, const int &Iter);

Matrix subMatrix(const Matrix &x, const Matrix &clusterAssment,const int &label);

int biKmeans(Matrix &x,const int &kNum,const int &Iter);

int kmeans();

