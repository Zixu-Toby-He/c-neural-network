#ifndef __NEURAL_H__
#define __NEURAL_H__

typedef void (*ActivateFunction_t)(double* /* 结果 */,double* /* 参数 */, unsigned int /* 参数个数 */);

typedef struct Neuro_Layer
{
	unsigned int in_num;         // 传入参数个数
	unsigned int out_num;        // 传出参数个数
	double* trans_matrix;        // 变换矩阵：维度信息：in_num * out_num
	ActivateFunction_t actfunc;  // 激励函数
	double* args;                // 该层参数值
}
Neuro_Layer;

typedef struct Neuro_Net
{
	unsigned int  nlayers;            // 层数
	unsigned int* args_nums;          // 各层参数个数
	Neuro_Layer*  layers;             // 所有层
	double*       buffer_pool;        // 缓存池
	unsigned int  args_matrix_num;    // 矩阵参数个数
}
Neuro_Net;

Neuro_Net* Neuro_Net_new(unsigned int nlayers, unsigned int* args_nums, ActivateFunction_t* activateFunctions);

#endif
