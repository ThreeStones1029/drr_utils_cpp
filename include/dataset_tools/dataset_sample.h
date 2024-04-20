/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-20 09:24:29
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-04-20 09:33:43
 */
#ifndef DATASET_SAMPLE_H
#define DATASET_SAMPLE_H

#include<vector>

class DatasetSample{
    public:
        // 网格法均匀采样
    void generateUniformSamplesGrid(const std::vector<std::pair<int, int>>& rotRangeList, const std::vector<std::pair<int, int>>& transRangeList, int numSamples);

    // 蒙特卡洛方法
    void monteCarloSampleDataset(const std::vector<std::pair<int, int>>& rotRangeList, const std::vector<std::pair<int, int>>& transRangeList, int numSamples);

    void show3DResample();

private:
    std::vector<std::vector<int>> rotations;
    std::vector<std::vector<int>> translations;

};


#endif // DATASET_SAMPLE_H