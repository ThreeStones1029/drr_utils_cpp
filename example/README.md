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