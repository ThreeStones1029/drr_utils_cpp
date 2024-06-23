cmake_minimum_required(VERSION 3.12)
project(drr_utils_cpp)

# 设置编译标准
set(CMAKE_CXX_STANDARD 17)

# 查找 yaml boost matplotlib ITK库
find_package(yaml-cpp REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(ITK REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${ITK_INCLUDE_DIRS})
# 添加头文件目录
include_directories(include/io)
include_directories(include/detection)
include_directories(include/dataset_tools)
include_directories(include/drr)

# 源文件列表
set(SOURCES 
    src/io/file_process.cpp
    src/io/debug_print.cpp
    src/io/image_process.cpp
    src/detection/coco_detection_data.cpp
    src/detection/GenDetectionDataset.cpp
    src/dataset_tools/dataset_sample.cpp
    src/drr/ITKGenDrr.cpp
    main_drr_detection_dataset.cpp)

add_executable(main_drr_detection_dataset ${SOURCES})

# 链接 yaml Boost ITK库
target_link_libraries(main_drr_detection_dataset yaml-cpp::yaml-cpp ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${ITK_LIBRARIES})