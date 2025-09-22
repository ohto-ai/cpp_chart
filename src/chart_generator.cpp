// chart_generator.cpp
#include "chart_generator.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <sstream>
#include <iomanip>

extern "C" {
#include <font8x8/font8x8.h>
}

ChartGenerator::ChartGenerator(const ChartConfig& cfg) : config(cfg) {
    plot_width = config.width - config.margin_left - config.margin_right;
    plot_height = config.height - config.margin_top - config.margin_bottom;
    pixels.resize(config.width * config.height * 3, 255);
}

bool ChartGenerator::generateScatterPlot(
    const std::vector<float>& x_data,
    const std::vector<float>& y_data,
    const std::string& x_label,
    const std::string& y_label,
    const std::string& title,
    const std::string& filename) {
    
    if (x_data.size() != y_data.size() || x_data.empty()) {
        return false;
    }
    
    // 计算数据范围
    calculateDataRange(x_data, y_data);
    
    // 初始化绘图区域
    initializePlotArea();
    
    // 绘制各个组件
    drawBackground();
    if (config.show_grid) {
        drawGrid();
    }
    drawAxes();
    drawPoints(x_data, y_data);
    drawTickMarks();
    drawLabels(x_label, y_label, title);
    
    // 保存为PNG
    return stbi_write_png(filename.c_str(), config.width, config.height, 3, 
                         pixels.data(), config.width * 3);
}

void ChartGenerator::initializePlotArea() {
    plot_x1 = config.margin_left;
    plot_y1 = config.margin_top;
    plot_x2 = config.width - config.margin_right;
    plot_y2 = config.height - config.margin_bottom;
}

void ChartGenerator::calculateDataRange(const std::vector<float>& x_data, const std::vector<float>& y_data) {
    data_min_x = *std::min_element(x_data.begin(), x_data.end());
    data_max_x = *std::max_element(x_data.begin(), x_data.end());
    data_min_y = *std::min_element(y_data.begin(), y_data.end());
    data_max_y = *std::max_element(y_data.begin(), y_data.end());
    
    // 添加一些边距
    double x_range = data_max_x - data_min_x;
    double y_range = data_max_y - data_min_y;
    data_min_x -= x_range * 0.05;
    data_max_x += x_range * 0.05;
    data_min_y -= y_range * 0.05;
    data_max_y += y_range * 0.05;
}

void ChartGenerator::drawBackground() {
    // 填充背景色
    for (int y = 0; y < config.height; ++y) {
        for (int x = 0; x < config.width; ++x) {
            setPixel(x, y, config.background_color);
        }
    }
}

void ChartGenerator::drawGrid() {
    auto x_ticks = calculateTicks(data_min_x, data_max_x, true);
    auto y_ticks = calculateTicks(data_min_y, data_max_y, false);
    
    // 绘制垂直网格线
    for (double tick : x_ticks) {
        int x = dataToPixelX(tick);
        if (x > plot_x1 && x < plot_x2) {
            drawLine(x, plot_y1, x, plot_y2, config.grid_color, config.grid_width);
        }
    }
    
    // 绘制水平网格线
    for (double tick : y_ticks) {
        int y = dataToPixelY(tick);
        if (y > plot_y1 && y < plot_y2) {
            drawLine(plot_x1, y, plot_x2, y, config.grid_color, config.grid_width);
        }
    }
}

void ChartGenerator::drawAxes() {
    // X轴：如果0在范围内，绘制在0，否则绘制在底部边界
    int x_axis_y;
    if (0 >= data_min_y && 0 <= data_max_y) {
        x_axis_y = dataToPixelY(0);
    } else {
        x_axis_y = plot_y2; // 底部边界
    }
    drawLine(plot_x1, x_axis_y, plot_x2, x_axis_y, config.axis_color, config.axis_width);

    // Y轴：如果0在范围内，绘制在0，否则绘制在左侧边界
    int y_axis_x;
    if (0 >= data_min_x && 0 <= data_max_x) {
        y_axis_x = dataToPixelX(0);
    } else {
        y_axis_x = plot_x1; // 左侧边界
    }
    drawLine(y_axis_x, plot_y1, y_axis_x, plot_y2, config.axis_color, config.axis_width);
}

void ChartGenerator::drawPoints(const std::vector<float>& x_data, const std::vector<float>& y_data) {
    for (size_t i = 0; i < x_data.size(); ++i) {
        int x = dataToPixelX(x_data[i]);
        int y = dataToPixelY(y_data[i]);
        
        if (x >= plot_x1 && x <= plot_x2 && y >= plot_y1 && y <= plot_y2) {
            if (config.point_size == 1) {
                setPixel(x, y, config.point_color);
            } else {
                drawCircle(x, y, config.point_size, config.point_color);
            }
        }
    }
}

