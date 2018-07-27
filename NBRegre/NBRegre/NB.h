#ifndef NB_H
#define NB_H
#include"DATA.h"
class NB
{
public:
	NB();
	NB(Data_Process dp);
	void Classification(float alpha,int vaild);
	void Regression(float alpha);
private:
	vector<Data_Elem>Data;
	vector<Data_Elem>Data_t;
	vector<Matrix_Elem>Train;
	vector<Matrix_Elem>Test;
	vector<string>All_key;
	vector<int>Count_key;
	map<string, int>Count_emot_key[6];
	int Count_All_key;
};
#endif // !NB_H
