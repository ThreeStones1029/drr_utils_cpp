#include <yaml-cpp/yaml.h> //yaml-cpp 头文件
#include <iostream>
#include <boost/program_options.hpp>


int main(int argc,char** argv){
   // define parser
   try {
         boost::program_options::options_description desc{"Options"};
         desc.add_options()
               ("help,h", "Using ITK to generate spine DRR detection dataset!")
               ("config, c", boost::program_options::value<std::string>()->default_value("../config/detection_config.yaml"), "Path to the YAML configuration file")
               ("regenerate_specified_cts, r", boost::program_options::value<bool>()->default_value(false), 
                                                      "The ct name list will be regenerated drrs, if all , then regenerate all from zero");
         boost::program_options::variables_map vm;
         boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
         boost::program_options::notify(vm);
         std::string config_path = vm["config"].as<std::string>();
         std::cout << "config:" << config_path << std::endl;
         std::cout << "regenerate_specified_cts:" << vm["regenerate_specified_cts"].as<bool>() << std::endl;

         YAML::Node config = YAML::LoadFile(config_path);
         std::cout << "ct_root_path:" << config["ct_root_path"].as<std::string>() << std::endl;
         std::cout << "dataset_path:" << config["dataset_path"].as<std::string>() << std::endl;
         std::cout << "projection_parameter: sdr" << config["projection_parameter"]["sdr"].as<int>() << std::endl;
   }
   catch (const boost::program_options::error &ex){
         std::cerr << ex.what() << "\n";
   }

   return 0;
}