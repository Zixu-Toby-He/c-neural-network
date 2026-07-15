#ifndef __NEURAL_H__
#define __NEURAL_H__

typedef struct 训练集_t
{
	unsigned int 结果集数量;
	unsigned int 自变量单组长度;
	unsigned int 函数值单组长度;
	double*      自变量数据集;
	double*      函数值数据集;
}
训练集_t;

typedef void (*激励函数_t)(double* /* 结果 */,double* /* 参数 */, unsigned int /* 参数个数 */, void* /* 调节参数 */);

typedef struct 单层神经网络_t
{
	unsigned int 传入参数个数;
	unsigned int 传出参数个数;
	double*      变换矩阵;       // 变换矩阵维度信息：in_num * out_num
	激励函数_t   激励函数;
	double*      该层参数值;
}
单层神经网络_t;

typedef struct 神经网络_t
{
	unsigned int 层数;
	unsigned int* 各层参数个数;
	单层神经网络_t* 所有层;
	double* 缓存池;
	unsigned int 矩阵参数个数;
}
神经网络_t;



神经网络_t* 新建神经网络(unsigned int 层数, unsigned int* 各层参数个数, 激励函数_t* 各层激励函数);

void 推理计算(神经网络_t* 网络,double* 输出结果, double* 输入参数);

void 训练(神经网络_t* 网络, 训练集_t* 训练集, unsigned int 训练轮数);

void 销毁神经网络(神经网络_t* 网络);

#endif
