#ifndef __MATH_BASE_H__
#define __MATH_BASE_H__

typedef double (*计算函数_t)(double* /* 矩阵 */, unsigned int /* 维数 */, void* /* 状态参数 */);

double 获取向量长度    (double* n维向量, unsigned int 维数);
double 获取向量长度平方(double* n维向量, unsigned int 维数);
double 获取两点距离    (double* n维点1, double* n维点2, unsigned int 维数);
double 获取两点距离平方(double* n维点1, double* n维点2, unsigned int 维数);

void   单位向量化(
	double* 结果,
	double* n维向量,
	unsigned int 维数
);
void   单位向量化_本地修改(
	double* n维向量,
	unsigned int 维数
);
void   固定长度方向化(
	double* 结果,
	double* n维向量,
	unsigned int 维数,
	double 长度
);
void   固定长度方向化_本地修改(
	double* n维向量,
	unsigned int 维数,
	double 长度
);
void   获取梯度(
	double* 结果,
	double* 坐标点,
	计算函数_t 计算函数,
	void* 状态参数,
	unsigned int 维数,
	double delta
);
void 获取凸凹性(
	double* 结果,
	double* 坐标点,
	计算函数_t 计算函数,
	void* 状态参数,
	unsigned int 维数,
	double delta
);

/* 返回值：最后一次计算的步长 */
double 梯度下降求解(
	double* 结果,
	double* 起始点,
	unsigned int 维数,
	计算函数_t 计算函数,
	void* 状态参数,
	double 初始步长,
	unsigned int 循环次数
);
double 梯度下降求解_本地修改(
	double* 结果,
	unsigned int 维数,
	计算函数_t 计算函数,
	void* 状态参数,
	double 初始步长,
	unsigned int 循环次数
);

#endif
