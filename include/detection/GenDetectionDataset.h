/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 07:41:10
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-25 15:04:32
 */
#ifndef GENDETECTIONDATASET_H
#define GENDETECTIONDATASET_H

#include <yaml-cpp/yaml.h>
#include <vector>
#include <string>
#include <coco_detection_data.h>


class GenDetectionDataset {
public:
    GenDetectionDataset(const YAML::Node& config); // 构造函数
    ~GenDetectionDataset(); // 析构函数
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> gen_random_pose_parameters(const std::vector<std::vector<int>> rot_range_list, 
                                                                                                              const std::vector<std::vector<int>> trans_range_list, 
                                                                                                              int num_samples);
    void gen_multple_cts_drrs_and_masks();
    void check_sub_folders(const std::vector<std::string>& sub_folder_paths);
    void gen_AP_drrs_and_masks(const std::string& ct_path, const std::string& bbox_label_type);
    void gen_LA_drrs_and_masks(const std::string& ct_path, const std::string& bbox_label_type);
    void gen_drr(const std::string& ct_name, int i, const std::vector<double>& rotation,
                 const std::vector<double>& translation, const std::string& filepath, const std::string& view);
    void gen_mask(const std::string& basename_wo_ext, const std::string& ct_name, int i,
                  const std::vector<double>& rotation, const std::vector<double>& translation,
                  const std::string& filepath, const std::string& view);

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
    std::unique_ptr<COCODetectionData> detection_dataset; // 使用智能指针
    bool delete_mask;



};

#endif // GENDETECTIONDATASET_H