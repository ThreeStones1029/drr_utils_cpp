cmake_minimum_required(VERSION 3.12)
project(drr_utils_cpp)

# 设置编译标准
set(CMAKE_CXX_STANDARD 14)

# 查找 yaml 库
find_package(yaml-cpp REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
# 源文件列表
set(SOURCES main_drr_detection_dataset.cpp)

# 生成共享库
add_executable(main_drr_detection_dataset ${SOURCES})

# 链接 yaml Boost库
target_link_libraries(main_drr_detection_dataset yaml-cpp::yaml-cpp ${Boost_LIBRARIES})