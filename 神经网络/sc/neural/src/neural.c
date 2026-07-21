#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "neural.h"
#include "math_base.h"

static unsigned int 计算矩阵池元素个数(unsigned int* 各层参数个数, unsigned int 层数);
static unsigned int 计算中间缓存池元素个数(unsigned int* 各层参数个数, unsigned int 层数);
static size_t 按double对齐(size_t 大小);
static size_t 按指针对齐(size_t 大小);

void 单层网络_线性传播(单层神经网络_t* 单层网络)
{
	for (unsigned int i = 0; i < (*单层网络).传出参数个数; i++)
	{
		(*单层网络).该层线性数值[i] = 0;
		for (unsigned int j = 0; j < (*单层网络).传入参数个数; j++)
		{
			(*单层网络).该层线性数值[i] += ((*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + j] * (*单层网络).该层传入数值[j]);
		}
		(*单层网络).该层线性数值[i] += (*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + (*单层网络).传入参数个数];
	}
}

void 单层网络_传播(单层神经网络_t* 单层网络)
{
	单层网络_线性传播(单层网络);
	(*单层网络).激励函数(
		(*单层网络).该层激励数值,
		(*单层网络).该层线性数值,
		(*单层网络).传出参数个数,
		NULL
	);
}

神经网络_t* 新建神经网络(unsigned int 层数, unsigned int *各层参数个数, 激励函数_t *各层激励函数)
{
	unsigned int 矩阵缓存个数 = 计算矩阵池元素个数(各层参数个数, 层数);
	unsigned int 中间缓存个数 = 计算中间缓存池元素个数(各层参数个数, 层数);
	size_t 各层参数个数偏移 = sizeof(神经网络_t);
	size_t 所有层偏移 = 按指针对齐(各层参数个数偏移 + (层数 + 1) * sizeof(unsigned int));
	size_t 缓存池偏移 = 按double对齐(所有层偏移 + 层数 * sizeof(单层神经网络_t));
	size_t 总空间大小 = 缓存池偏移 + (矩阵缓存个数 + 中间缓存个数) * sizeof(double);

	char* 申请内存 = (char*)malloc(总空间大小);
	if (申请内存 == NULL)
	{
		return NULL;
	}

	神经网络_t* 网络 = (神经网络_t*)申请内存;
	(*网络).层数 = 层数;
	(*网络).各层参数个数 = (unsigned int*)(申请内存 + 各层参数个数偏移);
	(*网络).所有层 = (单层神经网络_t*)(申请内存 + 所有层偏移);
	(*网络).缓存池 = (double*)(申请内存 + 缓存池偏移);
	(*网络).矩阵参数个数 = 矩阵缓存个数;
	(*网络).总空间大小 = (unsigned int)总空间大小;

	double* 当前矩阵缓存位置 = (*网络).缓存池;
	double* 当前中间缓存位置 = (*网络).缓存池 + 矩阵缓存个数;

	for (unsigned int i = 0; i < 层数; i++)
	{
		(*网络).各层参数个数[i]        = 各层参数个数[i];
		(*网络).所有层[i].传入参数个数 = 各层参数个数[i];
		(*网络).所有层[i].传出参数个数 = 各层参数个数[i + 1];
		(*网络).所有层[i].激励函数     = 各层激励函数[i];
		(*网络).所有层[i].变换矩阵     = 当前矩阵缓存位置;
		(*网络).所有层[i].该层线性数值 = 当前中间缓存位置;
		(*网络).所有层[i].该层激励数值 = 当前中间缓存位置 + 各层参数个数[i + 1];
		(*网络).所有层[i].该层传入数值 = NULL;
		当前矩阵缓存位置 += (((*网络).所有层[i].传入参数个数 + 1) * (*网络).所有层[i].传出参数个数);
		当前中间缓存位置 += 2 * 各层参数个数[i + 1];
	}
	(*网络).各层参数个数[层数] = 各层参数个数[层数];
	for (unsigned int i = 1; i < 层数; i++)
	{
		(*网络).所有层[i].该层传入数值 = (*网络).所有层[i - 1].该层激励数值;
	}
	return 网络;
}

