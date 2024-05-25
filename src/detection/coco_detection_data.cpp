// src/COCODetectionData.cpp

#include "coco_detection_data.h"
#include "file_process.h" // Assuming this header defines load_json_file and save_json_file
#include <ctime>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

COCODetectionData::COCODetectionData(const YAML::Node& projection_parameter, 
                                     const std::unordered_map<std::string, std::vector<std::vector<double>>>& rotations_and_translations) : annotation_num(0), image_num(0) {
    
    // 获取当前日期和时间
    std::time_t t = std::time(nullptr);
    std::tm* ltm = std::localtime(&t);

    // 定义 info 字典
    info = {
        {"dataset_info", {
            {"description", "Spine Detection DataSet"},
            {"url", "https://github.com/ThreeStones1029"},
            {"version", "1.0"},
            {"year", 1900 + ltm->tm_year},
            {"contributor", "ShuaiLei"},
            {"Date", get_current_date()}
        }},
        {"projection_parameter", yaml_node_to_nlohmann_json(projection_parameter)},
        {"rotations_and_translations", rotations_and_translations}
    };

    exist_ct_nii_names["AP"] = {};
    exist_ct_nii_names["LA"] = {};

    add_categories();
}

void COCODetectionData::add_image(const std::string& file_name, const std::string& ct_name, 
                                  const std::string& APorLA, int width, int height, 
                                  const std::vector<double>& rotation, 
                                  const std::vector<double>& translation) 
{
    ++image_num;
    nlohmann::json image = {
        {"id", image_num},
        {"width", width},
        {"height", height},
        {"file_name", file_name},
        {"ct_name", ct_name + ".nii.gz"},
        {"APorLA", APorLA},
        {"rotation", rotation},
        {"translation", translation}
    };
    images.push_back(image);
}

void COCODetectionData::add_categories() {
    categories = {
        {{"id", 1}, {"name", "L6"}, {"supercategory", "vertebrae"}},
        {{"id", 2}, {"name", "L5"}, {"supercategory", "vertebrae"}},
        {{"id", 3}, {"name", "L4"}, {"supercategory", "vertebrae"}},
        {{"id", 4}, {"name", "L3"}, {"supercategory", "vertebrae"}},
        {{"id", 5}, {"name", "L2"}, {"supercategory", "vertebrae"}},
        {{"id", 6}, {"name", "L1"}, {"supercategory", "vertebrae"}},
        {{"id", 7}, {"name", "T12"}, {"supercategory", "vertebrae"}},
        {{"id", 8}, {"name", "T11"}, {"supercategory", "vertebrae"}},
        {{"id", 9}, {"name", "T10"}, {"supercategory", "vertebrae"}},
        {{"id", 10}, {"name", "T9"}, {"supercategory", "vertebrae"}}
    };

    for (const auto& category : categories) {
        catid2catname[category["id"]] = category["name"];
        catname2catid[category["name"]] = category["id"];
    }
}

void COCODetectionData::add_annotation(const std::string& mask_file_name, int category_id, 
                                       const std::string& category_name, 
                                       const std::vector<double>& bbox, 
                                       const std::vector<double>& rotation_bbox, 
                                       int iscrowd) 
{
    ++annotation_num;
    nlohmann::json annotation = {
        {"mask_file_name", mask_file_name},
        {"id", annotation_num},
        {"image_id", image_num},
        {"category_id", category_id},
        {"category_name", category_name},
        {"area", bbox[2] * bbox[3]},
        {"bbox", bbox},
        {"segmentation", rotation_bbox},
        {"iscrowd", iscrowd}
    };
    annotations.push_back(annotation);
}

void COCODetectionData::to_json(const std::string& save_path) {
    nlohmann::json coco_data = {
        {"info", info},
        {"images", images},
        {"categories", categories},
        {"annotations", annotations}
    };

    save_json_file(coco_data, save_path);
}

void COCODetectionData::load_json(const std::string& coco_annotations_file) {
    nlohmann::json coco_data = load_json_file(coco_annotations_file);

    info = coco_data["info"];
    images = coco_data["images"];
    categories = coco_data["categories"];
    annotations = coco_data["annotations"];
    image_num = images.size();
    annotation_num = annotations.size();

    for (const auto& image_info : images) {
        std::string ct_name = image_info["ct_name"];
        std::string APorLA = image_info["APorLA"];
        if (APorLA == "AP" && 
            std::find(exist_ct_nii_names["AP"].begin(), exist_ct_nii_names["AP"].end(), ct_name) == exist_ct_nii_names["AP"].end()) 
        {
            exist_ct_nii_names["AP"].push_back(ct_name);
        } 
        else if (APorLA == "LA" && 
                 std::find(exist_ct_nii_names["LA"].begin(), exist_ct_nii_names["LA"].end(), ct_name) == exist_ct_nii_names["LA"].end()) 
        {
            exist_ct_nii_names["LA"].push_back(ct_name);
        }
    }
}

std::string COCODetectionData::get_current_date() {
    std::time_t t = std::time(nullptr);
    std::tm* ltm = std::localtime(&t);
    std::stringstream date_stream;
    date_stream << 1900 + ltm->tm_year << "-"
                << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon << "-"
                << std::setfill('0') << std::setw(2) << ltm->tm_mday;
    return date_stream.str();
}


COCODetectionData::~COCODetectionData() {
    // 析构函数的实现
}