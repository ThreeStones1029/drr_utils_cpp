/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 02:57:42
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-04-20 08:47:14
 */
#include <iostream>
#include "common.h"


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