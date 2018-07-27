
#include"NB.h"

NB::NB()
{
}
NB::NB(Data_Process dp) {
	Data = dp.Get_Data(1);
	Data_t = dp.Get_Data(0);
	Train = dp.Get_Matrix(1);
	Test = dp.Get_Matrix(0);
	All_key = dp.Get_All_key();
	Count_All_key = 0;
	for (int i = 0; i < 6; i++) { //计算各情感总词数
		Count_key.push_back(0);
		Emot_cnt.push_back(0);
	}
	for (int i = 0; i < Data.size(); i++) {
		for (int j = 0; j < 6; j++)
			if (Data[i].emotion[j] > 0.99) {    //计算各情感总词数
				Count_key[j] += Data[i].key.num_key;
				Emot_cnt[j]++;
			}
	}
	for (int i = 0; i < 6; i++) {  //计算词数
		Count_All_key += Count_key[i];
	}
	for (int i = 0; i< Data.size(); i++) {
		for (int k = 0; k < 6; k++) {
			if (Data[i].emotion[k] > 0.99)
				for (int j = 0; j < Data[i].key.key_.size(); j++) {
					if (Count_emot_key[k].count(Data[i].key.key_[j]) > 0)
						Count_emot_key[k][Data[i].key.key_[j]]++;
					else Count_emot_key[k][Data[i].key.key_[j]] = 1;
					//计算各情感中的单词出现情况
				}
		}
	}
}
void NB::Classification(float alpha, int vaild) {
	ofstream out;
	out.open("NB_Classification");
	int count = 0;
	for (int i = 0; i < Data_t.size(); i++) {
		float P_tmp[6];
		for (int j = 0; j < 6; j++) {
#ifndef USE_FLIE
			P_tmp[j] = (float)Count_key[j] / Count_All_key; //计算先验概率
#endif // !USE_FLIE
#ifdef USE_FILE
			P_tmp[j] = (float)Emot_cnt[j] / Count_All_key; //计算先验概率
#endif // USE_FILE
		}
			
		for (int j = 0; j < Data_t[i].key.key_.size(); j++) { //根据样本词典计算条件概率
			for (int k = 0; k < 6; k++) {
				if (Count_emot_key[k].count(Data_t[i].key.key_[j]) > 0) //使用拉普拉斯平滑
					P_tmp[k] *= (float)(Count_emot_key[k][Data_t[i].key.key_[j]] + alpha) / (Count_key[k] + alpha*All_key.size());
				else P_tmp[k] *= (float)alpha * 1 / (Count_key[k] + alpha*All_key.size());

			}
		}
#ifdef TEST
		float sum = 0;
		for (int j = 0; j <6; j++)sum += P_tmp[j];
		for (int j = 0; j < 6; j++) {
			cout << int_emot[j] << ':' << (float)P_tmp[j] / sum << endl;
		}
		int Max = 0;
		for (int j = 1; j < 6; j++)  //取最大值
			if (P_tmp[j] > P_tmp[Max])Max = j;
		cout << int_emot[Max] << endl;
#endif // TEST
#ifndef TEST
		int Max = 0;
		for (int j = 1; j < 6; j++)  //取最大值
			if (P_tmp[j] > P_tmp[Max])Max = j;
		out << int_emot[Max] << endl;
#endif // !TEST
		if(vaild)if (Data_t[i].emotion[Max]>0.99)count++;
	}
	if (vaild) {
		cout << alpha << ' ' << count << '/' << Data_t.size() << endl;
		cout << (float)count / (float)Data_t.size() << endl;
	}
}
void NB::Regression(float alpha) {
	ofstream out;
	out.open("NB_Regression");
	for (int i = 0; i < Data_t.size(); i++) {
		float P_tmp[6];
		vector<float>OH = Data_t[i].One_hot(All_key);
		for (int j = 0; j < 6; j++)P_tmp[j] = 0;
		for (int j = 0; j < 6; j++) { //6种
			for (int k = 0; k < Train.size(); k++) { //算一下每句话情感
				float tmp = Data[k].emotion[j];
				if (tmp < 1e-5)continue;			//情感为0无视
				for (int x = 0; x < OH.size(); x++) //出现的词再累乘训练集词频
					if (OH[x]>0.99)tmp *= (Train[k].col[x] + alpha) / (1 + alpha*All_key.size());
				P_tmp[j] += tmp;
			}
		}
		float sum = 0.0;
		for (int j = 0; j < 6; j++) //求和
			sum += P_tmp[j];
		for (int j = 0; j < 6; j++) { //线性放大
			P_tmp[j] /= sum;
			if (P_tmp[j]< 1e-2)P_tmp[j] = 0;
			//out << tmp[j] << '\t';
		}
		sum = 0;
		for (int j = 0; j <6; j++)sum += P_tmp[j];
		for (int j = 0; j < 6; j++) {
			P_tmp[j] /= sum;
			out << P_tmp[j] << '\t';
		}
		out << endl;
	}
}
