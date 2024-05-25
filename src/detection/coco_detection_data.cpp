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

    // 定义 projection_parameter 和 rotations_and_translations 示例数据
    nlohmann::json projection_parameter = {
        {"param1", "value1"},
        {"param2", "value2"}
    };

    nlohmann::json rotations_and_translations = {
        {"rotation1", {1, 2, 3}},
        {"translation1", {4, 5, 6}}
    };

    // 定义 info 字典
    nlohmann::json info = {
        {"dataset_info", {
            {"description", "Spine Detection DataSet"},
            {"url", "https://github.com/ThreeStones1029"},
            {"version", "1.0"},
            {"year", 1900 + ltm->tm_year},
            {"contributor", "ShuaiLei"},
            {"Date", std::to_string(1900 + ltm->tm_year) + "-" +
                     std::to_string(1 + ltm->tm_mon) + "-" +
                     std::to_string(ltm->tm_mday)}
        }},
        {"projection_parameter", projection_parameter},
        {"rotations_and_translations", rotations_and_translations}
    };


    // 将 JSON 对象保存为文件
    save_json_file(info, "info.json");
    add_categories();
}

// void COCODetectionData::add_image(const std::string& file_name, const std::string& ct_name, const std::string& APorLA, 
//                                   int width, int height, const std::vector<double>& rotation, const std::vector<double>& translation) {
//     image_num++;
//     images.push_back({image_num, width, height, file_name, ct_name + ".nii.gz", APorLA, rotation, translation});
// }

void COCODetectionData::add_categories() {
    categories = {{1, "L6", "vertebrae"}, 
                  {2, "L5", "vertebrae"}, 
                  {3, "L4", "vertebrae"}, 
                  {4, "L3", "vertebrae"},
                  {5, "L2", "vertebrae"}, 
                  {6, "L1", "vertebrae"}, 
                  {7, "T12", "vertebrae"}, 
                  {8, "T11", "vertebrae"},
                  {9, "T10", "vertebrae"}, 
                  {10, "T9", "vertebrae"}};
    
    for (const auto& category : categories) {
        catid2catname[category.id] = category.name;
        catname2catid[category.name] = category.id;
    }
}

// void COCODetectionData::add_annotation(const std::string& mask_file_name, int category_id, const std::string& category_name, 
//                                        const std::vector<double>& bbox, const std::vector<double>& rotation_bbox, int iscrowd) {
//     annotation_num++;
//     annotations.push_back({mask_file_name, annotation_num, image_num, category_id, category_name, 
//                            bbox[2] * bbox[3], bbox, rotation_bbox, iscrowd});
// }

// void COCODetectionData::to_json(const std::string& save_path) {
//     nlohmann::json coco_data;
//     coco_data["info"] = {{"description", info.description}, {"url", info.url}, {"version", info.version}, 
//                          {"year", info.year}, {"contributor", info.contributor}, {"date", info.date}};
    
//     coco_data["images"] = images;
//     coco_data["categories"] = categories;
//     coco_data["annotations"] = annotations;
    
//     save_json_file(coco_data, save_path);
// }

// void COCODetectionData::load_json(const std::string& coco_annotations_file) {
//     nlohmann::json coco_data = load_json_file(coco_annotations_file);
//     info.description = coco_data["info"]["description"];
//     info.url = coco_data["info"]["url"];
//     info.version = coco_data["info"]["version"];
//     info.year = coco_data["info"]["year"];
//     info.contributor = coco_data["info"]["contributor"];
//     info.date = coco_data["info"]["date"];
    
//     images = coco_data["images"].get<std::vector<Image>>();
//     categories = coco_data["categories"].get<std::vector<Category>>();
//     annotations = coco_data["annotations"].get<std::vector<Annotation>>();
    
//     image_num = images.size();
//     annotation_num = annotations.size();
    
//     for (const auto& image_info : images) {
//         if (image_info.APorLA == "AP" && 
//             std::find(exist_ct_nii_names["AP"].begin(), exist_ct_nii_names["AP"].end(), image_info.ct_name) == exist_ct_nii_names["AP"].end()) {
//             exist_ct_nii_names["AP"].push_back(image_info.ct_name);
//         }
//         if (image_info.APorLA == "LA" && 
//             std::find(exist_ct_nii_names["LA"].begin(), exist_ct_nii_names["LA"].end(), image_info.ct_name) == exist_ct_nii_names["LA"].end()) {
//             exist_ct_nii_names["LA"].push_back(image_info.ct_name);
//         }
//     }
// }

COCODetectionData::~COCODetectionData() {
    // 析构函数的实现
}




