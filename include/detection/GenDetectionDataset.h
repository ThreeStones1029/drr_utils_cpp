/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 07:41:10
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-23 13:29:29
 */
#ifndef GENDETECTIONDATASET_H
#define GENDETECTIONDATASET_H

#include <yaml-cpp/yaml.h>
#include <vector>
#include <string>


class GenDetectionDataset {
public:
    GenDetectionDataset(const YAML::Node& config); // 构造函数
    ~GenDetectionDataset(); // 析构函数
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> gen_random_pose_parameters(const std::vector<std::vector<int>> rot_range_list, 
                                                                                                              const std::vector<std::vector<int>> trans_range_list, 
                                                                                                              int num_samples);

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
    std::unordered_map<std::string, int> specific_heights_map;
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> AP_rotations_and_translations;
    std::vector<std::vector<double>> AP_rotations;
    std::vector<std::vector<double>> AP_translations;
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> LA_rotations_and_translations;
    std::vector<std::vector<double>> LA_rotations;
    std::vector<std::vector<double>> LA_translations;
    std::unordered_map<std::string, std::vector<std::vector<double>>> rotations_and_translations;
    // rotations_and_translations
    // COCODetectionData* detection_dataset;
    bool delete_mask;



};

#endif // GENDETECTIONDATASET_H