/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-07-29 02:20:01
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-07-29 02:26:42
 */
#include <opencv2/opencv.hpp>

int main() {
    // 读取图像
    cv::Mat image = cv::imread("../data/verse2020_test_detection_dataset/images/verse004_AP_1.png");

    // 定义框的位置和大小
    int xmin = 100;
    int ymin = 100;
    int width = 200;
    int height = 150;

    // 计算框的另一角的坐标
    int xmax = xmin + width;
    int ymax = ymin + height;

    // 在图像上绘制矩形框
    cv::rectangle(image, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 255, 0), 2);

    // 显示图像
    cv::imshow("Bounding Box", image);
    cv::waitKey(0);

    return 0;
}
