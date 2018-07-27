#include"KNN.h"


KNN::KNN()
{
}
KNN::KNN(Data_Process dp)
{
	Data = dp.Get_Data(1);
	Data_t = dp.Get_Data(0);
	Train = dp.Get_Matrix(1);
	Test = dp.Get_Matrix(0);
}
float KNN::Distance(vector<float>v1, vector<float>v2, int p) {
	float d = 0.0;
	int flag = 0;
	if (p == 0) {  //余弦距离
		float accum = 0.0;  //算内积
		float modv1 = 0.0, modv2 = 0.0; //算膜
		for (int i = 0; i < v1.size(); i++) {
			accum += v1[i] * v2[i];
			modv1 += v1[i] * v1[i];
			modv2 += v2[i] * v2[i];
		}
		modv1 = sqrtf(modv1);
		modv2 = sqrtf(modv2);
		d = modv1*modv2 / accum;	//因为余弦是越接近1越近，而不是越小，所以此处取倒
		if (modv1 == 0.0)d = 1e10;  //v1 也就是当前样本 mod=0意味着这句话全是新词 直接距离最大
		else if (modv1 != 0.0&& fabsf(modv1*modv2 - accum)<1e-5)d = 1e-10;//处理两句话一致
	}
	else { //曼哈顿和欧式
		for (int i = 0; i < v1.size(); i++) {
			if (fabsf(v1[i])>1e-10)flag = 1; //全是新flag=0
			if (p == 1)d += fabsf(v1[i] - v2[i]);
			else if (p == 2) d += (v1[i] - v2[i])*(v1[i] - v2[i]);
		}
		if (p == 2) d = sqrtf(d);
	}
	if (p != 0 && !flag)d = 1e10;//处理全是新词的情况
	return d;
}
void KNN::Classification(int p, int k,int vaild) {
	ofstream out;
	out.open("KNN_Classification");
	int count = 0;
	for (int i = 0; i < Test.size(); i++) {
		vector<Dis>dis;
		for (int j = 0; j < Train.size(); j++) {
			Dis tmp;
			tmp.No = j;
			tmp.dis = Distance(Test[i].col, Train[j].col, p);
			dis.push_back(tmp);
		}
		sort(dis.begin(), dis.end());
		vector<int>cnt;
		vector<float>min_dis;
		string label = "joy";
		if (dis[0].dis < 1e-9) {//如果距离为0 可以知道这句话在训练集中出现，无需其他
			for (int x = 0; x < 6; x++)
				if (Data[dis[0].No].emotion[x] > 0.99) label = int_emot[x];
		}
		else {
			int Max = -1;
			for (int j = 0; j < 6; j++) {
				cnt.push_back(0); //计数器初始化
				min_dis.push_back(0.0);//加权初始化
			}
			for (int j = 0; j < k; j++) {
				if (dis[j].dis > 1e9)break; //优化距离过大没有实际意义，无视
				for (int x = 0; x < 6; x++)
					if (Data[dis[j].No].emotion[x] > 0.99) { //该情感为1时
						cnt[x]++; //情感出现次数+1
						min_dis[x] += 1 / dis[j].dis / dis[j].dis; //进行加权
						if (cnt[x] > Max || (cnt[x] == Max && min_dis[x] > min_dis[emot_int[label]])) {
							Max = cnt[x]; //取出现次数最多的的情感 次数一样多时看权值
							label = int_emot[x];
						}
						break;
					}
			}
		}
		out << label << endl;
		if (vaild&&Data_t[i].emotion[emot_int[label]]>0.99)count++;
	}
	if (vaild) {
		cout << count << '/' << Data_t.size() << endl;
		cout << (float)count / (float)Data_t.size() << endl;
	}
	out.close();
}
void KNN::Regression(int p, int k, int vaild) {
	ofstream out;
	out.open("KNN_Regression");
	int count = 0;
	for (int i = 0; i <Test.size(); i++) {
		vector<Dis>dis;
		for (int j = 0; j < Train.size(); j++) { //计算距离
			Dis tmp;
			tmp.No = j;
			tmp.dis = Distance(Test[i].col, Train[j].col, p);
			dis.push_back(tmp);
		}
		sort(dis.begin(), dis.end());
		vector<float>tmp;//6种感情的概率
		float sum = 0.0;
		if (dis[0].dis < 1e-9) { //同样处理 完全一致的句子。
			tmp = Data[dis[0].No].emotion;
			sum = 1;
		}
		else {
			int tmp_k = k;
			if (dis[0].dis == 1e10)tmp_k = Data.size(); //全新的句子直接赋予文档平局值
			for (int x = 0; x < 6; x++)
				tmp.push_back(0.0);

			for (int j = 0; j < tmp_k; j++) {//前k个训练集中的样本
				if (tmp_k != Data.size() && dis[j].dis > 1e9)break; //距离过大停止。
				for (int x = 0; x < 6; x++) //计算平局值。
					tmp[x] += Data[dis[j].No].emotion[x] / dis[j].dis / log2(dis[j].dis); //两次加权
			}
			for (int x = 0; x < 6; x++) //求和准备线性放大
				sum += tmp[x];
		}
		for (int j = 0; j < tmp.size(); j++) { //线性放大
			tmp[j] /= sum;
			if (tmp[j] < 1e-2)tmp[j] = 0;
		}
		sum = 0;
		for (int j = 0; j < tmp.size(); j++)sum += tmp[j];
		for (int j = 0; j < tmp.size(); j++) {
			tmp[j] /= sum;
			out << tmp[j] << '\t';
		}
		out << endl;
	}
	out.close();
}