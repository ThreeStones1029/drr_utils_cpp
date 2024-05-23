/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 02:57:42
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-23 07:42:53
 */
#include <iostream>
#include "file_process.h"
#include <yaml-cpp/yaml.h> 


void remove_all_files(const std::filesystem::path& dir_path) {
    for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
        if (std::filesystem::is_directory(entry)) {
            remove_all_files(entry.path()); // 递归删除子文件夹中的文件和文件夹
            std::filesystem::remove(entry.path());
        } else {
            std::filesystem::remove(entry); // 删除文件
        }
    }
}

void create_folder(const std::string& path) {
    try {
        std::filesystem::create_directory(path);
        std::cout << "Folder created: " << path << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create folder: " << e.what() << std::endl;
    }
}


std::vector<std::vector<int>> load_two_dimensional_array_from_yaml(const YAML::Node& node) {
    /*从yaml文件中读取二维数组*/
    std::vector<std::vector<int>> two_dimensional_array;
    if (node) {
        for (std::size_t i = 0; i < node.size(); ++i) {
            std::vector<int> range;
            for (std::size_t j = 0; j < node[i].size(); ++j) {
                int value = node[i][j].as<int>();
                range.push_back(value);
            }
            two_dimensional_array.push_back(range);
        }
    } else {
        std::cerr << "AP_rot_range_list not found in config file." << std::endl;
    }
    return two_dimensional_array;
}


std::unordered_map<std::string, int> load_map_from_yaml(const YAML::Node& node) {
    /*从yaml文件中读取字典*/ 
    std::unordered_map<std::string, int> map;
    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
        map[it->first.as<std::string>()] = it->second.as<int>();
    }
    return map;
}