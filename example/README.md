<!--
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-07-30 15:15:09
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-07-30 16:57:25
-->
# Usage
## opencv test
(1)install
~~~bash
sudo apt-get install libopencv-dev
~~~
(2)compile
~~~bash
g++ -o opencv_test opencv_test.cpp `pkg-config --cflags --libs opencv4`
~~~
(3)run
~~~bash
./opencv_test
~~~

## vtk test
(1)install vtk
~~~bash

~~~

(2)compile
~~~cmake
cmake_minimum_required(VERSION 3.12)
project(example)

# 设置编译标准
set(CMAKE_CXX_STANDARD 17)

# 查找 VTK 库的路径
set(VTK_DIR "vtk install path")

# 查找 VTK 库
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# 源文件列表
set(SOURCES vtk_test.cpp)

# 生成共享库
add_executable(vtk_test ${SOURCES})

# 链接 VTK 库到共享库
target_link_libraries(vtk_test ${VTK_LIBRARIES})
~~~

~~~bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DOPENGL_INCLUDE_DIR=/usr/include -DOPENGL_gl_LIBRARY=/usr/lib/libGL.so ..
make
~~~

可能遇到的报错，找不到opengl，可以手动指定
~~~bash
CMake Error at /root/anaconda3/lib/python3.11/site-packages/cmake/data/share/cmake-3.29/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find OpenGL (missing: OPENGL_opengl_LIBRARY OPENGL_glx_LIBRARY)
Call Stack (most recent call first):
  /root/anaconda3/lib/python3.11/site-packages/cmake/data/share/cmake-3.29/Modules/FindPackageHandleStandardArgs.cmake:600 (_FPHSA_FAILURE_MESSAGE)
  /root/anaconda3/lib/python3.11/site-packages/cmake/data/share/cmake-3.29/Modules/FindOpenGL.cmake:579 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  CMake/vtkOpenGL.cmake:146 (find_package)
  ThirdParty/glew/vtkglew/CMakeLists.txt:17 (include)
~~~

(3)run
~~~bash
./vtk_test
~~~