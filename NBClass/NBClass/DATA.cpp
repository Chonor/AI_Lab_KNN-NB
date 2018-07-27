#include"DATA.h"
map<string, int>emot_int;
map<int, string>int_emot;
void init() {
	emot_int["anger"] = 0;
	emot_int["disgust"] = 1;
	emot_int["fear"] = 2;
	emot_int["joy"] = 3;
	emot_int["sad"] = 4;
	emot_int["surprise"] = 5;
	int_emot[0] = "anger";
	int_emot[1] = "disgust";
	int_emot[2] = "fear";
	int_emot[3] = "joy";
	int_emot[4] = "sad";
	int_emot[5] = "surprise";
}
Data_Elem::Data_Elem()
{
	key.num_key = 0;
	key.use_key = 0;
}
Data_Elem::Data_Elem(string str)
{

}
vector<float> Data_Elem::One_hot(vector<string>All_key) {
	vector<float>one_hot;
	for (int i = 0; i < All_key.size(); i++) { //遍历数据集所有词判断 这个词是不是在这条数据中出现
		if (key.key.count(All_key[i])>0)one_hot.push_back(1);	//出现此列为1
		else one_hot.push_back(0);							//否则为0
	}
	return one_hot;

}
vector<float> Data_Elem::TF(int train, vector<string>All_key) {
	vector<float>TF;
	for (int i = 0; i < All_key.size(); i++) {//遍历数据集所有词判断 这个词是不是在这条数据中出现
		if (train&&key.key.count(All_key[i])>0)TF.push_back(((float)key.key[All_key[i]] / (float)key.num_key));
		else if (key.key.count(All_key[i])>0)TF.push_back(((float)key.key[All_key[i]] / (float)key.use_key));
		else TF.push_back(0);					//这个词在这条数据中的出现次数/这条数据中的次词量
	}
	return TF;
}
vector<float>Data_Elem::TF_IDF(int train, vector<string>All_key, int Size, map<string, int>check_key) {
	vector<float>TF_IDF;
	for (int i = 0; i < All_key.size(); i++) {
		if (train&&key.key.count(All_key[i])>0 && key.key.count(All_key[i])<15)TF_IDF.push_back((((float)key.key[All_key[i]] / (float)key.num_key))*log((float)Size / (float)(1 + check_key[All_key[i]])));
		else if (key.key.count(All_key[i])>0 && key.key.count(All_key[i])<15)TF_IDF.push_back((((float)key.key[All_key[i]] / (float)key.use_key))*log((float)Size / (float)(1 + check_key[All_key[i]])));
		//根据公式 这个词在这条数据中的出现次数 / 这条数据中的次词量 * 数据量 /这个词的总出现次数+1
		else TF_IDF.push_back(0);
	}
	return TF_IDF;
}
istream& operator >>(istream &is, Data_Elem &data) {
	string str;
	getline(is, str);
	Data_Elem temp(str);
	data = temp;
	return is;
}

Matrix_Elem::Matrix_Elem()
{
	col.clear();
}
Matrix_Elem::Matrix_Elem(vector<float>col)
{
	this->col = col;
}

