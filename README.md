# c-neural-network

基于 CPU 的轻量级 C 语言神经网络实现。项目使用中文标识符编写核心 API，提供基础数学工具、数据集封装、常见激活函数、前向推理、训练、权重导出，以及若干可直接运行的示例程序。

## 项目特点

- 纯 C 实现，核心代码位于 `神经网络/sc/neural`。
- 使用 `double` 作为主要数值类型。
- 支持多层全连接网络的创建、推理和训练。
- 内置线性、绝对值、平方、三次方、指数、sigmoid、tanh、ReLU、LeakyReLU、Softmax 等激活函数。
- 提供数据集辅助类型和基础向量/梯度下降工具。
- 支持将训练后的网络权重写出为文本文件和二进制文件。
- Makefile 支持 Windows 和 Linux，自动构建动态库和 `sc/main` 下的示例程序。

## 目录结构

```text
.
├── README.md
├── LICENSE
└── 神经网络
    ├── Makefile
    ├── resource
    │   └── 1551_rre_train_x_times_x
    │       └── visualize_1_in_1_out.py
    └── sc
        ├── neural
        │   ├── include
        │   │   ├── activate.h
        │   │   ├── dataset.h
        │   │   ├── math_base.h
        │   │   └── neural.h
        │   └── src
        │       ├── activate.c
        │       ├── dataset.c
        │       ├── math_base.c
        │       └── neural.c
        └── main
            ├── grad_down
            ├── 1551_rre_train_x_times_x
            └── 2554_sssr_quadrant
```

## 环境要求

- Windows：MinGW-w64，并使用其工具链中的 `mingw32-make.exe`
- Linux：GNU Make
- GCC
- C 标准：GNU11
- 数学库：`libm`

编码注意事项：

- 源码中包含中文标识符，请确保文件以 UTF-8 编码保存。
- Windows 下 Makefile 会自动切换控制台到 UTF-8。
- Linux 下建议使用支持 UTF-8 中文标识符的 GCC 版本，推荐 GCC 10 或更高版本；Makefile 会在构建前执行编译器检查。

## 构建

进入项目的 Makefile 所在目录：

```bash
cd 神经网络
```

Windows 环境下使用 MinGW-w64 工具链中的 `mingw32-make.exe` 构建：

```powershell
mingw32-make.exe
```

Linux 环境下使用 `make` 构建：

```bash
make
```

构建完成后会生成：

- Windows：`run/windows/neural.dll` 和示例 `.exe`
- Linux：`run/linux/libneural.so` 和示例可执行文件

如需只构建核心神经网络动态库：

```powershell
mingw32-make.exe neural
```

Linux 下对应命令为 `make neural`。

如需查看当前构建环境：

```powershell
mingw32-make.exe env-info
```

Linux 下对应命令为 `make env-info`。

如需清理构建产物：

```powershell
mingw32-make.exe clean
```

Linux 下对应命令为 `make clean`。

## 运行示例

Makefile 会自动扫描 `sc/main/*` 下的示例目录，并为每个目录生成同名构建目标和 `run-<示例名>` 运行目标。

运行默认示例：

```powershell
mingw32-make.exe run
```

运行梯度下降示例：

```powershell
mingw32-make.exe run-grad_down
```

运行一元函数拟合示例：

```powershell
mingw32-make.exe run-1551_rre_train_x_times_x
```

该示例训练网络拟合 `y = x * x`，并在运行目录下生成输出文件。

运行象限分类示例：

```powershell
mingw32-make.exe run-2554_sssr_quadrant
```

该示例输入二维坐标 `(x, y)`，输出四个象限类别的概率分布。

Linux 下运行示例时，将上述命令中的 `mingw32-make.exe` 替换为 `make` 即可，例如 `make run-grad_down`。

## 示例输出

训练类示例会在对应平台的运行目录下创建 `output` 目录，例如：

```text
神经网络/run/windows/output
神经网络/run/linux/output
```

常见输出文件包括：

- `train.csv`：训练日志。
- `result_weight_*.txt`：文本格式权重。
- `result_weight_*_raw.bin`：二进制格式权重。
- `result_predict_*.csv`：推理结果、目标值和误差。

`1551_rre_train_x_times_x` 示例还包含一个 Python 可视化脚本：

```text
神经网络/resource/1551_rre_train_x_times_x/visualize_1_in_1_out.py
```

构建该示例时，Makefile 会把对应 `resource` 目录内容复制到运行目录。

## 核心 API 概览

头文件位于 `神经网络/sc/neural/include`。

### 神经网络

`neural.h` 提供主要网络结构和操作函数：

- `新建神经网络(...)`：创建多层神经网络。
- `推理计算(...)`：执行前向推理。
- `训练(...)`：训练网络。
- `训练_带日志(...)`：训练网络并写出日志。
- `写入到bin文件(...)`：导出二进制权重。
- `写入到txt文件(...)`：导出文本权重。
- `销毁神经网络(...)`：释放网络内存。

### 数据集

`dataset.h` 提供数据集创建、访问、格式化和销毁函数：

- `数据集_新建(...)`
- `数据集_获取一组自变量参数(...)`
- `数据集_获取一组函数值参数(...)`
- `数据集_str(...)`
- `数据集_repr(...)`
- `数据集_销毁(...)`

### 激活函数

`activate.h` 提供内置激活函数和自定义激活函数登记能力：

- `线性激励函数`
- `绝对值激励函数`
- `平方激励函数`
- `三次方激励函数`
- `指数激励函数`
- `指数减一激励函数`
- `sigmoid激励函数`
- `tanh激励函数`
- `relu激励函数`
- `leakyrelu激励函数`
- `softmax归一化函数`
- `新引入激励函数(...)`
- `获取激活函数名称(...)`
- `获取激活函数短名(...)`

### 数学工具

`math_base.h` 提供向量长度、距离、单位化、固定长度方向化、梯度和梯度下降求解等函数。

## 新增示例程序

如需添加新的示例程序：

1. 在 `神经网络/sc/main` 下新建一个目录，例如 `my_demo`。
2. 在该目录中放入 `main.c` 和需要的 `.c/.h` 文件。
3. 在代码中通过头文件使用核心库，例如：

```c
#include "activate.h"
#include "dataset.h"
#include "neural.h"
```

4. 运行：

```powershell
mingw32-make.exe my_demo
mingw32-make.exe run-my_demo
```

Linux 下对应命令为 `make my_demo` 和 `make run-my_demo`。

Makefile 会自动发现并构建该目录。

## 许可证

本项目使用 MIT License，详见 [LICENSE](LICENSE)。
