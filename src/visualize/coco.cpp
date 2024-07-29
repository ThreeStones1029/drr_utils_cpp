/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-07-29 02:30:33
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-07-29 15:05:47
 */
// coco.cpp

#include "coco.h"


COCO::COCO(const std::string& annotation_file) {
    std::ifstream file(annotation_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + annotation_file);
    }
    file >> dataset;
    file.close();
    createIndex();
}

void COCO::createIndex() {
    for (const auto& ann : dataset["annotations"]) {
        int image_id = ann["image_id"];
        int category_id = ann["category_id"];
        anns[ann["id"]] = ann;
        imgToAnns[image_id].push_back(ann);
    }

    for (const auto& img : dataset["images"]) {
        imgs[img["id"]] = img;
    }

    for (const auto& cat : dataset["categories"]) {
        cats[cat["id"]] = cat;
    }
    std::cout << "Create index successfully!" << std::endl;
}
