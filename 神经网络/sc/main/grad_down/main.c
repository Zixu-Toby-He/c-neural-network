#include <stdio.h>
#include <stdlib.h>

#include "math_base.h"

#define 维数 2
#define 初始步长 10.0
#define 循环次数 200

double f(double* arr, unsigned int n, void* other)
{
	(void)n;
	(void)other;
	return (arr[0] - 1) * (arr[0] - 2) + (arr[1] + 5) * (arr[1] - 3);
}

int main()
{
	double 数值[维数] = { 200, 200 };
	double 起点[维数] = { 数值[0], 数值[1] };
	double 理论最优点[维数] = { 1.5, -1.0 };
	double 初始函数值 = f(数值, 维数, NULL);
	double* 结果 = 数值;
	double 最终步长 = 梯度下降求解_本地修改(
		结果,
		维数,
		f,
		NULL,
		初始步长,
		循环次数
	);

	double 最终函数值 = f(结果, 维数, NULL);
	double 与最优点距离平方 = 获取两点距离平方(结果, 理论最优点, 维数);

	puts("gradient descent test");
	puts("---------------------");
	puts("function: f(x, y) = (x - 1)(x - 2) + (y + 5)(y - 3)");
	printf("start:    (% .10g, % .10g), f = %.17g\n", 起点[0], 起点[1], 初始函数值);
	printf("config:   dimension = %u, initial_step = %.10g, iterations = %u\n", 维数, 初始步长, 循环次数);
	printf("expected: (% .10g, % .10g), f = %.17g\n", 理论最优点[0], 理论最优点[1], f(理论最优点, 维数, NULL));
	printf("result:   (% .10g, % .10g), f = %.17g\n", 结果[0], 结果[1], 最终函数值);
	printf("error:    squared_distance_to_expected = %.17g\n", 与最优点距离平方);
	printf("step:     final_step = %.17g\n", 最终步长);
	return 0;
}
