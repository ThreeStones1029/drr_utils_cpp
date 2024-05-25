/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 07:40:46
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-25 13:02:24
 */
#include "GenDetectionDataset.h"
#include "coco_detection_data.h"
#include "file_process.h"
#include "local_format.h"
#include "dataset_sample.h"
#include "debug_print.h"
#include "nlohmann/json.hpp"


GenDetectionDataset::GenDetectionDataset(const YAML::Node& config) {
    /*
    Constructor implementation
    params
    sdr: Half of the distance from the ray source to the projection plane
    height: Image size
    delx: Image pixel distance
    threshold: ITK projection threshold
    AP_num_samples: Number of AP generated per nii file
    LA_num_samples: Number of LA generated per nii file
    AP_bbox_label_type: AP Annotation method big or small
    LA_bbox_label_type: LA Annotation method big or small
    ct_root_path: The root path of ct
    dataset_path: save folder path
    dataset_json_path: json file generated
    AP_rot_range_list: AP Range of Angle
    AP_trans_range_list: AP Range of movement
    LA_rot_range_list: LA Range of Angle
    LA_trans_range_list: LA Range of movement
    min_bbox_percentage_of_height: The minimum percentage of the bbox relative to the picture 
    AP_rotations, AP_translations: The range of AP angles generated, The range of AP movement generated
    LA_rotations, LA_translations: The range of LA angles generated, The range of LA movement generated
    detection_dataset: CoCo format json
    delete_mask: Remove mask when disk runs out. mask takes up about 6-7 times the space of images 
    specific_heights_map: Sometimes the generated image needs to be resized, so you can specify the size manually
    */
    this->config = config;
    this->sdr = config["projection_parameter"]["sdr"].as<float>();
    this->height = config["projection_parameter"]["height"].as<float>();
    this->delx = config["projection_parameter"]["delx"].as<float>();
    this->threshold = config["projection_parameter"]["threshold"].as<float>();
    this->AP_num_samples = config["projection_parameter"]["AP_num_samples"].as<int>();
    this->LA_num_samples = config["projection_parameter"]["LA_num_samples"].as<int>();
    this->AP_bbox_label_type = config["projection_parameter"]["AP_bbox_label_type"].as<std::string>();
    this->LA_bbox_label_type = config["projection_parameter"]["LA_bbox_label_type"].as<std::string>();
    this->ct_root_path = config["ct_root_path"].as<std::string>();
    this->dataset_path = config["dataset_path"].as<std::string>();
    this->dataset_images_path = config["dataset_images_path"].as<std::string>();
    this->dataset_masks_path = config["dataset_masks_path"].as<std::string>();
    this->dataset_json_path = config["dataset_json_path"].as<std::string>();
    this->AP_rot_range_list = load_two_dimensional_array_from_yaml(config["projection_parameter"]["AP_rot_range_list"]);
    this->AP_trans_range_list = load_two_dimensional_array_from_yaml(config["projection_parameter"]["AP_trans_range_list"]);
    this->LA_rot_range_list = load_two_dimensional_array_from_yaml(config["projection_parameter"]["LA_rot_range_list"]);
    this->LA_trans_range_list = load_two_dimensional_array_from_yaml(config["projection_parameter"]["LA_trans_range_list"]);
    this->specific_heights_map = load_map_from_yaml(config["projection_parameter"]["specific_height_list"]);
    this->min_bbox_percentage_of_height = config["projection_parameter"]["height"].as<float>();
    
    this->AP_rotations_and_translations = gen_random_pose_parameters(this->AP_rot_range_list, this->AP_trans_range_list, this->AP_num_samples);
    this->AP_rotations = std::get<0>(this->AP_rotations_and_translations);
    this->AP_translations = std::get<1>(this->AP_rotations_and_translations);
    this->LA_rotations_and_translations = gen_random_pose_parameters(this->LA_rot_range_list, this->LA_trans_range_list, this->LA_num_samples);
    this->LA_rotations = std::get<0>(this->LA_rotations_and_translations);
    this->LA_translations = std::get<1>(this->LA_rotations_and_translations);
    this->rotations_and_translations["AP_rotations"] = this->AP_rotations;
    this->rotations_and_translations["AP_translations"] = this->AP_translations;
    this->rotations_and_translations["LA_rotations"] = this->LA_rotations;
    this->rotations_and_translations["LA_translations"] = this->LA_translations;
    this->detection_dataset = std::make_unique<COCODetectionData>(config["projection_parameter"], rotations_and_translations); // 使用智能指针
    detection_dataset->to_json(this->dataset_json_path);
    this->delete_mask = (this->AP_num_samples + this->LA_num_samples >= 2000) ? true : false;
    // create save folder
    create_folder(this->dataset_path);
    create_folder(this->dataset_images_path);
    create_folder(this->dataset_masks_path);
}

GenDetectionDataset::~GenDetectionDataset() {
    // 析构函数的实现
}

std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> GenDetectionDataset::gen_random_pose_parameters(const std::vector<std::vector<int>> rot_range_list, 
                                                                                                                               const std::vector<std::vector<int>> trans_range_list, 
                                                                                                                               int num_samples) {
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> rotations_and_translations;
    DatasetSample data_rot_and_trans;
    rotations_and_translations = data_rot_and_trans.Monte_Carlo_sample_dataset(rot_range_list,trans_range_list, num_samples);
    // rotations_and_translations = data_rot_and_trans.generate_uniform_samples_grid(rot_range_list,trans_range_list, num_samples);
    return rotations_and_translations;
}
