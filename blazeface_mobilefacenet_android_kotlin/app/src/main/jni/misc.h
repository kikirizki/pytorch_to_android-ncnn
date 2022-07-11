//
// Created by kiki on 26/06/22.
//

#ifndef FACE_RECOGNITION_KT_MISC_H
#define FACE_RECOGNITION_KT_MISC_H
struct FaceObject
{
    cv::Rect_<float> rect;
    int label;
    std::string name;
    float score;
    std::vector<cv::Point2f> pts;
    float  rotation;
    float  cx;
    float  cy;
    float  w;
    float  h;
    cv::Point2f  pos[4];
    cv::Mat trans_image;
    std::vector<cv::Point2f> skeleton;
    std::vector<cv::Point2f> left_eyes;
    std::vector<cv::Point2f> right_eyes;
};
#endif //FACE_RECOGNITION_KT_MISC_H
