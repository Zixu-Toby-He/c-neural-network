import pathlib

import matplotlib.pyplot
import pandas


当前路径 = pathlib.Path(__file__).resolve().parent


def 查找输出目录():
	候选路径 = [
		当前路径 / "output",
		当前路径 / "run" / "output",
		当前路径.parent / "run" / "output",
	]
	for 路径 in 候选路径:
		if (路径 / "result_predict_121_x_times_x.csv").exists():
			return 路径
	return 候选路径[0]


输出目录 = 查找输出目录()
预测文件路径 = 输出目录 / "result_predict_121_x_times_x.csv"
训练文件路径 = 输出目录 / "train.csv"
图片路径 = 输出目录 / "result_predict_121_x_times_x.png"

数据 = pandas.read_csv(预测文件路径)
训练数据 = pandas.read_csv(训练文件路径) if 训练文件路径.exists() else None

x = 数据["x"]
y_aim = 数据["y_aim"]
y_prd = 数据["y_prd"]
diff = 数据["diff"]

子图数量 = 3 if 训练数据 is not None else 2
图像, 坐标轴 = matplotlib.pyplot.subplots(子图数量, 1, figsize=(10, 11 if 训练数据 is not None else 8))

坐标轴[0].plot(x, y_aim, label="y_aim", linewidth=2)
坐标轴[0].plot(x, y_prd, label="y_prd", linewidth=2)
坐标轴[0].set_ylabel("y")
坐标轴[0].set_title("y = x^2 prediction")
坐标轴[0].grid(True, linestyle="--", alpha=0.4)
坐标轴[0].legend()

坐标轴[1].plot(x, diff, label="diff", color="tab:red", linewidth=2)
坐标轴[1].axhline(0, color="black", linewidth=1)
坐标轴[1].set_xlabel("x")
坐标轴[1].set_ylabel("diff")
坐标轴[1].grid(True, linestyle="--", alpha=0.4)
坐标轴[1].legend()

if 训练数据 is not None:
	训练轮数 = 训练数据["i"]
	训练偏差 = 训练数据["diff"]
	坐标轴[2].plot(训练轮数, 训练偏差, label="train diff", color="tab:green", linewidth=1.5)
	坐标轴[2].set_xlabel("training iteration")
	坐标轴[2].set_ylabel("diff")
	坐标轴[2].set_title("gradient descent training")
	坐标轴[2].grid(True, linestyle="--", alpha=0.4)
	坐标轴[2].legend()

图像.tight_layout()
图像.savefig(图片路径, dpi=160)
matplotlib.pyplot.show()