Data_Process::Data_Process()
{
	classification = 1;
	train = 1;
	verify = 0;
	src = "";
}
Data_Process::Data_Process(string src, bool classification, bool train, bool verify)
{
	this->classification = classification;
	this->train = train;
	this->verify = verify;
	this->src = src;
}
void Data_Process::Is_Test(string src, int flag) {
	train = !train;
	if (flag)verify = 1;
	else verify = 0;
	this->src = src;
}
void Data_Process::Data_in() {
	string unuse, str;
	ifstream in;
	in.open(src);
	Data_Elem tmp;
	getline(in, unuse);
	while (getline(in, str))
		Str_to_Data(str);
	cout << "Data_In_end\n";
}
void Data_Process::Str_to_Data(string str) {
	Data_Elem tmp;
	tmp.key.num_key = 0;
	tmp.key.use_key = 0;
	string  stmp = "";
	int cnt = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != ','&& str[i] != ' ')
			stmp += str[i];
		if ((train || verify) && cnt == 1) {
			if (classification&&i == str.length() - 1) {
				for (int k = 0; k < 6; k++)tmp.emotion.push_back(0.0);
				tmp.emotion[emot_int[stmp]] = 1;
			}
			else if (str[i] == ',' || i == str.length() - 1) {
				float d_tmp = 0.0;
				stringstream ss(stmp);
				ss >> d_tmp;
				tmp.emotion.push_back(d_tmp);
				stmp = "";
			}
		}
		else if (cnt == 1)break;

		if (cnt == 0 && (str[i] == ' ' || str[i] == ',')) {
#ifdef USE_ALL_KEY
			tmp.key.key_.push_back(stmp);
#endif // !USE_ALL_KEY

			tmp.key.num_key++;   //当前数据行 总词数+1
			if (tmp.key.key.count(stmp) > 0) {  // 这个词已经在这行中出现
				tmp.key.key[stmp]++;
				if (!train)tmp.key.use_key++;
#ifndef USE_ALL_KEY
				tmp.key.key_.push_back(stmp);
#endif // !USE_ALL_KEY
			}
			else if (train) {						//这个词第一次出现
				tmp.key.key[stmp] = 1;
#ifndef USE_ALL_KEY
				tmp.key.key_.push_back(stmp);
#endif // !USE_ALL_KEY
				if (!check_key.count(stmp)) {	//这个词在全部数据中没有出现
					check_key[stmp] = 1;
					All_key.push_back(stmp);
				}
				else check_key[stmp]++;		//这个词在全部词中出现 
			}
			else {						//这个词第一次出现
				if (check_key.count(stmp) > 0) { 	//这个词在全部数据中有出现
					tmp.key.use_key++;
					tmp.key.key[stmp] = 1;
#ifndef USE_ALL_KEY
					tmp.key.key_.push_back(stmp);
#endif // !USE_ALL_KEY
				}
			}
			stmp = "";
			if (str[i] == ',')cnt++;
		}
	}
	if (tmp.key.use_key == 0)tmp.key.use_key = 1;
	if (train)Data.push_back(tmp);
	else Data_t.push_back(tmp);
}
void Data_Process::Train_Data_to_Matrix(int flag) {
	for (int i = 0; i < Data.size(); i++) {
		if (flag == 0) {
			Matrix_Elem tmp(Data[i].One_hot(All_key));
			Train.push_back(tmp);
		}
		else if (flag == 1) {
			Matrix_Elem tmp(Data[i].TF(train, All_key));
			Train.push_back(tmp);
		}
		else if (flag == 2) {
			Matrix_Elem tmp(Data[i].TF_IDF(train, All_key, Data.size(), check_key));
			Train.push_back(tmp);
		}
	}
}
void Data_Process::Test_Data_to_Matrix(int flag) {
	for (int i = 0; i < Data_t.size(); i++) {
		if (flag == 0) {
			Matrix_Elem tmp(Data_t[i].One_hot(All_key));
			Test.push_back(tmp);
		}
		else if (flag == 1) {
			Matrix_Elem tmp(Data_t[i].TF(train, All_key));
			Test.push_back(tmp);
		}
		else if (flag == 2) {
			Matrix_Elem tmp(Data_t[i].TF_IDF(train, All_key, Data.size(), check_key));
			Test.push_back(tmp);
		}
	}
}
void Data_Process::Z_Score() {
	float sum, mean, accum, Std;
	for (int i = 0; i < Train.size(); i++) {
		sum = 0.0;
		accum = 0.0;
		for (int j = 0; j < Train[i].col.size(); j++)
			sum += Train[i].col[j];
		mean = sum / Train[i].col.size();
		for (int j = 0; j < Train[i].col.size(); j++)
			accum += (Train[i].col[j] - mean)*(Train[i].col[j] - mean);
		Std = accum / (Train[i].col.size() - 1);
		Std = sqrtf(Std);
		for (int j = 0; j < Train[i].col.size(); j++)
			if (Std != 0.0)Train[i].col[j] = (Train[i].col[j] - mean) / Std;
			else Train[i].col[j] = 0;
	}
	for (int i = 0; i < Test.size(); i++) {
		sum = 0.0;
		accum = 0.0;
		for (int j = 0; j < Test[i].col.size(); j++)
			sum += Test[i].col[j];
		mean = sum / Test[i].col.size();
		for (int j = 0; j < Test[i].col.size(); j++)
			accum += (Test[i].col[j] - mean)*(Test[i].col[j] - mean);
		Std = accum / (Test[i].col.size() - 1);
		Std = sqrtf(Std);
		for (int j = 0; j < Test[i].col.size(); j++)
			if (Std != 0.0)Test[i].col[j] = (Test[i].col[j] - mean) / Std;
			else Test[i].col[j] = 0;
	}
}
void Data_Process::Min_Max() {
	float Min;
	float Max;
	for (int i = 0; i < Train.size(); i++) {
		Min = 1e10;
		Max = -1e10;
		for (int j = 0; j < Train[i].col.size(); j++) {
			Min = Min > Train[i].col[j] ? Train[i].col[j] : Min;
			Max = Max < Train[i].col[j] ? Train[i].col[j] : Max;
		}
		for (int j = 0; j < Train[i].col.size(); j++)
			if (Max - Min != 0.0)Train[i].col[j] = (Train[i].col[j] - Min) / (Max - Min);
			else Train[i].col[j] = 0;
	}

	for (int i = 0; i < Test.size(); i++) {
		Min = 1e10;
		Max = -1e10;
		for (int j = 0; j < Test[i].col.size(); j++) {
			Min = Min > Test[i].col[j] ? Test[i].col[j] : Min;
			Max = Max < Test[i].col[j] ? Test[i].col[j] : Max;
		}
		for (int j = 0; j < Test[i].col.size(); j++)
			if (Max - Min != 0.0)Test[i].col[j] = (Test[i].col[j] - Min) / (Max - Min);
			else Test[i].col[j] = 0;
	}
}
void Data_Process::Mod_norm() {
	float dis;
	for (int i = 0; i < Train.size(); i++) {
		dis = 0.0;
		for (int j = 0; j < Train[i].col.size(); j++)
			dis += Train[i].col[j] * Train[i].col[j];
		dis = sqrtf(dis);
		for (int j = 0; j < Train[i].col.size(); j++)
			if (dis != 0.0)Train[i].col[j] = Train[i].col[j] / dis;
			else Train[i].col[j] = 0;
	}
	for (int i = 0; i < Test.size(); i++) {
		dis = 0.0;
		for (int j = 0; j < Test[i].col.size(); j++)
			dis += Test[i].col[j] * Test[i].col[j];
		dis = sqrtf(dis);
		for (int j = 0; j < Test[i].col.size(); j++)
			if (dis != 0.0)Test[i].col[j] = Test[i].col[j] / dis;
			else Test[i].col[j] = 0;
	}
}
void Data_Process::Data_norm(int normal) {
	if (normal == 1)
		Z_Score();
	else if (normal == 2)
		Min_Max();
	else if (normal == 3)
		Mod_norm();
	else if (normal == 4) {
		Z_Score();
		Mod_norm();
	}
	else if (normal == 5) {
		Z_Score();
		Min_Max();
		Mod_norm();
	}
	else if (normal == 6) {
		Z_Score();
		Mod_norm();
		Min_Max();
	}
}
vector<Data_Elem> Data_Process::Get_Data(int flag) {
	if (flag)return Data;
	else return Data_t;
}
vector<Matrix_Elem> Data_Process::Get_Matrix(int flag) {
	if (flag)return Train;
	else return Test;
}
vector<string> Data_Process::Get_All_key() {
	return All_key;
}
istream& operator >>(istream &is, Data_Process &data_p) {
	string unuse, str;
	ifstream in;
	in.open(data_p.src);
	Data_Elem tmp;
	getline(in, unuse);
	while (getline(in, str))
		data_p.Str_to_Data(str);
	cout << "Data_In_end\n";
	return is;
}
