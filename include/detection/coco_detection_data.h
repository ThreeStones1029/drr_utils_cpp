#ifndef COCO_DETECTION_DATA_H
#define COCO_DETECTION_DATA_H
#include <yaml-cpp/yaml.h>
#include <vector>
#include <nlohmann/json.hpp>


class COCODetectionData {
public:
    COCODetectionData(const YAML::Node& projection_parameter, const std::unordered_map<std::string, std::vector<std::vector<double>>>& rotations_and_translations); // 构造函数
    ~COCODetectionData(); // 析构函数
    void add_image(const std::string& file_name, const std::string& ct_name, 
                   const std::string& APorLA, int width, int height, 
                   const std::vector<double>& rotation, 
                   const std::vector<double>& translation);

    void add_categories();

    void add_annotation(const std::string& mask_file_name, int category_id, 
                        const std::string& category_name, 
                        const std::vector<double>& bbox, 
                        const std::vector<double>& rotation_bbox, 
                        int iscrowd);

    void to_json(const std::string& save_path);

    nlohmann::json load_json(const std::string& coco_annotations_file);

    std::string get_current_date();
    std::unordered_map<std::string, std::vector<std::string>> exist_ct_nii_names;
    std::unordered_map<int, std::string> catid2catname;
    std::unordered_map<std::string, int> catname2catid;


private:
    nlohmann::json info;
    int annotation_num;
    int image_num;
    std::vector<nlohmann::json> images;
    std::vector<nlohmann::json> categories;
    std::vector<nlohmann::json> annotations;
};

#endif // COCO_DETECTION_DATA_H