#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkNIFTIImageReader.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkImageActor.h>
#include <vtkNamedColors.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>


class CustomInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static CustomInteractorStyle* New();
    vtkTypeMacro(CustomInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void Rotate()
    {
        if (this->CurrentRenderer == nullptr)
        {
            return;
        }

        vtkRenderWindowInteractor* rwi = this->Interactor;

        int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
        int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

        int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();
        double delta_elevation = -20.0 / size[1];
        double delta_azimuth = -20.0 / size[0];

        // 反转旋转方向
        double rxf = -dx * delta_azimuth * this->MotionFactor;
        double ryf = -dy * delta_elevation * this->MotionFactor;

        vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
        camera->Azimuth(rxf);
        camera->Elevation(ryf);
        camera->OrthogonalizeViewUp();

        if (this->AutoAdjustCameraClippingRange)
        {
            this->CurrentRenderer->ResetCameraClippingRange();
        }

        if (rwi->GetLightFollowCamera())
        {
            this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
        }

        rwi->Render();
    }
};

vtkStandardNewMacro(CustomInteractorStyle);

int main(int argc, char* argv[])
{
    // // 创建渲染器
    // vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    // renderer->SetBackground(0.7, 0.85, 1.0); // 设置背景色

    // // 创建渲染窗口
    // vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    // renderWindow->AddRenderer(renderer);
    // renderWindow->SetSize(800, 600); // 设置窗口大小

    // // 创建渲染窗口交互器
    // vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    // renderWindowInteractor->SetRenderWindow(renderWindow);

    // // 创建自定义交互器样式
    // vtkSmartPointer<CustomInteractorStyle> style = vtkSmartPointer<CustomInteractorStyle>::New();
    // renderWindowInteractor->SetInteractorStyle(style);

    // // 创建球体几何对象
    // vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    // sphereSource->SetRadius(5.0); // 设置球体半径

    // // 创建球体映射器
    // vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    // // 创建球体演员
    // vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    // sphereActor->SetMapper(sphereMapper); // 设置演员的映射器
    // sphereActor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 设置球体颜色为红色

    // 创建NIfTI图像读取器
    vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
    reader->SetFileName("../../data/verse2020_test/verse004/verse004.nii.gz");
    reader->Update();

    // 获取读取的图像数据
    vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();

    // 创建颜色传递函数
    vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    colorTransferFunction->AddRGBPoint(-3024, 0, 0, 0);
    colorTransferFunction->AddRGBPoint(-77, 0.5, 0.5, 0.5);
    colorTransferFunction->AddRGBPoint(94, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(179, 1.0, 0.5, 0.3);
    colorTransferFunction->AddRGBPoint(260, 1.0, 0.5, 0.3);
    colorTransferFunction->AddRGBPoint(3071, 1.0, 1.0, 1.0);

    // 创建不透明度传递函数
    vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityTransferFunction->AddPoint(-3024, 0.0);
    opacityTransferFunction->AddPoint(-77, 0.0);
    opacityTransferFunction->AddPoint(94, 0.29);
    opacityTransferFunction->AddPoint(179, 0.55);
    opacityTransferFunction->AddPoint(260, 0.84);
    opacityTransferFunction->AddPoint(3071, 0.87);

    // 创建体积属性
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();

    // 创建体积映射器
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetInputData(imageData);

    // 创建体积
    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    // 创建渲染器
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

    // 添加体积到渲染器
    renderer->AddVolume(volume);

    // 设置相机
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition(0, -1, 0);
    camera->SetViewUp(0, 0, -1);
    renderer->SetActiveCamera(camera);
    renderer->ResetCamera();

    // 创建渲染窗口
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // 创建渲染窗口交互器
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // // 创建自定义交互器样式
    // vtkSmartPointer<CustomInteractorStyle> style = vtkSmartPointer<CustomInteractorStyle>::New();
    // renderWindowInteractor->SetInteractorStyle(style);

    // 开始渲染和交互
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
