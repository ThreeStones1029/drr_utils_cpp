/*
 * @Description: 
 * @version: 
 * @Author: ThreeStones1029 2320218115@qq.com
 * @Date: 2024-04-01 14:09:34
 * @LastEditors: ShuaiLei
 * @LastEditTime: 2024-05-23 13:26:50
 */
#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>
#include <yaml-cpp/yaml.h> //yaml-cpp 头文件
#include "file_process.h"
#include "GenDetectionDataset.h"
#include "dataset_sample.h"
// #include <Eigen/Dense>


int main(int argc,char** argv){
   // define parser
   YAML::Node config;
   std::string config_path;
   bool regenerate_specified_cts;
   try {
         boost::program_options::options_description desc{"Options"};
         desc.add_options()
               ("help,h", "Using ITK to generate spine DRR detection dataset!")
               ("config,c", boost::program_options::value<std::string>()->default_value("../config/detection_config.yaml"), "Path to the YAML configuration file")
               ("regenerate_specified_cts,r", boost::program_options::value<bool>()->default_value(true), 
                                                      "The ct name list will be regenerated drrs, if all , then regenerate all from zero");
         boost::program_options::variables_map vm;
         boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
         boost::program_options::notify(vm);
         config_path = vm["config"].as<std::string>();
         regenerate_specified_cts = vm["regenerate_specified_cts"].as<bool>();
         std::cout << "config:" << config_path << std::endl;
         std::cout << "regenerate_specified_cts:" << regenerate_specified_cts << std::endl;

         config = YAML::LoadFile(config_path);
   }
   catch (const boost::program_options::error &ex){
         std::cerr << ex.what() << "\n";
   }

   // whether regenerate drrs and masks
   if (regenerate_specified_cts){
      std::string dataset_path = config["dataset_path"].as<std::string>();
      std::filesystem::path dir_path(dataset_path);
      try {
         // 删除文件夹及其下面的所有文件和文件夹
         remove_all_files(dir_path);
         std::cout << "[Note]: Directory and its contents deleted successfully." << std::endl;
      } catch (const std::filesystem::filesystem_error& ex) {
         std::cerr << "Error deleting directory: " << ex.what() << std::endl;
      }
      std::cout << "Then Generate from zero." << std::endl;
   }
   
   GenDetectionDataset dataset(config);

   // DatasetSample dataset;

   // Eigen::MatrixXd rot_range_list(3, 2);
   // rot_range_list << 60, 120,
   //                   150, 210,
   //                   150, 210;

   // Eigen::MatrixXd trans_range_list(3, 2);
   // trans_range_list << 0, 10,
   //                   0, 10,
   //                   0, 10;

   // int num_samples = 1000;

   // auto [rotations, translations] = dataset.generate_uniform_samples_grid(rot_range_list, trans_range_list, num_samples);
   // dataset.show_3D_resample(rotations);

   // auto [mc_rotations, mc_translations] = dataset.Monte_Carlo_sample_dataset(rot_range_list, trans_range_list, num_samples);
   // dataset.show_3D_resample(mc_rotations);

        

   return 0;
}