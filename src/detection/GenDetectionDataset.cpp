/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 07:40:46
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-04-20 09:23:11
 */
#include "GenDetectionDataset.h"
#include "common.h"

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
    specific_height_list: Sometimes the generated image needs to be resized, so you can specify the size manually
    */
    this->config = config;
    this->sdr = config["projection_parameter"]["sdr"].as<float>();
    this->height = config["projection_parameter"]["height"].as<float>();
    this->delx = config["projection_parameter"]["delx"].as<float>();
    this->threshold = config["projection_parameter"]["threshold"].as<float>();
    const YAML::Node& specific_height_list_node = config["specific_height_list"];
    for (const auto& it : specific_height_list_node){
        specific_height_list[it.first.as<std::string>()] = it.second.as<int>();
    }
    this->AP_num_samples = config["projection_parameter"]["AP_num_samples"].as<int>();
    this->LA_num_samples = config["projection_parameter"]["LA_num_samples"].as<int>();
    this->AP_bbox_label_type = config["projection_parameter"]["AP_bbox_label_type"].as<std::string>();
    this->LA_bbox_label_type = config["projection_parameter"]["LA_bbox_label_type"].as<std::string>();
    this->ct_root_path = config["ct_root_path"].as<std::string>();
    this->dataset_path = config["dataset_path"].as<std::string>();
    this->dataset_images_path = config["dataset_images_path"].as<std::string>();
    this->dataset_masks_path = config["dataset_masks_path"].as<std::string>();
    this->dataset_json_path = config["dataset_json_path"].as<std::string>();

    initializeVector(config["projection_parameter"]["AP_rot_range_list"], AP_rot_range_list);
    initializeVector(config["projection_parameter"]["AP_trans_range_list"], AP_trans_range_list);
    initializeVector(config["projection_parameter"]["LA_rot_range_list"], LA_rot_range_list);
    initializeVector(config["projection_parameter"]["LA_trans_range_list"], LA_trans_range_list);
    //this->specific_height_list = config["projection_parameter"]["height"].as<float>();
    this->min_bbox_percentage_of_height = config["projection_parameter"]["height"].as<float>();
    
    self.AP_rotations, self.AP_translations = self.gen_random_pose_parameters(self.AP_rot_range_list, self.AP_trans_range_list, self.AP_num_samples)
    self.LA_rotations, self.LA_translations = self.gen_random_pose_parameters(self.LA_rot_range_list, self.LA_trans_range_list, self.LA_num_samples)
    self.rotations_and_translations = {"AP_rotations": self.AP_rotations, "AP_translations": self.AP_translations,
                                        "LA_rotations": self.LA_rotations, "LA_translations": self.LA_translations}
    self.detection_dataset = COCODetectionData(config.projection_parameter, self.rotations_and_translations)
    
    this->delete_mask = (this->AP_num_samples + this->LA_num_samples >= 2000) ? ture : false;
    // create save folder
    create_folder(self.dataset_path);
    create_folder(self.dataset_images_path);
    create_folder(self.dataset_masks_path);

}

GenDetectionDataset::~GenDetectionDataset() {
    // 析构函数的实现
}

void GenDetectionDataset::initializeVector(const YAML::Node& node, std::vector<std::vector<int>>& vec) {
    for (const auto& item : node) {
        vec.push_back({item[0].as<int>(), item[1].as<int>()});
    }
}