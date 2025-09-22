// main.cpp - 使用示例
#include "chart_generator.h"
#include <vector>
#include <random>
#include <iostream>

int main() {
    // 生成测试数据
    std::vector<float> x_data, y_data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0, 1);
    
    for (int i = 0; i < 50000; ++i) {
        x_data.push_back((dist(gen) * 100));
        y_data.push_back((dist(gen) * 100));
    }
    
    // 配置图表
    ChartConfig config;
    config.width = 1600;
    config.height = 1200;
    config.point_color = RGBColor(70, 130, 180); // 钢蓝色
    config.point_size = 1;
    config.show_grid = true;
    // 生成图表
    ChartGenerator generator(config);

    generator.addLineFunc(1.0, 2.0, RGBColor(0,0,255), 2, LineStyle::Solid);   // 实线
    generator.addLineFunc(-0.5, 3.0, RGBColor(0,255,0), 3, LineStyle::Dotted); // 点线
    generator.addLineFunc(0.2, -1.0, RGBColor(255,0,0), 2, LineStyle::Dashed); // 段线

    bool success = generator.generateScatterPlot(
        x_data, y_data,
        "X Axis Label", 
        "Y Axis Label",
        "Scatter Plot of 50,000 Points",
        "scatter_plot.png"
    );
    
    if (success) {
        std::cout << "图表生成成功: scatter_plot.png" << std::endl;
    } else {
        std::cout << "图表生成失败" << std::endl;
    }
    
    return 0;
}
