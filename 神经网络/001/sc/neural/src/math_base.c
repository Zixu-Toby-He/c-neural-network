#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "math_base.h"

#define max2(a,b) (((a)>(b))?(a):(b))
#define 试探次数上限 128

double 获取向量长度(double* n维向量, unsigned int 维数)
{
	double 长度平方 = 0;
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		长度平方 += n维向量[counting]*n维向量[counting];
	}
	return sqrt(长度平方);
}

double 获取向量长度平方(double* n维向量, unsigned int 维数)
{
	double 长度平方 = 0;
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		长度平方 += n维向量[counting]*n维向量[counting];
	}
	return 长度平方;
}

double 获取两点距离(double* n维点1, double* n维点2, unsigned int 维数)
{
	double 长度平方 = 0;
	double 投影距离;
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		投影距离 = n维点1[counting] - n维点2[counting];
		长度平方 += 投影距离*投影距离;
	}
	return sqrt(长度平方);
}

double 获取两点距离平方(double* n维点1, double* n维点2, unsigned int 维数)
{
	double 长度平方 = 0;
	double 投影距离;
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		投影距离 = n维点1[counting] - n维点2[counting];
		长度平方 += 投影距离*投影距离;
	}
	return 长度平方;
}

void 单位向量化(double* 结果, double* n维向量, unsigned int 维数)
{
	double 向量长度 = 获取向量长度(n维向量, 维数);
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		结果[counting] = n维向量[counting] / 向量长度;
	}
}

void 单位向量化_本地修改(double* n维向量, unsigned int 维数)
{
	double 向量长度 = 获取向量长度(n维向量, 维数);
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		n维向量[counting] /= 向量长度;
	}
}

void 固定长度方向化(double* 结果, double* n维向量, unsigned int 维数, double 长度)
{
	double 向量长度 = 获取向量长度(n维向量, 维数);
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		结果[counting] = 长度 * n维向量[counting] / 向量长度;
	}
}
void 固定长度方向化_本地修改(double* n维向量, unsigned int 维数, double 长度)
{
	double 向量长度 = 获取向量长度(n维向量, 维数);
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		n维向量[counting] *= (长度 / 向量长度);
	}
}

void 获取梯度(double* 结果, double* 坐标点, 计算函数_t 计算函数, void* 状态参数, unsigned int 维数, double delta)
{
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		double 坐标点原始数据 = 坐标点[counting];
		坐标点[counting] = 坐标点原始数据 + delta;
		double plus  = 计算函数(坐标点, 维数, 状态参数);
		坐标点[counting] = 坐标点原始数据 - delta;
		double minus = 计算函数(坐标点, 维数, 状态参数);
		坐标点[counting] = 坐标点原始数据;
		结果[counting] = (plus - minus) / (2 * delta);
	}
}

void 获取凸凹性(double* 结果, double* 坐标点, 计算函数_t 计算函数, void* 状态参数, unsigned int 维数, double delta)
{
	double zero = 计算函数(坐标点, 维数, 状态参数);
	for (unsigned int counting = 0; counting < 维数; counting++)
	{
		double 坐标点原始数据 = 坐标点[counting];
		坐标点[counting] = 坐标点原始数据 + delta;
		double plus  = 计算函数(坐标点, 维数, 状态参数);
		坐标点[counting] = 坐标点原始数据 - delta;
		double minus = 计算函数(坐标点, 维数, 状态参数);
		坐标点[counting] = 坐标点原始数据;
		结果[counting] = (plus + minus - 2 * zero) / (2 * delta * delta);
	}
}

double 梯度下降求解(double* 结果, double* 起始点, unsigned int 维数, 计算函数_t 计算函数, void* 状态参数, double 步长, unsigned int 循环次数)
{
	double* 梯度信息    = (double*)malloc(维数*sizeof(double));
	double* 下一步参数  = (double*)malloc(维数*sizeof(double));

	double 上一步函数值;
	double delta = max2(步长, 1.0)/1024;
	memcpy(下一步参数, 起始点, 维数*sizeof(double));
	for (unsigned int counting = 0; counting < 循环次数; counting++)
	{
		上一步函数值 = 计算函数(下一步参数, 维数, 状态参数);
		获取梯度(梯度信息, 下一步参数, 计算函数, 状态参数, 维数, delta);
		double 梯度大小 = 获取向量长度(梯度信息, 维数);
		if (梯度大小 == 0)
		{
			memcpy(结果, 下一步参数, 维数*sizeof(double));
			free(梯度信息);
			free(下一步参数);
			return 0;
		}
		else
		{
			uint8_t 试探次数 = 0;
			固定长度方向化_本地修改(梯度信息, 维数, 步长);
			for (unsigned int counting_1 = 0; counting_1 < 维数; counting_1++)
			{
				下一步参数[counting_1] -= 梯度信息[counting_1];
			}
			for (试探次数 = 0; 试探次数 < 试探次数上限; 试探次数++)
			{
				if (计算函数(下一步参数, 维数, 状态参数) >= 上一步函数值)
				{
					步长  /= 2;
					delta /= 2;
					固定长度方向化_本地修改(梯度信息, 维数, 步长);
					for (unsigned int counting_1 = 0; counting_1 < 维数; counting_1++)
					{
						下一步参数[counting_1] += 梯度信息[counting_1];
					}
				}
				else
				{
					break;
				}
			}
			if (试探次数 == 试探次数上限)
			{
				break;
			}
		}	
	}
	memcpy(结果, 下一步参数, 维数*sizeof(double));
	free(梯度信息);
	free(下一步参数);
	return 步长;
}

double 梯度下降求解_本地修改(double* 结果, unsigned int 维数, 计算函数_t 计算函数, void* 状态参数, double 步长, unsigned int 循环次数)
{
	double* 梯度信息    = (double*)malloc(维数*sizeof(double));
	double 上一步函数值;
	double delta = max2(步长, 1.0)/1024;
	for (unsigned int counting = 0; counting < 循环次数; counting++)
	{
		上一步函数值 = 计算函数(结果, 维数, 状态参数);
		获取梯度(梯度信息, 结果, 计算函数, 状态参数, 维数, delta);
		double 梯度大小 = 获取向量长度(梯度信息, 维数);
		if (梯度大小 == 0)
		{
			free(梯度信息);
			return 0;
		}
		else
		{
			uint8_t 试探次数 = 0;
			固定长度方向化_本地修改(梯度信息, 维数, 步长);
			for (unsigned int counting_1 = 0; counting_1 < 维数; counting_1++)
			{
				结果[counting_1] -= 梯度信息[counting_1];
			}
			for (试探次数 = 0; 试探次数 < 试探次数上限; 试探次数++)
			{
				if (计算函数(结果, 维数, 状态参数) >= 上一步函数值)
				{
					步长  /= 2;
					delta /= 2;
					固定长度方向化_本地修改(梯度信息, 维数, 步长);
					for (unsigned int counting_1 = 0; counting_1 < 维数; counting_1++)
					{
						结果[counting_1] += 梯度信息[counting_1];
					}
				}
				else
				{
					break;
				}
			}
			if (试探次数 == 试探次数上限)
			{
				break;
			}
		}	
	}
	free(梯度信息);
	return 步长;
}
