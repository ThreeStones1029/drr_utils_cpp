/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-07-29 02:30:43
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-07-29 15:05:56
 */
// coco.h

#ifndef COCO_H
#define COCO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "nlohmann/json.hpp"  // Include JSON library

class COCO {
public:
    COCO(const std::string& annotation_file);
    void createIndex();

protected:
    nlohmann::json dataset;
    std::unordered_map<int, nlohmann::json> anns;
    std::unordered_map<int, nlohmann::json> imgs;
    std::unordered_map<int, nlohmann::json> cats;
    std::unordered_map<int, std::vector<nlohmann::json>> imgToAnns;
};

#endif // COCO_H
