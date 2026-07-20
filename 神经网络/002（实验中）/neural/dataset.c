#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataset.h"

#define printDoubleArray(_aRrAy,length)               {printf(#_aRrAy"：[%i]\n[ ", length);for(size_t __CoUnTiNg=0;__CoUnTiNg<length;__CoUnTiNg++){printf("%g, ",(double)_aRrAy[__CoUnTiNg]);};puts("]");}

数据集_t* 数据集_新建(unsigned int 自变量单组长度, unsigned int 函数值单组长度, unsigned int 结果集数量)
{
	unsigned int 自变量总长度 = 结果集数量 * 自变量单组长度;
	unsigned int 函数值总长度 = 结果集数量 * 函数值单组长度;
	char* 对象内存池 = (char*)malloc(sizeof(数据集_t) + (自变量总长度 + 函数值总长度) * sizeof(double));
	if (对象内存池 == NULL)
	{
		return NULL;
	}
	数据集_t* 数据集         = (数据集_t*)对象内存池;
	(*数据集).结果集数量     = 结果集数量;
	(*数据集).自变量单组长度 = 自变量单组长度;
	(*数据集).函数值单组长度 = 函数值单组长度;
	(*数据集).自变量数据集   = (double*)(对象内存池 + sizeof(数据集_t));
	(*数据集).函数值数据集   = (double*)(对象内存池 + sizeof(数据集_t) + 自变量总长度 * sizeof(double));
	return 数据集;
}

double* 数据集_获取一组自变量参数(数据集_t* 数据集, unsigned int index)
{
	return (*数据集).自变量数据集 + (index * (*数据集).自变量单组长度);
}

double* 数据集_获取一组函数值参数(数据集_t* 数据集, unsigned int index)
{
	return (*数据集).函数值数据集 + (index * (*数据集).函数值单组长度);
}

char* 数据集_str(char* 缓冲, 数据集_t* 数据集)
{
	char* _填充 = 缓冲;
	_填充 += sprintf(_填充, "数据集_t(%u * ([%u] -> [%u]))",
		(*数据集).结果集数量,
		(*数据集).自变量单组长度,
		(*数据集).函数值单组长度
	);
	return 缓冲;
}
char* 数据集_repr(char* 缓冲, 数据集_t* 数据集)
{
	char* _填充 = 缓冲;
	_填充 += sprintf(_填充, "数据集_t(%u * ([%u] -> [%u]))\n[\n",
		(*数据集).结果集数量,
		(*数据集).自变量单组长度,
		(*数据集).函数值单组长度
	);
	for (size_t i = 0; i < (*数据集).结果集数量; i++)
	{
		sprintf(_填充, "[ ");
		for (size_t i = 0; i < (*数据集).自变量单组长度; ++i)
		{
			printf("%g, ", (double)(*数据集).自变量数据集[i]);
		};
		sprintf(_填充, "], [ ");
		for (size_t i = 0; i < (*数据集).函数值单组长度; ++i)
		{
			printf("%g, ", (double)(*数据集).函数值数据集[i]);
		};
		sprintf(_填充, "], ");
	}
	_填充 += sprintf(_填充, "]\n");
	return 缓冲;

}


void 数据集_销毁(数据集_t* 数据集)
{
	free((char*)数据集);
}