void 推理计算(神经网络_t* 网络, double* 输出结果, double* 输入参数)
{
	(*网络).所有层[0].该层传入数值 = 输入参数;
	for (unsigned int i = 0; i < (*网络).层数; i++)
	{
		单层网络_传播(&(*网络).所有层[i]);
	}
	double* 最终结果 = (*网络).所有层[(*网络).层数-1].该层激励数值;
	if(输出结果 != 最终结果)
	{
		memcpy(输出结果, 最终结果, (*网络).所有层[(*网络).层数-1].传出参数个数 * sizeof(double));
	}
	(*网络).所有层[0].该层传入数值 = NULL;
}

double 评价函数(double* 矩阵参数, unsigned int 矩阵参数个数, void* 神经网络与训练集)
{
	//  消除警告
	(void)矩阵参数;
	(void)矩阵参数个数;
	/* 该函数不会进行匹配校验 */
	神经网络_t* 神经网络 = (神经网络_t*)(((void**)神经网络与训练集)[0]);
	训练集_t* 训练集     = (训练集_t*)  (((void**)神经网络与训练集)[1]);
	double* 推理结果 = (*神经网络).所有层[(*神经网络).层数-1].该层激励数值;
	double 距离平方和 = 0;
	for (size_t i = 0; i < (*训练集).结果集数量; i++)
	{
		推理计算(神经网络, 推理结果, (*训练集).自变量数据集 + i * (*训练集).自变量单组长度);
		距离平方和 += 获取两点距离平方(
			推理结果,
			(*训练集).函数值数据集 + i * (*训练集).函数值单组长度,
			(*训练集).函数值单组长度
		);
	}
	return 距离平方和;
}

// 合法调用方法：
//     写入梯度下降日志(日志文件, 轮数, 偏差, false)
//     写入梯度下降日志(日志文件,    0,    0, true)
static void 写入梯度下降日志(FILE* 日志文件, unsigned int 轮数, double 偏差, bool 清空缓冲区)
{
	static struct
	{
		unsigned int 轮数;
		double 偏差;
	}
	写入数据[256] = {
		{0, 0}
	};
	static unsigned int i = 0;
	static char 缓冲区[8192];
	static bool 文件头 = false;

	if (日志文件 == NULL)
	{
		return;
	}

	if (!文件头)
	{
		fprintf(日志文件, "i,diff\n");
		文件头 = true;
	}

	if (清空缓冲区)
	{
		unsigned int k = 0;
		for (unsigned int j = 0; j < i; j++)
		{
			k += sprintf(缓冲区 + k, "%u,%.17g\n", 写入数据[j].轮数, 写入数据[j].偏差);
		}
		i = 0;
		if (k > 0)
		{
			fputs(缓冲区, 日志文件);
		}
		fflush(日志文件);
		return;
	}

	写入数据[i].轮数 = 轮数;
	写入数据[i].偏差 = 偏差;
	i++;

	if (i >= 256)
	{
		unsigned int k = 0;
		for (unsigned int j = 0; j < 256; j++)
		{
			k += sprintf(缓冲区 + k, "%u,%.17g\n", 写入数据[j].轮数, 写入数据[j].偏差);
		}
		i = 0;
		fputs(缓冲区, 日志文件);
	}
}

static void 写入训练日志(void* 日志文件, const char* 格式, ...)
{
	va_list 参数;
	va_start(参数, 格式);
	va_list 参数副本;
	va_copy(参数副本, 参数);
	vprintf(格式, 参数);
	if (日志文件 != NULL)
	{
		vfprintf((FILE*)日志文件, 格式, 参数副本);
	}
	va_end(参数副本);
	va_end(参数);
}

