#ifndef __NEURAL_H__
#define __NEURAL_H__


#include "activate.h"
#include "dataset.h"



typedef struct 单层神经网络_t
{
	unsigned int 传入参数个数;
	unsigned int 传出参数个数;
	double*      变换矩阵;       // 变换矩阵维度信息：(in_num + 1) * out_num
	激励函数_t   激励函数;
	double*      该层传入数值;   // 保存传入参数的值（初始层不需要）
	double*      该层线性数值;   // 保存线性计算后的结果
	double*      该层激励数值;   // 保存激励计算后的结果（末尾层不需要）
//	char         激励函数参数[64];
}
单层神经网络_t;

typedef struct 神经网络_t
{
	unsigned int    层数;
	unsigned int*   各层参数个数;
	单层神经网络_t* 所有层;
	double*         参数池;
	unsigned int    矩阵参数个数;
	unsigned int    总空间大小;    // 单位：字节
}
神经网络_t;



神经网络_t* 新建神经网络(unsigned int 层数, unsigned int* 各层参数个数, 激励函数_t* 各层激励函数);

void 神经网络_推理计算(神经网络_t* 网络, double* 输出结果, double* 输入参数);

void 神经网络_训练(神经网络_t* 网络, 数据集_t* 训练集, unsigned int 训练轮数);

void 神经网络_销毁(神经网络_t* 网络);

#endif
