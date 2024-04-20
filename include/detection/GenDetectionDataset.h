#ifndef GENDETECTIONDATASET_H
#define GENDETECTIONDATASET_H

#include <yaml-cpp/yaml.h>
#include <vector>
#include <string>

class GenDetectionDataset {
public:
    GenDetectionDataset(const YAML::Node& config); // 构造函数
    ~GenDetectionDataset(); // 析构函数
    void initializeVector(const YAML::Node& node, std::vector<std::vector<int>>& vec);

private:
    YAML::Node config; 
    float sdr;
    float height;
    float delx;
    float threshold;
    std::map<std::string, int> specific_height_list;
    int AP_num_samples;
    int LA_num_samples;
    std::string AP_bbox_label_type;
    std::string LA_bbox_label_type;
    std::string ct_root_path;
    std::string dataset_path;
    std::string dataset_images_path;
    std::string dataset_masks_path;
    std::string dataset_json_path;
    std::vector<std::vector<int>> AP_rot_range_list;
    std::vector<std::vector<int>> AP_trans_range_list;
    std::vector<std::vector<int>> LA_rot_range_list;
    std::vector<std::vector<int>> LA_trans_range_list;
    float min_bbox_percentage_of_height;
    AP_rotations, AP_translations
    LA_rotations, LA_translations
    rotations_and_translations
    COCODetectionData* detection_dataset;
    bool delete_mask;

    



};

#endif // GENDETECTIONDATASET_H