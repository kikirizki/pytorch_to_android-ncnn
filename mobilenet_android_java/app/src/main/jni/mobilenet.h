#ifndef MOBILENET_H
#define MOBILENET_H

#include <opencv2/core/core.hpp>
#include <net.h>

class Mobilenet
{
public:
    Mobilenet();

    int load(AAssetManager* mgr, const char* modeltype, bool use_gpu = false);

    std::string classify(const cv::Mat& rgb);

private:

    ncnn::Net mobilenet_model;

    int target_size;
    float mean_vals[3];
    float norm_vals[3];
    ncnn::UnlockedPoolAllocator blob_pool_allocator;
    ncnn::PoolAllocator workspace_pool_allocator;
};

#endif // MOBILENET_H
