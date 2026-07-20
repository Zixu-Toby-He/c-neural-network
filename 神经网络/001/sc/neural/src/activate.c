#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "activate.h"

#define max2(x,y) (((x)>(y))?(x):(y))

void 线性激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 参数[i];
	}
}

void 绝对值激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = (参数[i] >= 0)? 参数[i] : -参数[i];
	}
}
void 平方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 参数[i] * 参数[i];
	}
}
void 三次方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 参数[i] * 参数[i] * 参数[i];
	}
}
void 指数激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = exp(参数[i]);
	}
}
void 指数减一激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = exp(参数[i]) - 1;
	}
}
// sigmoid(x) = 1/(1+exp(-x))
void sigmoid激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = 1.0/(1 + exp(-参数[i]));
	}
}
// tanh(x) = (exp(x)-exp(-x))/(exp(x)+exp(-x))
void tanh激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = tanh(参数[i]);
	}
}
// ReLU(x) = max(x,0)
void relu激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = max2(参数[i], 0);
	}
}

// LeakyReLU(x) = max(x, x * 系数)
void leakyrelu激励函数(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数)
{
	double leak参数 = *((double*)调节参数);
	double temp;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		temp = leak参数 * 参数[i];
		结果[i] = max2(参数[i], temp);
	}
}

// ELU(x) = (x>0)?x:(e^x-1)
void elu激励函数(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数)
{
	(void)调节参数;
	for (unsigned int i = 0; i < 参数个数; i++)
	{
		结果[i] = (参数[i] > 0)?(参数[i]):(exp(参数[i]) - 1);
	}
}
// Softmax归一化函数：{a_n} -> { exp(a_n) / sum(exp(a_n)) }
void softmax归一化函数(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数)
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
	(void)调节参数;
	free(exps);
}

#define 最大个数 32

static struct
{
	激励函数_t 函数指针;
	char* 函数名称;
	char* 短名;
}
激励函数集合[最大个数] = {
	{ 线性激励函数,      "线性激励函数",      "l"    },
	{ 平方激励函数,      "平方激励函数",      "s"    },
	{ 三次方激励函数,    "三次方激励函数",    "t"    },
	{ 绝对值激励函数,    "绝对值激励函数",    "a"    },
	{ relu激励函数,      "relu激励函数",      "relu" },
	{ leakyrelu激励函数, "leakyrelu激励函数", "lkrl" },
	{ sigmoid激励函数,   "sigmoid激励函数",   "sgmd" },
	{ tanh激励函数,      "tanh激励函数",      "tanh" },
	{ 指数激励函数,      "指数激励函数",      "exp0" },
	{ 指数减一激励函数,  "指数减一激励函数",  "exp1" },
	{ softmax归一化函数, "softmax归一化函数", "sfmx" },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
	{ NULL,              NULL               , NULL   },
};
static bool 标准激励函数库 = true;

void 新引入激励函数(激励函数_t 激励函数, char* 函数名称, char* 短名)
{
	uint8_t 序号 = 0;
	for(uint8_t _序号 = 0; _序号 < 最大个数; _序号++)
	{
		if (激励函数集合[_序号].函数名称 == NULL)
		{
			序号 = _序号;
			break;
		}
	}
	标准激励函数库 = false;
	激励函数集合[序号].函数指针 = 激励函数;
	激励函数集合[序号].函数名称 = 函数名称;
	激励函数集合[序号].短名     = 短名;
}

bool 判断是否有非标激励函数(void)
{
	return !标准激励函数库;
}

const char* 获取激活函数短名(激励函数_t 激励函数)
{
	for (uint8_t 序号 = 0; 序号 < 最大个数; 序号++)
	{
		if (激励函数集合[序号].函数指针 == NULL)
		{
			break;
		}
		if (激励函数集合[序号].函数指针 == 激励函数)
		{
			return 激励函数集合[序号].短名;
		}
	}
	return "unknown";
}

const char* 获取激活函数名称(激励函数_t 激励函数)
{
	for (uint8_t 序号 = 0; 序号 < 最大个数; 序号++)
	{
		if (激励函数集合[序号].函数指针 == NULL)
		{
			break;
		}
		if (激励函数集合[序号].函数指针 == 激励函数)
		{
			return 激励函数集合[序号].函数名称;
		}
	}
	return "unknown";
}