void ChartGenerator::drawTickMarks() {
    auto x_ticks = calculateTicks(data_min_x, data_max_x, true);
    auto y_ticks = calculateTicks(data_min_y, data_max_y, false);

    // X轴刻度
    int x_axis_y = dataToPixelY(0);
    for (double tick : x_ticks) {
        int x = dataToPixelX(tick);
        if (x >= plot_x1 && x <= plot_x2) {
            // 刻度线
            drawLine(x, x_axis_y - 5, x, x_axis_y + 5, config.axis_color, 1);

            // 刻度文本
            std::string label = formatTickLabel(tick);
            int label_x = x - (int)label.size() * 4;
            int label_y = x_axis_y + 10;
            drawText(label_x, label_y, label, config.axis_color);
        }
    }

    // Y轴刻度
    int y_axis_x = dataToPixelX(0);
    for (double tick : y_ticks) {
        int y = dataToPixelY(tick);
        if (y >= plot_y1 && y <= plot_y2) {
            // 刻度线
            drawLine(y_axis_x - 5, y, y_axis_x + 5, y, config.axis_color, 1);

            // 刻度文本
            std::string label = formatTickLabel(tick);
            int label_x = y_axis_x - (int)label.size() * 8 - 8;
            int label_y = y - 4;
            drawText(label_x, label_y, label, config.axis_color);
        }
    }
}

void ChartGenerator::drawLabels(const std::string& x_label, const std::string& y_label, const std::string& title) {
    // 绘制标题（居中）
    if (!title.empty()) {
        int tx = config.width / 2 - (int)title.size() * 4;
        int ty = config.margin_top / 2 - 4;
        drawText(tx, ty, title, config.axis_color);
    }
    // 绘制X轴标签（居中）
    if (!x_label.empty()) {
        int tx = config.width / 2 - (int)x_label.size() * 4;
        int ty = config.height - config.margin_bottom / 2 + 16;
        drawText(tx, ty, x_label, config.axis_color);
    }
    // 绘制Y轴标签（竖排，居中）
    if (!y_label.empty()) {
        int tx = config.margin_left / 4;
        int ty = config.height / 2 - (int)y_label.size() * 4;
        for (size_t i = 0; i < y_label.size(); ++i) {
            drawChar(tx, ty + i * 8, y_label[i], config.axis_color);
        }
    }
}

// 绘制单个字符（使用font8x8_basic）
void ChartGenerator::drawChar(int x, int y, char c, const RGBColor& color) {
    typedef char bitmap_t[8];
    if (c < 0 || c > 127) return;
    auto bitmap = static_cast<bitmap_t>(font8x8_basic[(int)c]);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (bitmap[row] & (1 << col)) {
                setPixel(x + col, y + row, color);
            }
        }
    }
}

// 绘制字符串
void ChartGenerator::drawText(int x, int y, const std::string& text, const RGBColor& color) {
    int cursor_x = x;
    for (char c : text) {
        drawChar(cursor_x, y, c, color);
        cursor_x += 8; // 每个字符宽度为8
    }
}

// 坐标转换函数
int ChartGenerator::dataToPixelX(double x) {
    return plot_x1 + static_cast<int>((x - data_min_x) / (data_max_x - data_min_x) * plot_width);
}

int ChartGenerator::dataToPixelY(double y) {
    return plot_y2 - static_cast<int>((y - data_min_y) / (data_max_y - data_min_y) * plot_height);
}

// 绘图工具函数
void ChartGenerator::setPixel(int x, int y, const RGBColor& color) {
    if (x < 0 || x >= config.width || y < 0 || y >= config.height) return;
    
    int index = (y * config.width + x) * 3;
    pixels[index] = color.r;
    pixels[index + 1] = color.g;
    pixels[index + 2] = color.b;
}

void ChartGenerator::drawLine(int x1, int y1, int x2, int y2, const RGBColor& color, int width) {
    // Bresenham直线算法
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        for (int i = -width/2; i <= width/2; ++i) {
            for (int j = -width/2; j <= width/2; ++j) {
                setPixel(x1 + i, y1 + j, color);
            }
        }
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void ChartGenerator::drawCircle(int center_x, int center_y, int radius, const RGBColor& color) {
    int x = radius;
    int y = 0;
    int err = 0;
    
    while (x >= y) {
        for (int i = -x; i <= x; ++i) {
            setPixel(center_x + i, center_y + y, color);
            setPixel(center_x + i, center_y - y, color);
        }
        for (int i = -y; i <= y; ++i) {
            setPixel(center_x + i, center_y + x, color);
            setPixel(center_x + i, center_y - x, color);
        }
        
        y += 1;
        err += 1 + 2*y;
        if (2*(err - x) + 1 > 0) {
            x -= 1;
            err += 1 - 2*x;
        }
    }
}

// 刻度计算函数
std::vector<double> ChartGenerator::calculateTicks(double min_val, double max_val, bool is_x_axis) {
    std::vector<double> ticks;
    double range = max_val - min_val;
    
    // 计算合适的刻度间隔
    double magnitude = pow(10, floor(log10(range)));
    double interval = magnitude;
    
    if (range / interval < 4) {
        interval /= 2;
    } else if (range / interval > 8) {
        interval *= 2;
    }
    
    // 生成刻度值
    double start = ceil(min_val / interval) * interval;
    double end = floor(max_val / interval) * interval;
    
    for (double tick = start; tick <= end + interval/2; tick += interval) {
        ticks.push_back(tick);
    }
    
    return ticks;
}

std::string ChartGenerator::formatTickLabel(double value) {
    std::stringstream ss;
    if (fabs(value) < 1e-10) value = 0; // 处理浮点误差
    
    if (fabs(value) >= 1000 || (fabs(value) < 0.01 && value != 0)) {
        ss << std::scientific << std::setprecision(2) << value;
    } else {
        ss << std::fixed << std::setprecision(3) << value;
    }
    return ss.str();
}
