#include"NB.h"
#include<Windows.h>
using namespace std;

int main() {
	init();
	string src1 = "train_set.csv", src2 = "validation_set.csv", src3 = "test_set.csv";//�����ļ�·��s
	//getline(cin, src1);
	//getline(cin, src2);
	bool CLASS = 1;  //1Ϊ����  0Ϊ�ع�
	int Matrix_type = 1;//One-hot Ϊ0   TF Ϊ1  TF-IDFΪ2 (���鲻Ҫ�޸�)
	int vaild = 0;   //ʹ�õ�����֤��Ϊ1 ���Լ�Ϊ0
	Data_Process dp(src1, CLASS, 1, 0);   //���ݴ���
	dp.Data_in();
	dp.Train_Data_to_Matrix(Matrix_type);
	dp.Is_Test(src3, vaild);
	dp.Data_in();
	dp.Test_Data_to_Matrix(Matrix_type);
	dp.Data_norm(0);//���ݹ�һ�� ���鲻Ҫ�޸�
	NB nb(dp);
	nb.Classification(1,vaild);
	//�뿴Ŀ¼�µ� NB_Classification;
	cout << "end\n";
	system("pause");//*/
}