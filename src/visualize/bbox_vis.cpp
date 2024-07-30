/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-07-29 02:12:44
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-07-30 01:52:17
 */
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include "bbox_vis.h"
#include "file_process.h"


VisCoCo::VisCoCo(const std::string& annotation_file, const std::string& images_folder, const std::string& bbox_vis_folder, const std::string& rotate_bbox_vis_folder) 
        : COCO(annotation_file), images_folder(images_folder), bbox_vis_folder(bbox_vis_folder), rotate_bbox_vis_folder(rotate_bbox_vis_folder) {

        create_folder(bbox_vis_folder);
        create_folder(rotate_bbox_vis_folder);

        for (const auto& img : dataset["images"]) {
            file_name2img_id[img["file_name"]] = img["id"];
            img_id2file_name[img["id"]] = img["file_name"];
        }
        for (const auto& cat : dataset["categories"]) {
            categories_id2name[cat["id"]] = cat["name"];
        }
    }

void VisCoCo::visualize_bboxes_in_images() {
    std::vector<std::thread> threads;
    for (const auto& entry : std::filesystem::directory_iterator(images_folder)) {
        std::string file_path = entry.path().string();
        std::string file_name = entry.path().filename().string();
        if (file_name2img_id.find(file_name) != file_name2img_id.end()) {
            threads.emplace_back(&VisCoCo::visualize_bboxes_in_image, this, file_path);
        }
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

void VisCoCo::visualize_bboxes_in_image(const std::string& file_path) {
    std::string file_name = std::filesystem::path(file_path).filename().string();
    int image_id = file_name2img_id[file_name];
    std::string save_image_path = bbox_vis_folder + "/" + file_name;
    cv::Mat image = cv::imread(file_path);
    auto annotations = imgToAnns[image_id];
    draw_bbox(image, annotations);
    cv::imwrite(save_image_path, image);
}


void VisCoCo::draw_bbox(cv::Mat& image, const std::vector<nlohmann::json>& annotations) {
    for (const auto& ann : annotations) {
        auto bbox = ann["bbox"];
        int xmin = bbox[0];
        int ymin = bbox[1];
        int width = bbox[2];
        int height = bbox[3];
        int xmax = xmin + width;
        int ymax = ymin + height;
        cv::rectangle(image, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 0, 255), 2);
        
        int catid = ann["category_id"];
        std::string text = categories_id2name[catid];
        int baseLine;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        ymin = std::max(ymin, label_size.height);
        cv::rectangle(image, cv::Point(xmin, ymin - label_size.height), cv::Point(xmin + label_size.width, ymin + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
        cv::putText(image, text, cv::Point(xmin, ymin), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    }
}