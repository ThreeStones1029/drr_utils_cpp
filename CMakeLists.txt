cmake_minimum_required(VERSION 3.12)
project(drr_utils_cpp)

# 设置编译标准
set(CMAKE_CXX_STANDARD 17)

# 查找 yaml boost库
find_package(yaml-cpp REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

# 添加头文件目录
include_directories(include/io)

# 源文件列表
set(SOURCES 
    src/io/common.cpp
    main_drr_detection_dataset.cpp)

add_executable(main_drr_detection_dataset ${SOURCES})

# 链接 yaml Boost库
target_link_libraries(main_drr_detection_dataset yaml-cpp::yaml-cpp ${Boost_LIBRARIES})