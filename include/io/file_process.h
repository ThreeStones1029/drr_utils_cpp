/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 03:00:34
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-23 08:14:51
 */
#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H

#include <filesystem>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

void remove_all_files(const std::filesystem::path& dir_path);

void create_folder(const std::string & path);

std::vector<std::vector<int>> load_two_dimensional_array_from_yaml(const YAML::Node& node);

std::unordered_map<std::string, int> load_map_from_yaml(const YAML::Node& node);

#endif