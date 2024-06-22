/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-06-22 06:43:57
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-22 08:26:21
 */
#ifndef ITKGENDRR_H
#define ITKGENDRR_H

#include <vector>
#include <string>

int GenerateDrrs(const std::string& ct_file_path, const std::vector<std::vector<double>>& rotations, 
                                              const std::vector<std::vector<double>>& translations, bool save_img, double sid_value, 
                                              double sx_value, double sy_value, int dx_value, int dy_value, double threshold_value, 
                                              const std::string& APorLA, const std::string& save_images_folder);
#endif