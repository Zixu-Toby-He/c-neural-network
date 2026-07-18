#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEP '\\'
#define mkdir_one(path) _mkdir(path)
#else
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEP '/'
#define mkdir_one(path) mkdir(path, 0755)
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include "activate.h"
#include "neural.h"
#include "dataset.h"

#include "other.h"

static int get_exe_dir(char* out_dir, size_t out_dir_size)
{
	if (out_dir == NULL || out_dir_size == 0)
	{
		return 0;
	}

#ifdef _WIN32
	DWORD len = GetModuleFileNameA(NULL, out_dir, (DWORD)out_dir_size);
	if (len == 0 || len >= out_dir_size)
	{
		return 0;
	}
#else
	ssize_t len = readlink("/proc/self/exe", out_dir, out_dir_size - 1);
	if (len <= 0 || (size_t)len >= out_dir_size)
	{
		return 0;
	}
	out_dir[len] = '\0';
#endif

	char* last_slash = strrchr(out_dir, '/');
	char* last_backslash = strrchr(out_dir, '\\');
	char* last_sep = last_slash;
	if (last_backslash != NULL && (last_sep == NULL || last_backslash > last_sep))
	{
		last_sep = last_backslash;
	}
	if (last_sep == NULL)
	{
		return 0;
	}
	*last_sep = '\0';
	return 1;
}

static int join_path(char* 结果, size_t 结果长度, const char* 目录, const char* 文件名)
{
	return snprintf(结果, 结果长度, "%s%c%s", 目录, PATH_SEP, 文件名) < (int)结果长度;
}

static int 准备输出目录(char* 输出目录, size_t 输出目录长度)
{
	char exe目录[PATH_MAX];
	if (!get_exe_dir(exe目录, sizeof(exe目录)))
	{
		return 0;
	}
	if (!join_path(输出目录, 输出目录长度, exe目录, "output"))
	{
		return 0;
	}
	mkdir_one(输出目录);
	return 1;
}

static void 随机初始化权重(神经网络_t* 神经网络)
{
	srand(56);
	for (unsigned int i = 0; i < (*神经网络).矩阵参数个数; i++)
	{
		(*神经网络).缓存池[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
	}
}

static int 写出权重文本(神经网络_t* 神经网络, const char* 文件路径)
{
	FILE* 文件 = fopen(文件路径, "w");
	if (文件 == NULL)
	{
		return 0;
	}

	for (unsigned int i = 0; i < (*神经网络).层数; i++)
	{
		单层神经网络_t* 单层 = &(*神经网络).所有层[i];
		unsigned int 行长度 = (*单层).传入参数个数 + 1;
		fprintf(文件, "layer %u:\n", i);
		fprintf(文件, "    in: %u\n", (*单层).传入参数个数);
		fprintf(文件, "    out: %u\n", (*单层).传出参数个数);
		fprintf(文件, "    w:\n");
		for (unsigned int 行 = 0; 行 < (*单层).传出参数个数; 行++)
		{
			fprintf(文件, "        ");
			for (unsigned int 列 = 0; 列 < 行长度; 列++)
			{
				fprintf(文件, "%10.3e", (*单层).变换矩阵[行 * 行长度 + 列]);
				if (列 + 1 < 行长度)
				{
					fprintf(文件, " ");
				}
			}
			fprintf(文件, "\n");
		}
		fprintf(文件, "    actvate: %s\n", 获取激活函数名称((*单层).激励函数));
		fprintf(文件, "\n");
	}

	fclose(文件);
	return 1;
}

static int 写出推理结果(神经网络_t* 神经网络, 数据集_t* 推理集, const char* 文件路径)
{
	FILE* 文件 = fopen(文件路径, "w");
	if (文件 == NULL)
	{
		return 0;
	}

	fprintf(文件, "x,y_aim,y_prd,diff\n");
	for (size_t i = 0; i < (*推理集).结果集数量; i++)
	{
		double 推理结果[1];
		double* 自变量 = 数据集_获取一组自变量参数(推理集, i);
		double* 函数值 = 数据集_获取一组函数值参数(推理集, i);
		推理计算(神经网络, 推理结果, 自变量);
		fprintf(文件, "%.10g,%.10g,%.10g,%.10g\n", 自变量[0], 函数值[0], 推理结果[0], 推理结果[0] - 函数值[0]);
	}

	fclose(文件);
	return 1;
}

int main()
{
#define _层数 3
	char 输出目录[PATH_MAX];
	char 权重文本路径[PATH_MAX];
	char 权重原始数据路径[PATH_MAX];
	char 推理结果路径[PATH_MAX];
	char 训练日志路径[PATH_MAX];

	puts("创建并整理数据集");
	数据集_t* 训练集 = 数据集_新建(1, 1, 训练集长度);
	数据集_t* 推理集 = 数据集_新建(1, 1, 推理集长度);
	填充训练集(训练集);
	填充推理集(推理集);

	unsigned int 各层参数个数[_层数 + 1] = { 1, 5, 5, 1 };
	激励函数_t 各层激励函数[_层数] = { relu激励函数, relu激励函数, 指数激励函数 };
	puts("创建神经网络");
	神经网络_t* 神经网络 = 新建神经网络(_层数, 各层参数个数, 各层激励函数);
	随机初始化权重(神经网络);
	puts("神经网络创建完成");

	if (!准备输出目录(输出目录, sizeof(输出目录)))
	{
		puts("输出目录创建失败");
		销毁神经网络(神经网络);
		数据集_销毁(训练集);
		数据集_销毁(推理集);
		return 1;
	}

	join_path(权重文本路径, sizeof(权重文本路径), 输出目录, "result_weight_121_x_times_x.txt");
	join_path(权重原始数据路径, sizeof(权重原始数据路径), 输出目录, "result_weight_121_x_times_x_raw.bin");
	join_path(推理结果路径, sizeof(推理结果路径), 输出目录, "result_predict_121_x_times_x.csv");
	join_path(训练日志路径, sizeof(训练日志路径), 输出目录, "train.txt");

	训练_带日志(神经网络, (训练集_t*)训练集, 1024 * 1024, 训练日志路径);

	if (!写出权重文本(神经网络, 权重文本路径))
	{
		puts("权重文本写出失败");
	}
	写入到文件(神经网络, 权重原始数据路径);
	if (!写出推理结果(神经网络, 推理集, 推理结果路径))
	{
		puts("推理结果写出失败");
	}

	puts("销毁神经网络与数据集");
	销毁神经网络(神经网络);
	数据集_销毁(训练集);
	数据集_销毁(推理集);
	puts("神经网络与数据集销毁完成");
	return 0;
}
