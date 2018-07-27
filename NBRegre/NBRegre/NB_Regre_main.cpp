#include"NB.h"
#include<Windows.h>
using namespace std;

int main() {
	init();
	string src1 = "train_set.csv", src2 = "validation_set.csv", src3 = "test_set.csv";//两个文件路径
	//getline(cin, src1);
	//getline(cin, src2);
	bool CLASS = 0;  //1为分类  0为回归
	int Matrix_type = 1;//One-hot 为0   TF 为1  TF-IDF为2 (建议不要修改)
	int vaild = 0;   //使用的是验证集为1 测试集为2
	Data_Process dp(src1, CLASS, 1, 0);   //数据处理
	dp.Data_in();
	dp.Train_Data_to_Matrix(Matrix_type);
	dp.Is_Test(src3, vaild);
	dp.Data_in();
	dp.Test_Data_to_Matrix(Matrix_type);
	dp.Data_norm(0);//数据归一化 建议不要修改
	NB nb(dp);
	nb.Regression(0.0059);
	//请看目录下的 NB_Regression;
	cout << "end\n";
	Sleep(3000);//*/
}