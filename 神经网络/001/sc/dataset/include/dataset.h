#ifndef __DATASET_H__
#define __DATASET_H__

typedef struct 数据集_t
{
	unsigned int 结果集数量;
	unsigned int 自变量单组长度;
	unsigned int 函数值单组长度;
	double*      自变量数据集;
	double*      函数值数据集;
}
数据集_t;
数据集_t* 数据集_新建(unsigned int 自变量单组长度, unsigned int 函数值单组长度, unsigned int 结果集数量);
double*   数据集_获取一组自变量参数(数据集_t* 数据集, unsigned int index);
double*   数据集_获取一组函数值参数(数据集_t* 数据集, unsigned int index);
char*     数据集_str(char* 缓冲, 数据集_t*);
char*     数据集_repr(char* 缓冲, 数据集_t*);
void      数据集_销毁(数据集_t*);

#endif
