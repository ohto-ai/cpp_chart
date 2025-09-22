// chart_generator.h
#ifndef CHART_GENERATOR_H
#define CHART_GENERATOR_H

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// RGB颜色结构体
struct RGBColor {
    uint8_t r, g, b;
    RGBColor(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) 
        : r(red), g(green), b(blue) {}
};

// 图表配置
struct ChartConfig {
    int width = 1200;
    int height = 800;
    RGBColor background_color = {255, 255, 255}; // 白色背景
    RGBColor axis_color = {0, 0, 0};            // 黑色坐标轴
    RGBColor grid_color = {240, 240, 240};      // 浅灰色网格
    RGBColor text_color = {0, 0, 0};            // 黑色文字
    RGBColor point_color = {255, 0, 0};         // 红色点
    
    int margin_left = 80;    // 左边距
    int margin_right = 40;   // 右边距
    int margin_top = 60;     // 上边距
    int margin_bottom = 80;  // 下边距
    
    int point_size = 2;      // 点的大小
    int axis_width = 2;      // 坐标轴宽度
    int grid_width = 1;      // 网格线宽度
    
    bool show_grid = true;   // 显示网格
    int grid_step_x = 0;     // 网格X步长（0表示自动）
    int grid_step_y = 0;     // 网格Y步长（0表示自动）
};

class ChartGenerator {
private:
    ChartConfig config;
    std::vector<uint8_t> pixels;
    int plot_width, plot_height;
    
    // 绘图区域坐标
    int plot_x1, plot_y1, plot_x2, plot_y2;
    
    // 数据范围
    double data_min_x, data_max_x, data_min_y, data_max_y;
    
public:
    ChartGenerator(const ChartConfig& cfg = ChartConfig());
    
    // 生成散点图
    bool generateScatterPlot(
        const std::vector<float>& x_data,
        const std::vector<float>& y_data,
        const std::string& x_label,
        const std::string& y_label,
        const std::string& title,
        const std::string& filename);
    
private:
    void initializePlotArea();
    void calculateDataRange(const std::vector<float>& x_data, const std::vector<float>& y_data);
    void drawBackground();
    void drawGrid();
    void drawAxes();
    void drawPoints(const std::vector<float>& x_data, const std::vector<float>& y_data);
    void drawLabels(const std::string& x_label, const std::string& y_label, const std::string& title);
    void drawTickMarks();void drawChar(int x, int y, char c, const RGBColor& color);
    void drawText(int x, int y, const std::string& text, const RGBColor& color);

    
    // 坐标转换
    int dataToPixelX(double x);
    int dataToPixelY(double y);
    
    // 绘图工具函数
    void setPixel(int x, int y, const RGBColor& color);
    void drawLine(int x1, int y1, int x2, int y2, const RGBColor& color, int width = 1);
    void drawCircle(int center_x, int center_y, int radius, const RGBColor& color);
    
    // 刻度计算
    std::vector<double> calculateTicks(double min_val, double max_val, bool is_x_axis);
    std::string formatTickLabel(double value);
};

#endif
