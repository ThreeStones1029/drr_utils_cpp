/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-05-23 08:14:11
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-23 02:43:32
 */
#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIOFactory.h>
#include <itkResampleImageFilter.h>
#include <itkCenteredEuler3DTransform.h>
#include <itkRayCastInterpolateImageFunction.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkLabelStatisticsImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkBinaryImageToShapeLabelMapFilter.h>
// #include <opencv2/opencv.hpp>
#include <itkImageRegionConstIterator.h>


// 获取图像的最小包围盒
itk::ImageRegion<3> GetMinimumBoundingBox(itk::Image<float, 3>::Pointer itkImage) {
    using InputImageType = itk::Image<float, 3>;
    using OutputPixelType = unsigned char;
    using OutputImageType = itk::Image<OutputPixelType, 3>;

    // 使用 CastImageFilter 将 float 图像转换为 unsigned char 图像
    using CastFilterType = itk::CastImageFilter<InputImageType, OutputImageType>;
    CastFilterType::Pointer castFilter = CastFilterType::New();
    castFilter->SetInput(itkImage);

    // 使用二值化滤波器
    using BinaryThresholdFilterType = itk::BinaryThresholdImageFilter<OutputImageType, OutputImageType>;
    BinaryThresholdFilterType::Pointer binaryFilter = BinaryThresholdFilterType::New();
    binaryFilter->SetInput(castFilter->GetOutput());
    binaryFilter->SetLowerThreshold(1); // 根据实际情况设置阈值
    binaryFilter->SetInsideValue(1);
    binaryFilter->SetOutsideValue(0);

    // 更新二值化滤波器
    try {
        binaryFilter->Update();
    }
    catch (itk::ExceptionObject& err) {
        std::cerr << "ERROR: ExceptionObject caught while updating the binary filter!" << std::endl;
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }

    // 计算标签统计
    using LabelStatisticsFilterType = itk::LabelStatisticsImageFilter<OutputImageType, OutputImageType>;
    LabelStatisticsFilterType::Pointer labelStatisticsFilter = LabelStatisticsFilterType::New();
    labelStatisticsFilter->SetInput(binaryFilter->GetOutput());
    labelStatisticsFilter->SetLabelInput(binaryFilter->GetOutput());

    try {
        labelStatisticsFilter->Update();
    }
    catch (itk::ExceptionObject& err) {
        std::cerr << "ERROR: ExceptionObject caught while computing label statistics!" << std::endl;
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }

    // 获取最小包围盒
    return labelStatisticsFilter->GetRegion(1); // Label 1代表非零像素
}


// 将三维最小包围盒转换为二维包围盒，并返回 std::vector<double>
const std::vector<double> BoundingBoxTo2DVector(const itk::ImageRegion<3>& bbox) {
    std::vector<double> bbox_vec;
    const itk::ImageRegion<3>::SizeType& size = bbox.GetSize();
    const itk::ImageRegion<3>::IndexType& index = bbox.GetIndex();
    // 选择 Z=0 的平面
    itk::ImageRegion<2> bbox_2d_region;
    bbox_2d_region.SetIndex(0, index[0]);
    bbox_2d_region.SetIndex(1, index[1]);
    bbox_2d_region.SetSize(0, size[0]);
    bbox_2d_region.SetSize(1, size[1]);

    // 添加起始索引到向量
    bbox_vec.push_back(static_cast<double>(bbox_2d_region.GetIndex(0)));
    bbox_vec.push_back(static_cast<double>(bbox_2d_region.GetIndex(1)));

    // 添加大小到向量
    bbox_vec.push_back(static_cast<double>(bbox_2d_region.GetSize(0)));
    bbox_vec.push_back(static_cast<double>(bbox_2d_region.GetSize(1)));

    return bbox_vec;
}

// 将 ITK 图像转换为 OpenCV 矩阵
// cv::Mat itkImageToCvMat(itk::Image<float, 3>::Pointer itkImage) {
//     // 获取图像大小
//     itk::Image<float, 3>::SizeType size = itkImage->GetLargestPossibleRegion().GetSize();

//     // 创建一个 OpenCV 矩阵，尺寸为 ITK 图像的 2D 投影（z 轴为平面）
//     cv::Mat cvImage(size[1], size[0], CV_32F, cv::Scalar(0));

//     // 遍历 ITK 图像并将数据复制到 OpenCV 矩阵
//     itk::ImageRegionConstIterator<itk::Image<float, 3>> it(itkImage, itkImage->GetLargestPossibleRegion());
//     for (int z = 0; z < size[2]; ++z) {
//         for (int y = 0; y < size[1]; ++y) {
//             for (int x = 0; x < size[0]; ++x) {
//                 itk::Image<float, 3>::IndexType index = {x, y, z};
//                 cvImage.at<float>(y, x) = itkImage->GetPixel(index);
//             }
//         }
//     }

//     // 转换为 8 位灰度图像
//     cv::Mat cvImage8U;
//     cvImage.convertTo(cvImage8U, CV_8U, 255.0);

//     return cvImage8U;
// }


// std::vector<std::vector<int>> GetMinimumRotationBox(itk::Image<float, 3>::Pointer itkImage) {
//     // Convert ITK image to OpenCV image
//     cv::Mat cvImage = itkImageToCvMat(itkImage);

//     // Threshold the image to create a binary image
//     cv::Mat binaryImage;
//     cv::threshold(cvImage, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

//     // Find contours
//     std::vector<std::vector<cv::Point>> contours;
//     cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//     if (contours.empty()) {
//         return {{0, 0, 0, 0, 0, 0, 0, 0}};
//     }

//     // Calculate the minimum area rectangle for each contour
//     std::vector<cv::RotatedRect> rotation_rects;
//     for (const auto& contour : contours) {
//         rotation_rects.push_back(cv::minAreaRect(contour));
//     }

//     // Find the largest area minimum bounding rectangle
//     auto max_rect_it = std::max_element(rotation_rects.begin(), rotation_rects.end(), [](const cv::RotatedRect& a, const cv::RotatedRect& b) {
//         return a.size.area() < b.size.area();
//     });

//     cv::RotatedRect rotation_max_rect = *max_rect_it;
//     cv::Point2f rotation_bbox[4];
//     rotation_max_rect.points(rotation_bbox);

//     std::vector<int> segmentation;
//     for (int i = 0; i < 4; ++i) {
//         segmentation.push_back(static_cast<int>(rotation_bbox[i].x));
//         segmentation.push_back(static_cast<int>(rotation_bbox[i].y));
//     }

//     return {segmentation};
// }
