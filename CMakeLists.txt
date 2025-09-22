cmake_minimum_required(VERSION 3.10)

project(stb_draw_graph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files here
add_executable(stb_draw_graph
    src/main.cpp
    src/chart_generator.cpp
)

# 如果需要，可以添加 include 目录
target_include_directories(stb_draw_graph PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
)

# 如有需要，链接其他库
# target_link_libraries(stb_draw_graph PRIVATE some_library)