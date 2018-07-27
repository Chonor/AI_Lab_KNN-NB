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
	if (p == 0) {  //���Ҿ���
		float accum = 0.0;  //���ڻ�
		float modv1 = 0.0, modv2 = 0.0; //��Ĥ
		for (int i = 0; i < v1.size(); i++) {
			accum += v1[i] * v2[i];
			modv1 += v1[i] * v1[i];
			modv2 += v2[i] * v2[i];
		}
		modv1 = sqrtf(modv1);
		modv2 = sqrtf(modv2);
		d = modv1*modv2 / accum;	//��Ϊ������Խ�ӽ�1Խ����������ԽС�����Դ˴�ȡ��
		if (modv1 == 0.0)d = 1e10;  //v1 Ҳ���ǵ�ǰ���� mod=0��ζ����仰ȫ���´� ֱ�Ӿ������
		else if (modv1 != 0.0&& fabsf(modv1*modv2 - accum)<1e-5)d = 1e-10;//�������仰һ��
	}
	else { //�����ٺ�ŷʽ
		for (int i = 0; i < v1.size(); i++) {
			if (fabsf(v1[i])>1e-10)flag = 1; //ȫ����flag=0
			if (p == 1)d += fabsf(v1[i] - v2[i]);
			else if (p == 2) d += (v1[i] - v2[i])*(v1[i] - v2[i]);
		}
		if (p == 2) d = sqrtf(d);
	}
	if (p != 0 && !flag)d = 1e10;//����ȫ���´ʵ����
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
		if (dis[0].dis < 1e-9) {//�������Ϊ0 ����֪����仰��ѵ�����г��֣���������
			for (int x = 0; x < 6; x++)
				if (Data[dis[0].No].emotion[x] > 0.99) label = int_emot[x];
		}
		else {
			int Max = -1;
			for (int j = 0; j < 6; j++) {
				cnt.push_back(0); //��������ʼ��
				min_dis.push_back(0.0);//��Ȩ��ʼ��
			}
			for (int j = 0; j < k; j++) {
				if (dis[j].dis > 1e9)break; //�Ż��������û��ʵ�����壬����
				for (int x = 0; x < 6; x++)
					if (Data[dis[j].No].emotion[x] > 0.99) { //�����Ϊ1ʱ
						cnt[x]++; //��г��ִ���+1
						min_dis[x] += 1 / dis[j].dis / dis[j].dis; //���м�Ȩ
						if (cnt[x] > Max || (cnt[x] == Max && min_dis[x] > min_dis[emot_int[label]])) {
							Max = cnt[x]; //ȡ���ִ������ĵ���� ����һ����ʱ��Ȩֵ
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
		for (int j = 0; j < Train.size(); j++) { //�������
			Dis tmp;
			tmp.No = j;
			tmp.dis = Distance(Test[i].col, Train[j].col, p);
			dis.push_back(tmp);
		}
		sort(dis.begin(), dis.end());
		vector<float>tmp;//6�ָ���ĸ���
		float sum = 0.0;
		if (dis[0].dis < 1e-9) { //ͬ������ ��ȫһ�µľ��ӡ�
			tmp = Data[dis[0].No].emotion;
			sum = 1;
		}
		else {
			int tmp_k = k;
			if (dis[0].dis == 1e10)tmp_k = Data.size(); //ȫ�µľ���ֱ�Ӹ����ĵ�ƽ��ֵ
			for (int x = 0; x < 6; x++)
				tmp.push_back(0.0);

			for (int j = 0; j < tmp_k; j++) {//ǰk��ѵ�����е�����
				if (tmp_k != Data.size() && dis[j].dis > 1e9)break; //�������ֹͣ��
				for (int x = 0; x < 6; x++) //����ƽ��ֵ��
					tmp[x] += Data[dis[j].No].emotion[x] / dis[j].dis / log2(dis[j].dis); //���μ�Ȩ
			}
			for (int x = 0; x < 6; x++) //���׼�����ԷŴ�
				sum += tmp[x];
		}
		for (int j = 0; j < tmp.size(); j++) { //���ԷŴ�
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