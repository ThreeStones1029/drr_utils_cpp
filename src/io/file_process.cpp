/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 02:57:42
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-22 13:07:45
 */
#include <iostream>
#include <fstream>
#include "file_process.h"
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <filesystem>


/**
 * @brief create folder.
 * @param dir_path root folder.                                      
 * @return
 */
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

/**
 * @brief create folder.
 * @param path 
 * @return
 */
void create_folder(const std::string& path) {
    try {
        std::filesystem::create_directory(path);
        std::cout << "Folder created: " << path << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create folder: " << e.what() << std::endl;
    }
}

/**
 * @brief load two dimensional array.
 * @param node 
 * @return two dimensional array.
 */
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
        std::cerr << "node not found in config file." << std::endl;
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


nlohmann::json load_json_file(const std::string& file_path) {
    std::ifstream input_file(file_path);
    nlohmann::json json_data;

    if (input_file.is_open()) {
        input_file >> json_data;
        input_file.close();
    } else {
        std::cerr << "can not open file: " << file_path << std::endl;
    }

    return json_data;
}

void save_json_file(const nlohmann::json& data, const std::string& file_path) {
    std::ofstream output_file(file_path);

    if (output_file.is_open()) {
        output_file << data.dump(4); // Pretty print with 4 spaces indentation
        output_file.close();
    } else {
        std::cerr << "can not open file: " << file_path << std::endl;
    }
}


nlohmann::json yaml_node_to_nlohmann_json(const YAML::Node& yaml_node) {
    /*
    将yaml node转为 nlohman json格式
    */
    nlohmann::json json_node;
    switch (yaml_node.Type()) {
        case YAML::NodeType::Null:
            json_node = nullptr;
            break;
        case YAML::NodeType::Scalar: //yaml_node 是一个标量（单个值）
            json_node = yaml_node.as<std::string>();
            break;
        case YAML::NodeType::Sequence: //yaml_node 是一个序列（数组），则遍历该序列的每一个元素，递归调用
            for (std::size_t i = 0; i < yaml_node.size(); ++i) {
                json_node.push_back(yaml_node_to_nlohmann_json(yaml_node[i]));
            }
            break;
        case YAML::NodeType::Map: // yaml_node 是一个映射（键值对），则遍历该映射的每一个键值对，递归调用 yaml_to_json 函数将值转换为 JSON
            for (YAML::const_iterator it = yaml_node.begin(); it != yaml_node.end(); ++it) {
                json_node[it->first.as<std::string>()] = yaml_node_to_nlohmann_json(it->second);
            }
            break;
        default:
            throw std::runtime_error("Unknown YAML node type");
    }

    return json_node;
}

std::vector<std::string> getSubFolderPaths(const std::string& root_path) {
    std::vector<std::string> sub_folders;
    for (const auto& entry : std::filesystem::directory_iterator(root_path)) {
        if (entry.is_directory()) {
            sub_folders.push_back(entry.path().string());
        }
    }
    return sub_folders;
}


std::vector<std::string> getFilesWithExtension(const std::string& folderPath, const std::string& extension) {
    std::vector<std::string>  files;
    // 遍历指定目录下的所有文件和子目录
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            // 如果是文件且后缀匹配，则将其路径添加到结果向量中
            files.push_back(entry.path().string());
        }
    }
    return files;
}


std::vector<std::string> getFilesWithEnding(const std::string& folderPath, const std::string& ending) {
    std::vector<std::string> files;
    // 遍历指定目录下的所有文件和子目录
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            // 检查文件名是否以指定字符串结尾
            if (filename.size() >= ending.size() && filename.compare(filename.size() - ending.size(), ending.size(), ending) == 0) {
                // 如果匹配，则将其路径添加到结果向量中
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}


std::vector<std::string> getFilteredFiles(const std::string& folderPath, const std::string& includeEnding, const std::string& excludeEnding) {
    std::vector<std::string> files;
    // 遍历指定目录下的所有文件和子目录
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            // 检查文件名是否以包含字符串结尾
            if (filename.size() >= includeEnding.size() && 
                filename.compare(filename.size() - includeEnding.size(), includeEnding.size(), includeEnding) == 0) {
                // 如果需要排除特定结尾的文件
                if (!excludeEnding.empty() && filename.size() >= excludeEnding.size() &&
                    filename.compare(filename.size() - excludeEnding.size(), excludeEnding.size(), excludeEnding) == 0) {
                    continue; // 跳过匹配排除字符串的文件
                }
                // 如果匹配，则将其路径添加到结果向量中
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}


std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delimiter)) {
        result.push_back(part);
    }
    return result;
}