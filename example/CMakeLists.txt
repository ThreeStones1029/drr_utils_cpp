cmake_minimum_required(VERSION 3.12)
project(example)

# 设置编译标准
set(CMAKE_CXX_STANDARD 17)

# 查找 VTK 库的路径
set(VTK_DIR "/home/software/VTK-8.2.0")

# 查找 VTK 库
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# 源文件列表
set(SOURCES vtk_test.cpp)

# 生成可执行文件
add_executable(vtk_test ${SOURCES})

# 链接 VTK 库到共享库
target_link_libraries(vtk_test ${VTK_LIBRARIES})