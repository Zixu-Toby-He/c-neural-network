import pathlib

import matplotlib.pyplot
import pandas


当前路径 = pathlib.Path(__file__).resolve().parent
预测文件名 = "result_predict_2554_sssr_quadrant.csv"
图片文件名 = "result_predict_2554_sssr_quadrant_meshgrid.png"


def 查找输出目录():
	候选路径 = []
	for 基准路径 in [当前路径, *当前路径.parents]:
		候选路径.extend(
			[
				基准路径 / "output",
				基准路径 / "run" / "windows" / "output",
				基准路径 / "run" / "linux" / "output",
			]
		)

	已检查路径 = set()
	for 路径 in 候选路径:
		if 路径 in 已检查路径:
			continue
		已检查路径.add(路径)
		if (路径 / 预测文件名).exists():
			return 路径
	return 候选路径[0]


def 读取网格数据(预测文件路径):
	数据 = pandas.read_csv(预测文件路径)
	必要列 = {"x", "y", "q1_prd", "q2_prd", "q3_prd", "q4_prd"}
	缺失列 = 必要列.difference(数据.columns)
	if 缺失列:
		raise ValueError(f"CSV missing columns: {sorted(缺失列)}")
	return 数据


def 生成网格矩阵(数据, 数值列):
	网格矩阵 = 数据.pivot_table(index="y", columns="x", values=数值列, aggfunc="mean")
	网格矩阵 = 网格矩阵.sort_index(axis=0).sort_index(axis=1)
	return 网格矩阵


输出目录 = 查找输出目录()
预测文件路径 = 输出目录 / 预测文件名
图片路径 = 输出目录 / 图片文件名

数据 = 读取网格数据(预测文件路径)
数值列组 = ["q1_prd", "q2_prd", "q3_prd", "q4_prd"]
标题组 = ["output 1", "output 2", "output 3", "output 4"]

图像, 坐标轴 = matplotlib.pyplot.subplots(2, 2, figsize=(14, 12), constrained_layout=True)

for 单个坐标轴, 数值列, 标题 in zip(坐标轴.flat, 数值列组, 标题组):
	网格矩阵 = 生成网格矩阵(数据, 数值列)
	颜色图 = 单个坐标轴.imshow(
		网格矩阵.values,
		origin="lower",
		extent=[
			网格矩阵.columns.min(),
			网格矩阵.columns.max(),
			网格矩阵.index.min(),
			网格矩阵.index.max(),
		],
		cmap="viridis",
	)
	单个坐标轴.set_title(标题)
	单个坐标轴.set_xlabel("x")
	单个坐标轴.set_ylabel("y")
	图像.colorbar(颜色图, ax=单个坐标轴, fraction=0.046, pad=0.04)

图像.suptitle("2554_sssr_quadrant meshgrid predictions", fontsize=14)
图像.savefig(图片路径, dpi=180)
matplotlib.pyplot.show()
