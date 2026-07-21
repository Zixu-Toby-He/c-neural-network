#ifndef OTHER_H
#define OTHER_H

#include "dataset.h"

#define 坐标下限 (-128)
#define 坐标上限 (128)
#define 训练步长 20
#define 推理步长 1

#define 训练格点数量 (((坐标上限) - (坐标下限)) / 训练步长 + 1)
#define 推理格点数量 (((坐标上限) - (坐标下限)) / 推理步长 + 1)

#define 训练集长度 (训练格点数量 * 训练格点数量)
#define 推理集长度 (推理格点数量 * 推理格点数量)

void 填充训练集(数据集_t* 训练集);
void 填充推理集(数据集_t* 推理集);

#endif