void 训练_带日志(神经网络_t* 网络, 训练集_t* 训练集, unsigned int 训练轮数, char* 日志文件路径)
{
	unsigned int 神经网络输入长度 = (*网络).所有层[0].传入参数个数;
	unsigned int 神经网络输出长度 = (*网络).所有层[(*网络).层数-1].传出参数个数;
	FILE* 梯度下降日志文件;
	if (日志文件路径)
	{
		梯度下降日志文件 = fopen(日志文件路径, "w");
	}
	else
	{
		梯度下降日志文件 = NULL;
	}

	if ((神经网络输入长度 != (*训练集).自变量单组长度) || (神经网络输出长度 != (*训练集).函数值单组长度))
	{
		写入训练日志(NULL, "神经网络与数据集输入输出长度不匹配。\n");
		if (梯度下降日志文件)
		{
			fclose(梯度下降日志文件);
		}
		return;
	}
	if (训练轮数 == 0)
	{
		写入训练日志(NULL, "训练轮数为 0，保持当前权重。\n");
		if (梯度下降日志文件)
		{
			fclose(梯度下降日志文件);
		}
		return;
	}
	void* 状态参数[2] = {(void*)网络, (void*)训练集};
	const unsigned int 单步轮数 = 8;
	unsigned int 已训练轮数 = 0;
	写入训练日志(NULL, "开始训练\n");
	double 当前步长 = 128.0;
	double 当前误差 = 评价函数((*网络).缓存池, (*网络).矩阵参数个数, (void*)状态参数);
	写入梯度下降日志(梯度下降日志文件, 已训练轮数, 当前误差, false);
	while (已训练轮数 < 训练轮数)
	{
		unsigned int 本次轮数 = 训练轮数 - 已训练轮数;
		if (本次轮数 > 单步轮数)
		{
			本次轮数 = 单步轮数;
		}

		当前步长 = 梯度下降求解_本地修改(
			(*网络).缓存池,
			(*网络).矩阵参数个数,
			评价函数,
			(void*)状态参数,
			当前步长,
			本次轮数
		);
		已训练轮数 += 本次轮数;
		当前误差 = 评价函数((*网络).缓存池, (*网络).矩阵参数个数, (void*)状态参数);
		写入梯度下降日志(梯度下降日志文件, 已训练轮数, 当前误差, false);
		if(当前步长 < 1e-10)
		{
			写入训练日志(NULL, "当前模型满足要求，退出训练。轮数：%u，误差：%.17g，当前步长：%g\n", 已训练轮数, 当前误差, 当前步长);
			break;
		}
	}
	写入训练日志(NULL, "训练完成。轮数：%u，误差：%.17g，当前步长：%g\n", 已训练轮数, 当前误差, 当前步长);
	if (梯度下降日志文件)
	{
		写入梯度下降日志(梯度下降日志文件, 0, 0, true);
		fclose(梯度下降日志文件);
	}
}


void 写入到bin文件(神经网络_t* 网络, char* 文件路径, bool 是否为标准激励函数库)
{
	FILE* 文件 = fopen(文件路径, "wb");
	if (文件 == NULL)
	{
		return;
	}

	uint8_t  版本号[4] = { 0, 1, 0, 0 };

	版本号[0] = (是否为标准激励函数库)? 0 : 0xFF;
	版本号[1] = 1;
	版本号[2] = 0;
	版本号[3] = 0;

	fwrite("CNNW", 1, 4, 文件);
	fwrite(版本号, sizeof(uint8_t), 4, 文件);
	fwrite(&(*网络).层数, sizeof(unsigned int), 1, 文件);
	for (unsigned int i = 0; i < (*网络).层数; i++)
	{
		单层神经网络_t* 单层 = &(*网络).所有层[i];

		const char* 激活函数名称 = 获取激活函数短名((*单层).激励函数);

		unsigned int 名称长度 = (unsigned int)strlen(激活函数名称);
		unsigned int 矩阵长度 = ((*单层).传入参数个数 + 1) * (*单层).传出参数个数;

		fwrite(&(*单层).传入参数个数, sizeof(unsigned int), 1, 文件);
		fwrite(&(*单层).传出参数个数, sizeof(unsigned int), 1, 文件);
		fwrite(&名称长度, sizeof(unsigned int), 1, 文件);
		fwrite(激活函数名称, 1, 名称长度, 文件);
		fwrite(&矩阵长度, sizeof(unsigned int), 1, 文件);
		fwrite((*单层).变换矩阵, sizeof(double), 矩阵长度, 文件);
	}
	fclose(文件);
}

