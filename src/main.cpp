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
        x_data.push_back(100 + fabs(dist(gen) * 100));
        y_data.push_back(fabs(dist(gen) * 100));
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
