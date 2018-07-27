#ifndef DATA_H
#define DATA_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<cmath>
#include<iomanip>
#include<thread>
#include<functional>
#include<numeric> 
#include<algorithm>
#include<random>
#include<cstdlib>
#include<ctime>
using namespace std;

struct Key
{
	map<string, int>key;
	vector<string>key_;
	int num_key;
	int use_key;
};
class Data_Elem
{
public:
	Data_Elem();
	Data_Elem(string str);
	friend istream& operator >>(istream &is, Data_Elem &data);
	vector<float> One_hot(vector<string>All_key);
	vector<float>TF(int train, vector<string>All_key);
	vector<float>TF_IDF(int train, vector<string>All_key, int Size, map<string, int>check_key);
	vector<float>emotion;
	Key key;
};

class Matrix_Elem
{
public:
	Matrix_Elem();
	Matrix_Elem(vector<float>col);
	vector<float>col;
};
struct Dis
{
	int No; //存储训练集序号用于之后的判断
	float dis;//距离
	bool operator<(const Dis b) const//重载
	{
		return this->dis < b.dis;
	}
};
class Data_Process
{
public:
	Data_Process();
	Data_Process(string src, bool classification, bool train, bool verify);
	void Is_Test(string src, int flag);
	void Data_in();
	friend istream& operator >>(istream &is, Data_Process &data_p);
	void Train_Data_to_Matrix(int flag);
	void Test_Data_to_Matrix(int flag);
	void Data_norm(int normal);
	vector<Data_Elem> Get_Data(int flag);
	vector<Matrix_Elem> Get_Matrix(int flag);
	vector<string> Get_All_key();
private:
	vector<Matrix_Elem>Train;
	vector<Matrix_Elem>Test;
	map<string, int>check_key;
	vector<string>All_key;
	vector<Data_Elem>Data;
	vector<Data_Elem>Data_t;
	bool classification, train, verify;
	string src;
	void Str_to_Data(string str);
	void Z_Score();
	void Min_Max();
	void Mod_norm();
};

void init();

extern map<string, int>emot_int;
extern map<int, string>int_emot;

#endif