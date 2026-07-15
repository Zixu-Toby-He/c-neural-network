#include <stdio.h>
#include <stdlib.h>

#include "../neural/activate.h"
#include "../neural/math base.h"
#include "../neural/neural.h"

#include "other.h"

#define printDoubleArray(_aRrAy,length)               {printf(#_aRrAy"：[%i]\n[ ", length);for(size_t __CoUnTiNg=0;__CoUnTiNg<length;__CoUnTiNg++){printf("%g, ",(double)_aRrAy[__CoUnTiNg]);};puts("]");}
#define printLongLongInt(x)                           printf(#x"：%lli\n",(long long int)x)
#define printDouble(x)                                printf(#x"：%g\n",(double)x)



int main()
{
	char buffer[1024];
	puts("创建并整理数据集");
	数据集_t* 训练集 = 数据集_新建(2, 5, 训练集长度);
	数据集_t* 验证集 = 数据集_新建(2, 5, 验证集长度);
	填充训练集(训练集);
	填充验证集(验证集);
	printf("(*训练集).结果集数量 = %u\n", (*训练集).结果集数量);
	printf("(*验证集).结果集数量 = %u\n", (*验证集).结果集数量);
	printf("训练集[%u] = [[%lf, %lf],[%lf, %lf, %lf, %lf, %lf]]\n",
		0,
		数据集_获取一组自变量参数(训练集, 0)[0],
		数据集_获取一组自变量参数(训练集, 0)[1],
		数据集_获取一组函数值参数(训练集, 0)[0],
		数据集_获取一组函数值参数(训练集, 0)[1],
		数据集_获取一组函数值参数(训练集, 0)[2],
		数据集_获取一组函数值参数(训练集, 0)[3],
		数据集_获取一组函数值参数(训练集, 0)[4]
	);
	printf("验证集[%u] = [[%lf, %lf],[%lf, %lf, %lf, %lf, %lf]]\n",
		0,
		数据集_获取一组自变量参数(验证集, 0)[0],
		数据集_获取一组自变量参数(验证集, 0)[1],
		数据集_获取一组函数值参数(验证集, 0)[0],
		数据集_获取一组函数值参数(验证集, 0)[1],
		数据集_获取一组函数值参数(验证集, 0)[2],
		数据集_获取一组函数值参数(验证集, 0)[3],
		数据集_获取一组函数值参数(验证集, 0)[4]
	);
	puts("");
	puts("");
	puts("数据集创建并填充完成");

	puts("创建神经网络");

#define _层数 3
	unsigned int 各层参数个数[_层数 + 1] = { 2, 10, 10, 5 };
	激励函数_t   各层激励函数[_层数] = { relu激励函数, relu激励函数, sigmoid激励函数 };
	
	puts("神经网络创建完成");
	
	
	//FILE* 初始化文件 = fopen("../resource/randnum.bin", "rb");
	//char 缓存池[1024];
	//int 参数个数 = 1024;
	//fread(缓存池, sizeof(double), 参数个数, 初始化文件);
	//fclose(初始化文件);
	//
	//// printLongLongInt((*神经网络).层数);
	//// printDoubleArray((*神经网络).各层参数个数, (*神经网络).层数 + 1);
	//// printDoubleArray((*神经网络).缓存池, (*神经网络).矩阵参数个数);
	//// printf("(*神经网络).缓存池：0x%p\n",(*神经网络).缓存池);
	//// printLongLongInt((*神经网络).矩阵参数个数);
	//// printf("(*神经网络).所有层[0].变换矩阵：0x%p\n",(*神经网络).所有层[0].变换矩阵);
	//// printf("(*神经网络).所有层[1].变换矩阵：0x%p\n",(*神经网络).所有层[1].变换矩阵);
	//// printf("(*神经网络).所有层[2].变换矩阵：0x%p\n",(*神经网络).所有层[2].变换矩阵);
	//
	//puts("开始训练");
	//puts("训练完成");
	//puts("");
	//
	//double 推理结果[5];
	//for (size_t i = 0; i < 验证集大小; i++)
	//{
	//	printDoubleArray(推理结果, 5);
	//	printf("经推理，点 (%f, %f) 在第 %i 象限\n", 验证数据[i][0], 验证数据[i][1], 翻译推理结果(推理结果));
	//}
	//puts("");
	//
	//
	//puts("销毁神经网络");
	//销毁神经网络(神经网络);
	//puts("神经网络销毁完成");
	return 0;
}