#include <stdio.h>
#include <stdlib.h>

#include "neural/math base.h"

double f(double* arr,unsigned int n, void* other)
{
	return arr[0]*arr[0] + arr[1] * arr[1];
}

int main()
{
	double 数值[2] = {200,200};
	double* 结果 = 数值;
	梯度下降求解_本地修改(
		结果,
		2,
		f,
		NULL,
		10,
		200);
	printf("(%g, %g)\n", 数值[0], 数值[1]);
	return 0;
}