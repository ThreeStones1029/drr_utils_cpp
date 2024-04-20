/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 03:00:34
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-04-20 08:46:07
 */
#ifndef COMMON_H
#define COMMON_H

#include <filesystem>
#include <string>

void remove_all_files(const std::filesystem::path& dir_path);

void create_folder(const std::string & path);

#endif