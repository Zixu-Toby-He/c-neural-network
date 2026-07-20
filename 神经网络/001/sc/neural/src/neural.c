#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "neural.h"
#include "math_base.h"



void 单层网络_线性传播(单层神经网络_t* 单层网络, double* 计算结果, double* 输入参数)
{
	for (unsigned int i = 0; i < (*单层网络).传出参数个数; i++)
	{
		计算结果[i] = 0;
		for (unsigned int j = 0; j < (*单层网络).传入参数个数; j++)
		{
			计算结果[i] += ((*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + j] * 输入参数[j]);
		}
		计算结果[i] += (*单层网络).变换矩阵[i * ((*单层网络).传入参数个数 + 1) + (*单层网络).传入参数个数];
	}
}

void 单层网络_传播(单层神经网络_t* 单层网络, double* 输入参数)
{
	double* 中间结果 = (double*)malloc((*单层网络).传出参数个数 * sizeof(double));
	单层网络_线性传播(单层网络, (*单层网络).该层参数值, 输入参数);
	(*单层网络).激励函数(
		中间结果,
		(*单层网络).该层参数值,
		(*单层网络).传出参数个数,
		NULL
	);
	memcpy((*单层网络).该层参数值, 中间结果, (*单层网络).传出参数个数 * sizeof(double));
	free(中间结果);
}

神经网络_t* 新建神经网络(unsigned int 层数, unsigned int *各层参数个数, 激励函数_t *各层激励函数)
{
	神经网络_t* 网络 = (神经网络_t*)malloc(sizeof(神经网络_t));
	if (网络==NULL)
	{
		return NULL;
	}

	(*网络).层数 = 层数;
	(*网络).各层参数个数 = (unsigned int*)malloc((层数 + 1)*sizeof(unsigned int));
	(*网络).所有层 = (单层神经网络_t*)malloc(层数 * sizeof(单层神经网络_t));
	unsigned int 矩阵缓存个数 = 0;
	unsigned int 参数缓存个数 = 0;
	for (unsigned int i = 0; i < 层数; i++)
	{
		(*网络).各层参数个数[i]        = 各层参数个数[i];
		(*网络).所有层[i].传入参数个数 = 各层参数个数[i];
		(*网络).所有层[i].传出参数个数 = 各层参数个数[i+1];
		(*网络).所有层[i].激励函数     = 各层激励函数[i];
		//(*网络).所有层[i].变换矩阵     = (double*)矩阵缓存个数; // 当前数值为偏移量
		//(*网络).所有层[i].该层参数值   = (double*)参数缓存个数; // 当前数值为偏移量
		矩阵缓存个数 += (各层参数个数[i] + 1) * 各层参数个数[i+1];
		参数缓存个数 += 各层参数个数[i+1];
	}
	(*网络).各层参数个数[层数] = 各层参数个数[层数];
	(*网络).矩阵参数个数 = 矩阵缓存个数;
	double* 缓存池 = (double*)malloc((矩阵缓存个数 + 参数缓存个数) * sizeof(double));
	(*网络).缓存池 = 缓存池;
	double* 参数缓存池 = 缓存池 + 矩阵缓存个数;
	double* 当前矩阵缓存位置 = 缓存池;
	double* 当前参数缓存位置 = 参数缓存池;

	for (unsigned int i = 0; i < 层数; i++)
	{
		(*网络).所有层[i].变换矩阵   = 当前矩阵缓存位置; // 当前数值为偏移量
		(*网络).所有层[i].该层参数值 = 当前参数缓存位置; // 当前数值为偏移量
		当前矩阵缓存位置 += (((*网络).所有层[i].传入参数个数 + 1) * (*网络).所有层[i].传出参数个数);
		当前参数缓存位置 += (*网络).所有层[i].传出参数个数;
	}
	return 网络;
}

void 推理计算(神经网络_t* 网络, double* 输出结果, double* 输入参数)
{
	double* 当前层参数 = 输入参数;
	for (unsigned int i = 0; i < (*网络).层数; i++)
	{
		单层网络_传播(&(*网络).所有层[i], 当前层参数);
		当前层参数 = (*网络).所有层[i].该层参数值;
	}
	if(输出结果 != (*网络).所有层[(*网络).层数-1].该层参数值)
	{
		memcpy(输出结果, (*网络).所有层[(*网络).层数-1].该层参数值, (*网络).所有层[(*网络).层数-1].传出参数个数 * sizeof(double));
	}
}

double 评价函数(double* 矩阵参数, unsigned int 矩阵参数个数, void* 神经网络与训练集)
{
	//  消除警告
	(void)矩阵参数;
	(void)矩阵参数个数;
	/* 该函数不会进行匹配校验 */
	神经网络_t* 神经网络 = (神经网络_t*)(((void**)神经网络与训练集)[0]);
	训练集_t* 训练集     = (训练集_t*)  (((void**)神经网络与训练集)[1]);
	double* 推理结果 = (*神经网络).所有层[(*神经网络).层数-1].该层参数值;
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
	free((*网络).各层参数个数);
	free((*网络).所有层);
	free((*网络).缓存池);
	free(网络);
}
