#ifndef __ACTIVATE_H__
#define __ACTIVATE_H__

typedef void (*激励函数_t)(
	double* 结果,
	double* 参数,
	unsigned int 参数个数,
	void* 状态参数
);

void 绝对值激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

void 平方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

void 三次方激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// exp(x)
void 指数激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// exp(x)-1
void 指数减一激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// sigmoid(x) = 1/(1+exp(-x))
void sigmoid激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// tanh(x) = (exp(x)-exp(-x))/(exp(x)+exp(-x))
void tanh激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// ReLU(x) = max(x,0)
void relu激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// LeakyReLU(x) = (x>0)? x : x*系数
void leakyrelu激励函数(double* 结果,double* 参数, unsigned int 参数个数, void* 状态参数);

// Softmax归一化函数：{a_n} -> { exp(a_n) / sum(exp(a_n)) }
void softmax归一化函数(double* 结果, double* 参数, unsigned int 参数个数, void* 状态参数);

#endif
