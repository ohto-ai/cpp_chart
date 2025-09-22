void drawChar(int x, int y, char c, const RGBColor& color);
void drawText(int x, int y, const std::string& text, const RGBColor& color);
enum LineStyle { Solid, Dotted, Dashed };
void drawLineFunc(double k, double b, const RGBColor& color, int width, LineStyle style);