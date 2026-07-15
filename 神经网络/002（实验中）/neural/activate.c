#include <stdlib.h>
#include <math.h>

#define max2(x,y) (((x)>(y))?(x):(y))
void 绝对值激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = (参数[i]>=0)?参数[i]:-参数[i];
	}
}
void 平方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 参数[i] * 参数[i];
	}
}
void 三次方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 参数[i] * 参数[i] * 参数[i];
	}
}
void 指数激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = exp(参数[i]);
	}
}
void 指数减一激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = exp(参数[i])-1;
	}
}
// sigmoid(x) = 1/(1+exp(-x))
void sigmoid激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 1.0/(1+exp(参数[i]));
	}
}
// tanh(x) = (exp(x)-exp(-x))/(exp(x)+exp(-x))
void tanh激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = tanh(参数[i]);
	}
}
// ReLU(x) = max(x,0)
void relu激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = max2(参数[i], 0);
	}
}
void leakyrelu激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数)
{
	double leak参数 = *((double*)状态参数);
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = max2(参数[i], leak参数 * 参数[i]);
	}
}
// Softmax归一化函数：{a_n} -> { exp(a_n) / sum(exp(a_n)) }
void softmax归一化函数(double* 结果, double* 参数, unsigned int 参数个数, void* 状态参数)
{
	double exp和 = 0;
	double* exps = (double*)malloc(参数个数 * sizeof(double));
	for(unsigned int i = 0; i < 参数个数; i++)
	{
		exps[i] = exp(参数[i]);
		exp和 += exps[i];
	}
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = exps[i] / exp和;
	}
}