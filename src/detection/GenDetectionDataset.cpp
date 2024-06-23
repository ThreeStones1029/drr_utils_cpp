/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 07:40:46
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-22 14:04:18
 */
#include "GenDetectionDataset.h"
#include "coco_detection_data.h"
#include "file_process.h"
#include "dataset_sample.h"
#include "debug_print.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include "ITKGenDrr.h"


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


void GenDetectionDataset::gen_multple_cts_drrs_and_masks() {
    auto total_start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::string> ct_path_list = getSubFolderPaths(ct_root_path);
    for (const auto& single_ct_path : ct_path_list) {
        std::string ct_name = std::filesystem::path(single_ct_path).filename().string();
        if (std::filesystem::exists(dataset_json_path)) {
            nlohmann::json coco_data = detection_dataset->load_json(dataset_json_path);
            AP_rotations = coco_data["info"]["rotations_and_translations"]["AP_rotations"];
            AP_translations = coco_data["info"]["rotations_and_translations"]["AP_translations"];
            LA_rotations = coco_data["info"]["rotations_and_translations"]["LA_rotations"];
            LA_translations = coco_data["info"]["rotations_and_translations"]["LA_translations"];
        }
        /*------------------------------------------------------------------------------------------------------------------- 
        # Note if you want regenerate completely,just add "-r all", Otherwise, it will automatically read the existing json 
        # file and only generate data for ct that is not in the json file #
        -------------------------------------------------------------------------------------------------------------------*/
        std::string ct_name_has_ext = ct_name + ".nii.gz";
        if (std::find(detection_dataset->exist_ct_nii_names["AP"].begin(), detection_dataset->exist_ct_nii_names["AP"].end(), ct_name_has_ext) == detection_dataset->exist_ct_nii_names["AP"].end() &&
            std::find(detection_dataset->exist_ct_nii_names["LA"].begin(), detection_dataset->exist_ct_nii_names["LA"].end(), ct_name_has_ext) == detection_dataset->exist_ct_nii_names["LA"].end()) {
            if (specific_height_list.find(ct_name) != specific_height_list.end()) {
                height = specific_height_list[ct_name];
            } 
            else {
                height = config["projection_parameter"]["height"].as<float>();
            } 
            gen_drrs_and_masks(single_ct_path);  // Replace with actual bbox label type
            detection_dataset->to_json(dataset_json_path);
        }
    }
}

void GenDetectionDataset::check_sub_folders(const std::vector<std::string>& sub_folder_paths) {
    for (const auto& sub_folder_path : sub_folder_paths) {
        std::filesystem::path basename = std::filesystem::path(sub_folder_path).filename();
        std::filesystem::path nii_path = std::filesystem::path(sub_folder_path) / (basename.string() + ".nii.gz");

        if (!std::filesystem::exists(nii_path)) {
            throw std::runtime_error("file '" + nii_path.string() + "' not exist");
        }
    }
    std::cout << "check completely!" << std::endl;
}


void GenDetectionDataset::gen_drrs_and_masks(const std::string& ct_path) {
    // 获取CT名称
    std::string ct_name = std::filesystem::path(ct_path).filename().string();
    std::string ct_filepath = std::filesystem::path(ct_path) / (ct_name + ".nii.gz");
    // if AP_bbox_label_type is small, it will generate small bbox only according vertebrae body.
    std::vector<std::string> seg_filepaths;
    if (AP_bbox_label_type == "small")
        seg_filepaths = getFilesWithEnding(ct_path, "body_seg.nii.gz");
    // if AP_bbox_label_type is big, it will generate big bbox according overall vertebrae.
    if (AP_bbox_label_type == "big")
        seg_filepaths = getFilteredFiles(ct_path, "seg.nii.gz", "body_seg.nii.gz");
    // 同时传入所有seg_filepaths
    GenerateDrrs(ct_filepath, AP_rotations, AP_translations, true, sdr*2, delx, delx, height, height, threshold, "AP", dataset_images_path, detection_dataset);
    GenerateMasks(ct_name, seg_filepaths, AP_rotations, AP_translations, false, sdr*2, delx, delx, height, height, threshold, "AP", dataset_masks_path, detection_dataset);
    GenerateDrrs(ct_filepath, LA_rotations, LA_translations, true, sdr*2, delx, delx, height, height, threshold, "LA", dataset_images_path, detection_dataset);
    GenerateMasks(ct_name, seg_filepaths, LA_rotations, LA_translations, false, sdr*2, delx, delx, height, height, threshold, "LA", dataset_masks_path, detection_dataset);
}
