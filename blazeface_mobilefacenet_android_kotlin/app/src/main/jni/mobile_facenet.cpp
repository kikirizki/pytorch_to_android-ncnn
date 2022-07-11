// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "mobile_facenet.h"

#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cpu.h"


int MobileFaceNet::load(AAssetManager *mgr, bool use_gpu) {
    pocketnet.clear();

    ncnn::set_cpu_powersave(2);
    ncnn::set_omp_num_threads(ncnn::get_big_cpu_count());

    pocketnet.opt = ncnn::Option();

#if NCNN_VULKAN
    pocketnet.opt.use_vulkan_compute = use_gpu;
#endif

    pocketnet.opt.num_threads = ncnn::get_big_cpu_count();

    pocketnet.load_param(mgr, "mobilefacenet.param");
    pocketnet.load_model(mgr, "mobilefacenet.bin");


    return 0;
}

void MobileFaceNet::add_face(ncnn::Mat &img) {
    ncnn::Mat embd = MobileFaceNet::forward(img);
    MobileFaceNet::saved_embedding.emplace_back(embd);
}

void normalize(std::vector<float> &feature) {
    float sum = 0;
    for (auto it = feature.begin(); it != feature.end(); it++) {
        sum += (float) *it * (float) *it;
    }
    sum = sqrt(sum);
    for (auto it = feature.begin(); it != feature.end(); it++) {
        *it /= sum;
    }
}

float calculate_cosine_similarity(std::vector<float> &v1, std::vector<float> &v2) {
    float similarity;
    if (v1.size() != v2.size() || !v1.size())
        return 0;
    float ret = 0.0, mod1 = 0.0, mod2 = 0.0, dist = 0.0, diff = 0.0;

    for (std::vector<float>::size_type i = 0; i != v1.size(); ++i) {
        ret += v1[i] * v2[i];
        mod1 += v1[i] * v1[i];
        mod2 += v2[i] * v2[i];
    }
    similarity = ret / (sqrt(mod1) * sqrt(mod2));

    return similarity;
}

std::string MobileFaceNet::get_name(ncnn::Mat &in) {
    ncnn::Mat feat_a_ncnn = MobileFaceNet::saved_embedding[0];
    ncnn::Mat feat_b_ncnn = MobileFaceNet::saved_embedding[1];
    ncnn::Mat unknown_emd = MobileFaceNet::forward(in);
    std::vector<float> person_a_feature;
    std::vector<float> person_b_feature;
    std::vector<float> unknown_person;

    for (int j = 0; j < 128; j++) {
        person_a_feature.emplace_back(feat_a_ncnn[j]);
        person_b_feature.emplace_back(feat_b_ncnn[j]);
        unknown_person.emplace_back(unknown_emd[j]);
    }
    normalize(person_a_feature);
    normalize(person_b_feature);
    normalize(unknown_person);
    float sim_a = calculate_cosine_similarity(person_a_feature, unknown_person);
    float sim_b = calculate_cosine_similarity(person_b_feature, unknown_person);

    if (sim_a > sim_b) {
        return "terry " + std::to_string(sim_a);
    }
    if (sim_a < sim_b) {
        return "robert " + std::to_string(sim_b);

    }
}

float MobileFaceNet::compare_face(ncnn::Mat &face_a, ncnn::Mat &face_b) {
    std::vector<float> person_a_feature;
    std::vector<float> person_b_feature;
    ncnn::Mat feat_a_ncnn = MobileFaceNet::forward(face_a);
    ncnn::Mat feat_b_ncnn = MobileFaceNet::forward(face_b);
    for (int j = 0; j < 128; j++) {
        person_a_feature.emplace_back(feat_a_ncnn[j]);
        person_b_feature.emplace_back(feat_b_ncnn[j]);
    }
    normalize(person_a_feature);
    normalize(person_b_feature);
    return calculate_cosine_similarity(person_a_feature, person_b_feature);
}

int MobileFaceNet::get_embeding(const cv::Mat &rgb, std::vector<FaceObject> &faceobjects) {
    for (auto &obj: faceobjects) {

        cv::Mat imCrop = rgb(obj.rect);
        ncnn::Mat in = ncnn::Mat::from_pixels_resize(imCrop.data,
                                                     ncnn::Mat::PIXEL_RGB,
                                                     imCrop.cols, imCrop.rows,
                                                     112, 112);

        obj.name = "Unknown";
        obj.name = MobileFaceNet::get_name(in);

    }
    return 0;
}

ncnn::Mat MobileFaceNet::forward(ncnn::Mat &in) {
    ncnn::Extractor ex = pocketnet.create_extractor();
    ex.input("data", in);
    ncnn::Mat out;
    ex.extract("fc1", out);
    return out;
}
void MobileFaceNet::addFace(ncnn::Mat &in){
    MobileFaceNet::saved_embedding.emplace_back(in);
}