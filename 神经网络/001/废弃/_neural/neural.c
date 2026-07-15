#include <stdio.h>
#include <stdlib.h>
#include "neural.h"

Neuro_Net* Neuro_Net_new(unsigned int nlayers, unsigned int *args_nums, ActivateFunction_t *activateFunctions)
{
	Neuro_Net* net = (Neuro_Net*)malloc(sizeof(Neuro_Net));
	if (net==NULL)
	{
		return NULL;
	}

	(*net).nlayers   = nlayers;
	(*net).args_nums = (unsigned int*)malloc((nlayers+1)*sizeof(unsigned int));
	(*net).layers    = (Neuro_Layer*)malloc(nlayers * sizeof(Neuro_Layer));
	unsigned int matrix_total_num = 0;
	unsigned int args_total_num = 0;
	for (unsigned int i = 0; i < nlayers; i++)
	{
		(*net).args_nums[i]           = args_nums[i];
		(*net).layers[i].in_num       = args_nums[i];
		(*net).layers[i].out_num      = args_nums[i+1];
		(*net).layers[i].actfunc      = activateFunctions[i];
		matrix_total_num += args_nums[i] * args_nums[i+1];
		args_total_num += args_nums[i+1];
	}
	(*net).args_matrix_num = matrix_total_num;
	double* buffer_pool = (double*)malloc((matrix_total_num + args_total_num) * sizeof(double));
	(*net).buffer_pool = buffer_pool;
	double* args_pool = buffer_pool+matrix_total_num;

	for (unsigned int i = 0; i < nlayers; i++)
	{
		(*net).layers[i].trans_matrix = buffer_pool + (*net).layers[i].trans_matrix; // 当前数值为偏移量
		(*net).layers[i].args         = args_pool   + (*net).layers[i].args; // 当前数值为偏移量
	}
	return net;
}

void 推理计算(Neuro_Net* net, double* 一级参数)
{

}