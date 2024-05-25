#include <vector>
#include <iostream>
#include "debug_print.h"


template <typename T>
void print_two_dimensional_array(const std::vector<std::vector<T>>& array){
    for (const auto& row : array) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}
// 显式实例化模板
template void print_two_dimensional_array<int>(const std::vector<std::vector<int>>&);
template void print_two_dimensional_array<double>(const std::vector<std::vector<double>>&);