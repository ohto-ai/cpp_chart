# cpp_chart

一个轻量级的 C++17 图表渲染库与示例，基于 [stb_image_write](https://github.com/nothings/stb) 和 [font8x8](https://github.com/dhepper/font8x8) 直接在内存中绘制像素并导出 PNG。无需 Matplotlib、Qt 或任何 GUI 依赖，适合在命令行工具、服务端或无图形环境中生成图表。

![scatter plot](scatter_plot.png)

## 功能

- 散点图绘制，自动计算数据范围、坐标轴刻度与网格
- 可配置画布尺寸、边距、背景色、坐标轴 / 网格 / 文字 / 点的颜色
- 支持叠加任意直线 `y = kx + b`，可选实线、点线、虚线
- 使用内置 font8x8 点阵字体绘制标题和坐标轴标签
- 输出单张 PNG 图片
- 依赖全部内置在 `inc/`，无需额外安装第三方库

## 依赖

- C++17 编译器
- CMake 3.10+
- 已内置：[`inc/stb`](inc/stb)、[`inc/font8x8`](inc/font8x8)

## 构建

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Windows + Visual Studio 生成器的输出位于 `build/Release/stb_draw_graph.exe`，单配置生成器（如 Ninja）位于 `build/stb_draw_graph`。

## 运行示例

```bash
./build/stb_draw_graph          # Linux / macOS
.\build\Release\stb_draw_graph.exe   # Windows (Visual Studio)
```

示例程序会生成 50,000 个正态分布随机点，并在当前目录输出 `scatter_plot.png`。

## 使用

```cpp
#include "chart_generator.h"

std::vector<float> x_data, y_data;
// ... 填充数据 ...

ChartConfig config;
config.width = 1600;
config.height = 1200;
config.point_color = RGBColor(70, 130, 180); // 钢蓝色
config.point_size = 1;
config.show_grid = true;

ChartGenerator generator(config);

// 叠加直线：y = kx + b
generator.addLineFunc(1.0, 2.0, RGBColor(0, 0, 255), 2, LineStyle::Solid);
generator.addLineFunc(-0.5, 3.0, RGBColor(0, 255, 0), 3, LineStyle::Dotted);
generator.addLineFunc(0.2, -1.0, RGBColor(255, 0, 0), 2, LineStyle::Dashed);

bool ok = generator.generateScatterPlot(
    x_data, y_data,
    "X Axis Label",
    "Y Axis Label",
    "Scatter Plot of 50,000 Points",
    "scatter_plot.png");
```

`ChartConfig` 还支持 `margin_left` / `margin_right` / `margin_top` / `margin_bottom`、`grid_step_x` / `grid_step_y`、`axis_width`、`grid_width` 等参数；`grid_step` 设为 `0` 时自动计算。

## 项目结构

```text
inc/                 内置第三方头文件（stb、font8x8）
src/chart_generator.h  图表配置与 ChartGenerator 接口
src/chart_generator.cpp 绘制、坐标变换与 PNG 输出
src/main.cpp         示例程序
```

## License

[MIT](LICENSE)
