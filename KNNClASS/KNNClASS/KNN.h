#ifndef KNN_H
#define KNN_H

#include"DATA.h"
class KNN
{
public:
	KNN();
	KNN(Data_Process dp);
	void Classification(int p, int k, int vaild);
	void Regression(int p, int k, int vaild);
private:
	vector<Data_Elem>Data;
	vector<Data_Elem>Data_t;
	vector<Matrix_Elem>Train;
	vector<Matrix_Elem>Test;
	float Distance(vector<float>v1, vector<float>v2, int p);
};

#endif // 


