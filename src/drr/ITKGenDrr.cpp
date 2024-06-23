#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkCenteredEuler3DTransform.h>
#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkPNGImageIO.h>
#include <itkPNGImageIOFactory.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkGiplImageIOFactory.h>
#include <itkCastImageFilter.h>
#include <itkNiftiImageIO.h>
#include <itkNiftiImageIOFactory.h>
#include <itkMetaImageIOFactory.h>
#include <itkRayCastInterpolateImageFunction.h>
#include "coco_detection_data.h"
#include "file_process.h"
#include "image_process.h"
#include <iostream>
#include <filesystem>
#include <sys/io.h>


int GenerateDrrs(const std::string& ct_file_path, const std::vector<std::vector<double>>& rotations, 
                  const std::vector<std::vector<double>>& translations, bool save_img, double sid_value, 
                  double sx_value, double sy_value, int dx_value, int dy_value, double threshold_value, 
				  const std::string& APorLA, const std::string& save_images_folder, const std::unique_ptr<COCODetectionData>& detection_dataset) {

	// 旋转中心的坐标
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;

	// 源到成像平面的距离（即源与检测器之间的距离）
	double sid = sid_value;

	// DRR图像在X和Y轴上的像素间隔
	double sx = sx_value;
	double sy = sy_value;

	// DRR图像的宽度和高度（以像素为单位）
	int dx = dx_value;
	int dy = dy_value;

	// DRR图像的原点偏移量
	double o2Dx = 0;
	double o2Dy = 0;

	// 图像阈值
	// 较低的阈值：
	//	将保留更多原始 CT 数据中的像素，包括低密度区域。
	//	这可能导致生成的 DRR 图像中显示更多的细节，但也可能包含一些噪音或无关信息。
	// 较高的阈值：
	//	将过滤掉原始 CT 数据中的低密度区域，只保留高密度区域。
	//	这可能导致生成的 DRR 图像中显示更强的骨骼结构，但可能丢失一些低密度的软组织信息。
	double threshold = threshold_value;
	bool verbose = false;
	const unsigned int Dimension = 3;
	using InputPixelType = float;
	using OutputPixelType = unsigned char;
	using InputImageType = itk::Image<InputPixelType, Dimension>;
	using OutputImageType = itk::Image<OutputPixelType, Dimension>;
	InputImageType::Pointer image;
	using ReaderType = itk::ImageFileReader<InputImageType>;
	ReaderType::Pointer reader = ReaderType::New();
	
	itk::NiftiImageIOFactory::RegisterOneFactory();
	reader->SetFileName(ct_file_path);
	try {
		reader->Update();
	}
	catch (itk::ExceptionObject& err) {
		std::cerr << "error exception object caught!" << std::endl;
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}
	image = reader->GetOutput();

	if (verbose) {
		const InputImageType::SpacingType spacing = image->GetSpacing();
		std::cout << std::endl << "Input: ";

		InputImageType::RegionType region = image->GetBufferedRegion();
		region.Print(std::cout);

		std::cout << " Resolution: [";
		for (int i = 0; i < Dimension; i++) {
			std::cout << spacing[i];
			if (i < Dimension - 1) std::cout << ", ";
		}
		std::cout << "]" << std::endl;

		const InputImageType::PointType origin = image->GetOrigin();
		std::cout << " Origin: [";
		for (int i = 0; i < Dimension; i++) {
			std::cout << origin[i];
			if (i < Dimension - 1) std::cout << ", ";
		}
		std::cout << "]" << std::endl << std::endl;
	}

	using FilterType = itk::ResampleImageFilter<InputImageType, InputImageType>;
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(image);
	filter->SetDefaultPixelValue(0);
	using TransformType = itk::CenteredEuler3DTransform<double>;
	TransformType::Pointer transform = TransformType::New();
	transform->SetComputeZYX(true);
	TransformType::OutputVectorType translation;
	std::string ct_name_with_nii = std::filesystem::path(ct_file_path).stem().string();
	std::string ct_name = std::filesystem::path(ct_name_with_nii).stem().string();
	for (int i = 0; i < rotations.size(); i++) {
		std::string image_name = ct_name + "_" + APorLA + "_" + std::to_string(i + 1) + ".png";
		std::string save_path = save_images_folder + "/" + image_name;
		// 添加标注
		detection_dataset->add_image(image_name, ct_name_with_nii, APorLA, dx, dy, rotations[i], translations[i]);
		double rx = rotations[i][0];
		double ry = rotations[i][1];
		double rz = rotations[i][2];
		translation[0] = translations[i][0];
		translation[1] = translations[i][1];
		translation[2] = translations[i][2];

		const double dtr = atan(1.0) * 4.0 / 180.0;
		transform->SetTranslation(translation);
		transform->SetRotation(dtr * rx, dtr * ry, dtr * rz);
		InputImageType::PointType imOrigin = image->GetOrigin();
		InputImageType::SpacingType imRes = image->GetSpacing();
		using InputImageRegionType = InputImageType::RegionType;
		using InputImageSizeType = InputImageRegionType::SizeType;
		InputImageRegionType imRegion = image->GetBufferedRegion();
		InputImageSizeType imSize = imRegion.GetSize();

		imOrigin[0] += imRes[0] * static_cast<double>(imSize[0]) / 2.0;
		imOrigin[1] += imRes[1] * static_cast<double>(imSize[1]) / 2.0;
		imOrigin[2] += imRes[2] * static_cast<double>(imSize[2]) / 2.0;
		
		TransformType::InputPointType center;
		center[0] = cx + imOrigin[0];
		center[1] = cy + imOrigin[1];
		center[2] = cz + imOrigin[2];
		transform->SetCenter(center);

		if (verbose) {
			std::cout << "Image size: " << imSize[0] << ", " << imSize[1] << ", " << imSize[2]
				<< std::endl << " resolution: " << imRes[0] << ", " << imRes[1] << ", " << imRes[2]
				<< std::endl << " origin: " << imOrigin[0] << ", " << imOrigin[1] << ", " <<
				imOrigin[2] << std::endl << " center: " << center[0] << ", " << center[1]
				<< ", " << center[2] << std::endl << "Transform: " << transform << std::endl;
		}
		using InterpolatorType = itk::RayCastInterpolateImageFunction<InputImageType, double>;
		InterpolatorType::Pointer interpolator = InterpolatorType::New();
		interpolator->SetTransform(transform);

		interpolator->SetThreshold(threshold);
		InterpolatorType::InputPointType focalpoint;

		focalpoint[0] = imOrigin[0];
		focalpoint[1] = imOrigin[1];
		focalpoint[2] = imOrigin[2] - sid / 2.0;
		interpolator->SetFocalPoint(focalpoint);

		if (verbose) {
			std::cout << "Focal Point: "
				<< focalpoint[0] << ", "
				<< focalpoint[1] << ", "
				<< focalpoint[2] << std::endl;
		}
		interpolator->Print(std::cout);

		filter->SetInterpolator(interpolator);
		filter->SetTransform(transform);
		// setup the scene
		InputImageType::SizeType size;
		size[0] = dx;  // number of pixels along X of the 2D DRR image
		size[1] = dy;  // number of pixels along Y of the 2D DRR image
		size[2] = 1;   // only one slice
		filter->SetSize(size);
		InputImageType::SpacingType spacing;

		spacing[0] = sx;  // pixel spacing along X of the 2D DRR image [mm]
		spacing[1] = sy;  // pixel spacing along Y of the 2D DRR image [mm]
		spacing[2] = 1.0; // slice thickness of the 2D DRR image [mm]
		filter->SetOutputSpacing(spacing);
		if (verbose)
		{
			std::cout << "Output image size: "
				<< size[0] << ", "
				<< size[1] << ", "
				<< size[2] << std::endl;

			std::cout << "Output image spacing: "
				<< spacing[0] << ", "
				<< spacing[1] << ", "
				<< spacing[2] << std::endl;
		}

		double origin[Dimension];
		origin[0] = imOrigin[0] + o2Dx - sx * ((double)dx - 1.) / 2.;
		origin[1] = imOrigin[1] + o2Dy - sy * ((double)dy - 1.) / 2.;
		origin[2] = imOrigin[2] + sid / 2.;
		filter->SetOutputOrigin(origin);
		if (verbose)
		{
			std::cout << "Output image origin: "
				<< origin[0] << ", "
				<< origin[1] << ", "
				<< origin[2] << std::endl;
		}

		// create writer
		using RescaleFilterType = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;
		RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(255);
		rescaler->SetInput(filter->GetOutput());

		using WriterType = itk::ImageFileWriter<OutputImageType>;
		WriterType::Pointer writer = WriterType::New();

		using pngType = itk::PNGImageIO;
		pngType::Pointer pngIO1 = pngType::New();
		itk::PNGImageIOFactory::RegisterOneFactory();
		writer->SetFileName(save_path);
		writer->SetImageIO(pngIO1);
		writer->SetImageIO(itk::PNGImageIO::New());
		writer->SetInput(rescaler->GetOutput());
		try
		{
			std::cout << "Writing image: " << save_path << std::endl;
			writer->Update();
		}
		catch (itk::ExceptionObject& err)
		{
			std::cerr << "ERROR: ExceptionObject caught !" << std::endl;
			std::cerr << err << std::endl;
		}
	}
	return 0;
}


