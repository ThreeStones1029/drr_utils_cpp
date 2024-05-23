/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 09:24:29
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-23 14:25:49
 */
#ifndef DATASET_SAMPLE_H
#define DATASET_SAMPLE_H
#include <vector>

class DatasetSample {
public:
    DatasetSample();

    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> generate_uniform_samples_grid(
        const std::vector<std::vector<int>>& rot_range_list,
        const std::vector<std::vector<int>>& trans_range_list,
        int num_samples);


    std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>> Monte_Carlo_sample_dataset(
        const std::vector<std::vector<int>>& rot_range_list,
        const std::vector<std::vector<int>>& trans_range_list,
        int num_samples);

    void show_3D_resample(const std::vector<std::vector<double>>& samples);
};

#endif // DATASET_SAMPLE_H