void 写入到txt文件(神经网络_t* 网络, char* 文件路径)
{
	FILE* 文件 = fopen(文件路径, "w");
	if (文件 == NULL)
	{
		return;
	}

	for (unsigned int i = 0; i < (*网络).层数; i++)
	{
		单层神经网络_t* 单层 = &(*网络).所有层[i];
		unsigned int 行长度 = (*单层).传入参数个数 + 1;
		fprintf(文件, "layer %u:\n", i);
		fprintf(文件, "    in: %u\n", (*单层).传入参数个数);
		fprintf(文件, "    out: %u\n", (*单层).传出参数个数);
		fprintf(文件, "    w:\n");
		for (unsigned int 行 = 0; 行 < (*单层).传出参数个数; 行++)
		{
			fprintf(文件, "        ");
			for (unsigned int 列 = 0; 列 < 行长度; 列++)
			{
				fprintf(文件, "%10.3e", (*单层).变换矩阵[行 * 行长度 + 列]);
				if (列 + 1 < 行长度)
				{
					fprintf(文件, " ");
				}
			}
			fprintf(文件, "\n");
		}
		fprintf(文件, "    actvate: %s\n", 获取激活函数名称((*单层).激励函数));
		fprintf(文件, "\n");
	}

	fclose(文件);
}

void 销毁神经网络(神经网络_t* 网络)
{
	if (网络==NULL)
	{
		return;
	}
	free((char*)网络);
}

static unsigned int 计算矩阵池元素个数(unsigned int* 各层参数个数, unsigned int 层数)
{
	unsigned int 结果 = 0;
	for (unsigned int i = 0; i < 层数; i++)
	{
		结果 += (各层参数个数[i] + 1) * 各层参数个数[i + 1];
	}
	return 结果;
}

static unsigned int 计算中间缓存池元素个数(unsigned int* 各层参数个数, unsigned int 层数)
{
	unsigned int 结果 = 0;
	for (unsigned int i = 0; i < 层数; i++)
	{
		结果 += 2 * 各层参数个数[i + 1];
	}
	return 结果;
}

static size_t 按double对齐(size_t 大小)
{
	size_t 对齐 = sizeof(double);
	return ((大小 + 对齐 - 1) / 对齐) * 对齐;
}

static size_t 按指针对齐(size_t 大小)
{
	size_t 对齐 = sizeof(void*);
	return ((大小 + 对齐 - 1) / 对齐) * 对齐;
}



//  // 该函数仅用于训练，参数微调后对对应项目传播
//  // 修改一个矩阵列的参数[i][j]，用一个向量去乘它，结果向量只有第[i]个元素会变化
//  static double 单层网络_单行线性传播(单层神经网络_t* 单层网络, unsigned int 行号)
//  {
//  	double 计算结果 = 0;
//  	double* 行 = &((*单层网络).变换矩阵[行号 * ((*单层网络).传入参数个数 + 1)]);
//  	for (unsigned int j = 0; j < (*单层网络).传入参数个数; j++)
//  	{
//  		计算结果 += (行[j] * (*单层网络).该层传入数值[j]);
//  	}
//  	计算结果 += 行[(*单层网络).传入参数个数];
//  	return 计算结果;
//  }

//  // 没想好这个功能怎么做
//  // 当向量只有第[j]个元素变化时，乘以一个不变的矩阵，只需计算[:][j]元素计算出来便可得到变化后的矩阵
//  static void 单层网络_单列线性传播(double* 计算结果,单层神经网络_t* 单层网络, unsigned int 列号)
//  {
//  	计算结果[列号] = 0;
//  	memcpy(计算结果, (*单层网络).该层线性数值, (*单层网络).传出参数个数 * sizeof(double));
//  	for (unsigned int i = 0; i < (*单层网络).传出参数个数; i++)
//  	{
//  		(*单层网络).该层线性数值[i] = 0;
//  		for (unsigned int j = 0; j < (*单层网络).传入参数个数; j++)
//  		{
//  			(*单层网络).该层线性数值[i] += ((*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + j] * (*单层网络).传入变量值[j]);
//  		}
//  		(*单层网络).该层线性数值[i] += (*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + (*单层网络).传入参数个数];
//  	}
//  	(*单层网络).该层线性数值[行号] += (*单层网络).变换矩阵[行号 * ((*单层网络).传入参数个数 + 1) + (*单层网络).传入参数个数];
//  }