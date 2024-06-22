/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 03:00:34
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-22 13:08:17
 */
#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H

#include <filesystem>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>


void remove_all_files(const std::filesystem::path& dir_path);

void create_folder(const std::string & path);

std::vector<std::vector<int>> load_two_dimensional_array_from_yaml(const YAML::Node& node);

std::unordered_map<std::string, int> load_map_from_yaml(const YAML::Node& node);

nlohmann::json load_json_file(const std::string& file_path);

void save_json_file(const nlohmann::json& data, const std::string& file_path);

nlohmann::json yaml_node_to_nlohmann_json(const YAML::Node& yaml_node);

std::vector<std::string> getSubFolderPaths(const std::string& root_path);

std::vector<std::string> getFilesWithExtension(const std::string& folderPath, const std::string& extension);

std::vector<std::string> getFilesWithEnding(const std::string& folderPath, const std::string& ending);

std::vector<std::string> getFilteredFiles(const std::string& folderPath, const std::string& includeEnding, const std::string& excludeEnding);

std::vector<std::string> splitString(const std::string& str, char delimiter);
#endif