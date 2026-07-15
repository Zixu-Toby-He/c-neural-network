#ifndef __OTHER_H__
#define __OTHER_H__

#define 训练集长度 200

#define 验证集长度 13

// 仅仅填充，不做创建
void 填充训练集(数据集_t* 训练集);

// 仅仅填充，不做创建
void 填充验证集(数据集_t* 训练集);

// 将概率序列中最大概率的索引提取出来
int 翻译概率到象限(double 推理结果[5]);



#endif