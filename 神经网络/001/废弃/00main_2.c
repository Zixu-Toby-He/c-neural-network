#include <stdio.h>
#include <stdlib.h>

#include "neural/activate.h"
#include "neural/math base.h"
#include "neural/neural.h"

#include "other.h"

#define printDoubleArray(_aRrAy,length)               {printf(#_aRrAy"：[%i]\n[ ", length);for(size_t __CoUnTiNg=0;__CoUnTiNg<length;__CoUnTiNg++){printf("%g, ",(double)_aRrAy[__CoUnTiNg]);};puts("]");}
#define printLongLongInt(x)                           printf(#x"：%lli\n",(long long int)x)
#define printDouble(x)                                printf(#x"：%g\n",(double)x)

int 翻译推理结果(double* 推理结果)
{
	int 返回值 = 0;
	for (size_t i = 1; i < 5; i++)
	{
		返回值 = (推理结果[返回值]<推理结果[i])? i : 返回值;
	}
	return 返回值;
}

int main()
{
	#define _层数 2
	训练集_t 训练集;
	puts("整理数据集");
	训练集.结果集数量 = sizeof(训练数据)/sizeof(训练数据[0]);
	训练集.自变量单组长度 = 2;
	训练集.函数值单组长度 = 5;
	double 自变量数据集[2 * sizeof(训练数据)/sizeof(训练数据[0])];
	double 函数值数据集[5 * sizeof(训练数据)/sizeof(训练数据[0])];
	训练集.自变量数据集 = 自变量数据集;
	训练集.函数值数据集 = 函数值数据集;
	整理数据(训练集.自变量数据集, 训练集.函数值数据集);

	unsigned int 各层参数个数[_层数+1] = {2, 10, 5};
	puts("创建神经网络");
	// 激励函数_t 各层激励函数[_层数] = {三次方激励函数, 三次方激励函数};
	// 激励函数_t 各层激励函数[_层数] = {sigmoid激励函数, sigmoid激励函数};
	激励函数_t 各层激励函数[_层数] = {relu激励函数, sigmoid激励函数};
	神经网络_t* 神经网络 = 新建神经网络(_层数, 各层参数个数, 各层激励函数);
	puts("神经网络创建完成");

	// 初始化初步方法，
	FILE* 初始化文件 = fopen("randnum.bin","rb");
	fread((*神经网络).缓存池, sizeof(double), (*神经网络).矩阵参数个数, 初始化文件);
	fclose(初始化文件);

	printLongLongInt((*神经网络).层数);
	printDoubleArray((*神经网络).各层参数个数, (*神经网络).层数+1);
	printDoubleArray((*神经网络).缓存池, (*神经网络).矩阵参数个数);
	// printf("(*神经网络).缓存池：0x%p\n",(*神经网络).缓存池);
	// printLongLongInt((*神经网络).矩阵参数个数);
	// printf("(*神经网络).所有层[0].变换矩阵：0x%p\n",(*神经网络).所有层[0].变换矩阵);
	// printf("(*神经网络).所有层[1].变换矩阵：0x%p\n",(*神经网络).所有层[1].变换矩阵);
	// printf("(*神经网络).所有层[2].变换矩阵：0x%p\n",(*神经网络).所有层[2].变换矩阵);

	puts("开始训练");
	训练(神经网络, &训练集, 64);
	puts("训练完成");
	printDoubleArray((*神经网络).缓存池, (*神经网络).矩阵参数个数);
	puts("");

	unsigned int 验证集大小 = sizeof(验证数据)/sizeof(验证数据[0]);
	double 推理结果[5];
	for (size_t i = 0; i < 验证集大小; i++)
	{
		推理计算(神经网络, 推理结果, 验证数据[i]);
		printDoubleArray(推理结果,5);
		printf("经推理，点 (%f, %f) 在第 %i 象限\n", 验证数据[i][0], 验证数据[i][1],翻译推理结果(推理结果));
	}
	puts("");
	

	puts("销毁神经网络");
	销毁神经网络(神经网络);
	puts("神经网络销毁完成");
	return 0;
}