int GenerateMasks(const std::string& ct_name, const std::vector<std::string>& seg_filepaths, const std::vector<std::vector<double>>& rotations, 
                  const std::vector<std::vector<double>>& translations, bool save_img, double sid_value, 
                  double sx_value, double sy_value, int dx_value, int dy_value, double threshold_value, 
                  const std::string& APorLA, const std::string& save_images_folder, const std::unique_ptr<COCODetectionData>& detection_dataset) {
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;
	double sid = sid_value;
	double sx = sx_value;
	double sy = sy_value;
	int dx = dx_value;
	int dy = dy_value;
	double o2Dx = 0;
	double o2Dy = 0;
	double threshold = threshold_value;
	bool verbose = false;
	const unsigned int Dimension = 3;
	using InputPixelType = float;
	using OutputPixelType = unsigned char;
	using InputImageType = itk::Image<InputPixelType, Dimension>;
	using OutputImageType = itk::Image<OutputPixelType, Dimension>;

	for (std::string seg_file_path : seg_filepaths) {
		InputImageType::Pointer image;
		using ReaderType = itk::ImageFileReader<InputImageType>;
		ReaderType::Pointer reader = ReaderType::New();
		
		itk::NiftiImageIOFactory::RegisterOneFactory();
		reader->SetFileName(seg_file_path);
		try {
			reader->Update();
		}
		catch (itk::ExceptionObject& err) {
			std::cerr << "error exception object caught!" << std::endl;
			std::cerr << err.what() << std::endl;
			return EXIT_FAILURE;
		}
		image = reader->GetOutput();

		using FilterType = itk::ResampleImageFilter<InputImageType, InputImageType>;
		FilterType::Pointer filter = FilterType::New();
		filter->SetInput(image);
		filter->SetDefaultPixelValue(0);

		using TransformType = itk::CenteredEuler3DTransform<double>;
		TransformType::Pointer transform = TransformType::New();
		transform->SetComputeZYX(true);
		TransformType::OutputVectorType translation;
		
		std::string mask_name_with_nii = std::filesystem::path(seg_file_path).stem().string();
		std::string mask_name_with_seg = std::filesystem::path(mask_name_with_nii).stem().string();
		std::string mask_name = mask_name_with_seg.substr(0, mask_name_with_seg.size() - 4);
		std::vector<std::string> parts = splitString(mask_name_with_seg, '_');
		std::string category_name = parts[0];
		for (int i = 0; i < rotations.size(); i++) {
			std::string image_name = ct_name  + "_" + APorLA + "_" + mask_name + "_" + std::to_string(i + 1) + ".png";
			std::string save_path = save_images_folder + "/" + image_name;
			// 添加标注
			double rx = rotations[i][0];
			double ry = rotations[i][1];
			double rz = rotations[i][2];
			translation[0] = translations[i][0];
			translation[1] = translations[i][1];
			translation[2] = translations[i][2];

			const double dtr = atan(1.0) * 4.0 / 180.0;
			transform->SetTranslation(translation);
			transform->SetRotation(dtr * rx, dtr * ry, dtr * rz);
			InputImageType::PointType imOrigin = image->GetOrigin();
			InputImageType::SpacingType imRes = image->GetSpacing();
			using InputImageRegionType = InputImageType::RegionType;
			using InputImageSizeType = InputImageRegionType::SizeType;
			InputImageRegionType imRegion = image->GetBufferedRegion();
			InputImageSizeType imSize = imRegion.GetSize();
			imOrigin[0] += imRes[0] * static_cast<double>(imSize[0]) / 2.0;
			imOrigin[1] += imRes[1] * static_cast<double>(imSize[1]) / 2.0;
			imOrigin[2] += imRes[2] * static_cast<double>(imSize[2]) / 2.0;
			
			TransformType::InputPointType center;
			center[0] = cx + imOrigin[0];
			center[1] = cy + imOrigin[1];
			center[2] = cz + imOrigin[2];
			transform->SetCenter(center);

			using InterpolatorType = itk::RayCastInterpolateImageFunction<InputImageType, double>;
			InterpolatorType::Pointer interpolator = InterpolatorType::New();
			interpolator->SetTransform(transform);

			interpolator->SetThreshold(threshold);
			InterpolatorType::InputPointType focalpoint;

			focalpoint[0] = imOrigin[0];
			focalpoint[1] = imOrigin[1];
			focalpoint[2] = imOrigin[2] - sid / 2.0;

			interpolator->SetFocalPoint(focalpoint);
			interpolator->Print(std::cout);
			filter->SetInterpolator(interpolator);
			filter->SetTransform(transform);

			// setup the scene
			InputImageType::SizeType size;
			size[0] = dx;  // number of pixels along X of the 2D DRR image
			size[1] = dy;  // number of pixels along Y of the 2D DRR image
			size[2] = 1;   // only one slice
			filter->SetSize(size);
			InputImageType::SpacingType spacing;
			spacing[0] = sx;  // pixel spacing along X of the 2D DRR image [mm]
			spacing[1] = sy;  // pixel spacing along Y of the 2D DRR image [mm]
			spacing[2] = 1.0; // slice thickness of the 2D DRR image [mm]
			filter->SetOutputSpacing(spacing);

			double origin[Dimension];
			origin[0] = imOrigin[0] + o2Dx - sx * ((double)dx - 1.) / 2.;
			origin[1] = imOrigin[1] + o2Dy - sy * ((double)dy - 1.) / 2.;
			origin[2] = imOrigin[2] + sid / 2.;
			filter->SetOutputOrigin(origin);
			
			// create writer
			if (save_img) {
				using RescaleFilterType = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;
				RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
				rescaler->SetOutputMinimum(0);
				rescaler->SetOutputMaximum(255);
				rescaler->SetInput(filter->GetOutput());

				using WriterType = itk::ImageFileWriter<OutputImageType>;
				WriterType::Pointer writer = WriterType::New();

				using pngType = itk::PNGImageIO;
				pngType::Pointer pngIO1 = pngType::New();
				itk::PNGImageIOFactory::RegisterOneFactory();
				writer->SetFileName(save_path);
				writer->SetImageIO(pngIO1);
				writer->SetImageIO(itk::PNGImageIO::New());
				writer->SetInput(rescaler->GetOutput());
				try
				{
					std::cout << "Writing image: " << save_path << std::endl;
					writer->Update();
				}
				catch (itk::ExceptionObject& err)
				{
					std::cerr << "ERROR: ExceptionObject caught !" << std::endl;
					std::cerr << err << std::endl;
				}
			}
			else {
				// 获取图像的最小包围盒
				itk::ImageRegion<Dimension> boundingBox = GetMinimumBoundingBox(filter->GetOutput());
				int category_id = detection_dataset->catname2catid[category_name];
				const std::vector<double> bbox = BoundingBoxTo2DVector(boundingBox);
				// std::vector<double> rotation_bbox = GetMinimumRotationBox(filter->GetOutput());
				detection_dataset->add_annotation(image_name, category_id, category_name, bbox, bbox, 0);
			}
		}
	}
	return 0;
}