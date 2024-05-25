#ifndef COCO_DETECTION_DATA_H
#define COCO_DETECTION_DATA_H
#include <yaml-cpp/yaml.h>
#include <vector>


struct Image {
    int id;
    int width;
    int height;
    std::string file_name;
    std::string ct_name;
    std::string APorLA;
    std::vector<double> rotation;
    std::vector<double> translation;
};

struct Category {
    int id;
    std::string name;
    std::string supercategory;
};

struct Annotation {
    std::string mask_file_name;
    int id;
    int image_id;
    int category_id;
    std::string category_name;
    double area;
    std::vector<double> bbox;
    std::vector<double> segmentation;
    int iscrowd;
};


class COCODetectionData {
public:
    COCODetectionData(const YAML::Node& projection_parameter, const std::unordered_map<std::string, std::vector<std::vector<double>>>& rotations_and_translations); // 构造函数
    ~COCODetectionData(); // 析构函数
    // void add_image(const std::string& file_name, const std::string& ct_name, const std::string& APorLA, 
    //                int width, int height, const std::vector<double>& rotation, const std::vector<double>& translation);
    // void add_annotation(const std::string& mask_file_name, int category_id, const std::string& category_name, 
    //                     const std::vector<double>& bbox, const std::vector<double>& rotation_bbox, int iscrowd = 0);
    void add_categories();
    // void to_json(const std::string& save_path);
    // void load_json(const std::string& coco_annotations_file);


private:
    struct Info {
        std::string description;
        std::string url;
        std::string version;
        int year;
        std::string contributor;
        std::string date;
    } info;

    int annotation_num;
    int image_num;
    std::vector<Image> images;
    std::vector<Category> categories;
    std::vector<Annotation> annotations;
    std::unordered_map<std::string, std::vector<std::string>> exist_ct_nii_names;
    std::unordered_map<int, std::string> catid2catname;
    std::unordered_map<std::string, int> catname2catid;

};

#endif // COCO_DETECTION_DATA_H