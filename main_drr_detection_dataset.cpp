#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>
#include <yaml-cpp/yaml.h> //yaml-cpp 头文件
#include "common.h"


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
               ("regenerate_specified_cts,r", boost::program_options::value<bool>()->default_value(false), 
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


   if (regenerate_specified_cts){
      std::string dataset_path = config["dataset_path"].as<std::string>();
      std::filesystem::path dir_path(dataset_path);
      try {
         // 删除文件夹及其下面的所有文件和文件夹
         remove_all_files(dir_path);
         std::cout << "Directory and its contents deleted successfully." << std::endl;
      } catch (const std::filesystem::filesystem_error& ex) {
         std::cerr << "Error deleting directory: " << ex.what() << std::endl;
      }
      std::cout << "[Note]" << dataset_path << " is deleted." << "Then Generate from zero." << std::endl;
   }
        

   return 0;
}