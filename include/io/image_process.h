/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-05-23 08:13:27
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-23 02:56:24
 */
#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
#include <itkImage.h>
#include <vector>
// #include <opencv2/opencv.hpp>


itk::ImageRegion<3> GetMinimumBoundingBox(itk::Image<float, 3>::Pointer itkImage);

const std::vector<double> BoundingBoxTo2DVector(const itk::ImageRegion<3>& bbox);

// cv::Mat itkImageToCvMat(itk::Image<float, 3>::Pointer itkImage);

// std::vector<std::vector<int>> GetMinimumRotationBox(itk::Image<float, 3>::Pointer itkImage);

#endif // IMAGE_PROCESS_H
