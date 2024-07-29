#ifndef BBOX_VIS_H
#define BBOX_VIS_H

#include <opencv2/opencv.hpp>
#include "coco.h"
#include "nlohmann/json.hpp"

class VisCoCo : public COCO {
public:
    VisCoCo(const std::string& annotation_file, const std::string& images_folder, const std::string& bbox_vis_folder, const std::string& rotate_bbox_vis_folder);
    void visualize_bboxes_in_images();
    void visualize_bboxes_in_image(const std::string& file_path);
    void draw_bbox(cv::Mat& image, const std::vector<nlohmann::json>& annotations);


private:
    std::string images_folder;
    std::string bbox_vis_folder;
    std::string rotate_bbox_vis_folder;
    std::unordered_map<std::string, int> file_name2img_id;
    std::unordered_map<int, std::string> img_id2file_name;
    std::unordered_map<int, std::string> categories_id2name;
};

#endif // BBOX_VIS_H
