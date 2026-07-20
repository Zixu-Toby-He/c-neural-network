#ifndef __ACTIVATE_H__
#define __ACTIVATE_H__

#include <stdbool.h>

typedef void (*激励函数_t)(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);

// x
void 线性激励函数     (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// abs(x)
void 绝对值激励函数   (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// x*x
void 平方激励函数     (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// x*x*x
void 三次方激励函数   (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// exp(x)
void 指数激励函数     (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// exp(x)-1
void 指数减一激励函数 (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// sigmoid(x) = 1/(1+exp(-x))
void sigmoid激励函数  (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// tanh(x) = (exp(x)-exp(-x))/(exp(x)+exp(-x))
void tanh激励函数     (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// ReLU(x) = max(x,0)
void relu激励函数     (double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);
// LeakyReLU(x) = (x>0)? x : x*系数
// 系数 = *(double*)调节参数
void leakyrelu激励函数(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);

// Softmax归一化函数：{a_n} -> { exp(a_n) / sum(exp(a_n)) }
void softmax归一化函数(double* 结果, double* 参数, unsigned int 参数个数, void* 调节参数);

//  激励函数的保存与输出
bool        判断是否有非标激励函数(void);
void        新引入激励函数(激励函数_t 激励函数, char* 函数名称, char* 短名);
const char* 获取激活函数名称(激励函数_t 激励函数);
const char* 获取激活函数短名(激励函数_t 激励函数);


#endif
