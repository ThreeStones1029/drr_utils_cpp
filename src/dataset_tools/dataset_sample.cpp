/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 14:32:34
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-06-21 14:33:03
 */
#include "dataset_sample.h"
#include <iostream>
#include <random>
#include <cmath>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;

DatasetSample::DatasetSample() {}

// Sample uniformly according to the grid
std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> DatasetSample::generate_uniform_samples_grid(
    const std::vector<std::vector<int>>& rot_range_list,
    const std::vector<std::vector<int>>& trans_range_list,
    int num_samples) {
    double rot_x_min = rot_range_list[0][0], rot_x_max = rot_range_list[0][1];
    double rot_y_min = rot_range_list[1][0], rot_y_max = rot_range_list[1][1];
    double rot_z_min = rot_range_list[2][0], rot_z_max = rot_range_list[2][1];
    double tran_x_min = trans_range_list[0][0], tran_x_max = trans_range_list[0][1];
    double tran_y_min = trans_range_list[1][0], tran_y_max = trans_range_list[1][1];
    double tran_z_min = trans_range_list[2][0], tran_z_max = trans_range_list[2][1];

    int num_samples_per_axis = std::ceil(std::pow(num_samples, 1.0 / 3.0));
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> rotations_and_translations;
    std::vector<std::vector<double>> rotations;
    std::vector<std::vector<double>> translations;

    for (int i = 0; i < num_samples_per_axis; ++i) {
        for (int j = 0; j < num_samples_per_axis; ++j) {
            for (int k = 0; k < num_samples_per_axis; ++k) {
                double rot_x = rot_x_min + i * (rot_x_max - rot_x_min) / num_samples_per_axis;
                double rot_y = rot_y_min + j * (rot_y_max - rot_y_min) / num_samples_per_axis;
                double rot_z = rot_z_min + k * (rot_z_max - rot_z_min) / num_samples_per_axis;
                double tran_x = tran_x_min + i * (tran_x_max - tran_x_min) / num_samples_per_axis;
                double tran_y = tran_y_min + j * (tran_y_max - tran_y_min) / num_samples_per_axis;
                double tran_z = tran_z_min + k * (tran_z_max - tran_z_min) / num_samples_per_axis;
                rotations.push_back({rot_x, rot_y, rot_z});
                translations.push_back({tran_x, tran_y, tran_z});
            }
        }
    }
    rotations_and_translations = std::make_tuple(rotations, translations);
    return rotations_and_translations;
}

// Monte Carlo random sampling
std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>>DatasetSample::Monte_Carlo_sample_dataset(
    const std::vector<std::vector<int>>& rot_range_list,
    const std::vector<std::vector<int>>& trans_range_list,
    int num_samples) 
{
    std::vector<std::vector<double>> rotations;
    std::vector<std::vector<double>> translations;
    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> rotations_and_translations;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> rot_x_dist(rot_range_list[0][0], rot_range_list[0][1]);
    std::uniform_real_distribution<> rot_y_dist(rot_range_list[1][0], rot_range_list[1][1]);
    std::uniform_real_distribution<> rot_z_dist(rot_range_list[2][0], rot_range_list[2][1]);
    std::uniform_real_distribution<> tran_x_dist(trans_range_list[0][0], trans_range_list[0][1]);
    std::uniform_real_distribution<> tran_y_dist(trans_range_list[1][0], trans_range_list[1][1]);
    std::uniform_real_distribution<> tran_z_dist(trans_range_list[2][0], trans_range_list[2][1]);

    for (int i = 0; i < num_samples; ++i) {
        std::vector<double> rotation = {rot_x_dist(gen), rot_y_dist(gen), rot_z_dist(gen)};
        std::vector<double> translation = {tran_x_dist(gen), tran_y_dist(gen), tran_z_dist(gen)};
        rotations.push_back(rotation);
        translations.push_back(translation);
    }
    rotations_and_translations = std::make_tuple(rotations, translations);
    return rotations_and_translations;
}

void DatasetSample::show_3D_resample(const std::vector<std::vector<double>>& samples) {
    /*3D visualize need to be fixed*/
    std::vector<double> x_coords, y_coords, z_coords;
    for (const auto& sample : samples) {
        x_coords.push_back(sample[0]);
        y_coords.push_back(sample[1]);
        z_coords.push_back(sample[2]);
    }

    plt::scatter(x_coords, y_coords, z_coords, 10.0);
    plt::xlabel("X Axis");
    plt::ylabel("Y Axis");
    plt::set_zlabel("Z Axis");
    plt::title("Uniform Samples Visualization");
    plt::show();
}

