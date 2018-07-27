#include"KNN.h"
#include<Windows.h>
using namespace std;

int main() {
	init();
	string src1 = "train_set.csv", src2 = "validation_set.csv", src3 = "test_set.csv";//两个文件路径s
	//getline(cin, src1);
	//getline(cin, src2);
	bool CLASS = 1;  //1为分类  0为回归
	int Matrix_type = 2;//One-hot 为0   TF 为1  TF-IDF为2 (建议不要修改)
	int vaild = 0;   //使用的是验证集为1 测试集为2
	int p = 0;		//0：余弦相似度  1：曼哈顿 2：欧几里得
	int k = 11;		//k值
	Data_Process dp(src1, CLASS, 1, 0);   //数据处理
	dp.Data_in();
	dp.Train_Data_to_Matrix(Matrix_type);
	dp.Is_Test(src3, vaild);
	dp.Data_in();
	dp.Test_Data_to_Matrix(Matrix_type);
	dp.Data_norm(3);//数据归一化 建议不要修改

	KNN knn(dp);
	knn.Classification(p, k, vaild);
	//请看目录下的 KNN_Classification;
	cout << "end\n";
	Sleep(3000);//*/
}