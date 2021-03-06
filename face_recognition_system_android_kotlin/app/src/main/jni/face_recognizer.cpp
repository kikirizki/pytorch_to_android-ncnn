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

#include "face_recognizer.h"

#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cpu.h"


int FaceRecognizer::load(AAssetManager *mgr, const char *modeltype, bool use_gpu) {
    face_embeding_net.clear();

    ncnn::set_cpu_powersave(2);
    ncnn::set_omp_num_threads(ncnn::get_big_cpu_count());

    face_embeding_net.opt = ncnn::Option();

#if NCNN_VULKAN
    face_embeding_net.opt.use_vulkan_compute = use_gpu;
#endif

    face_embeding_net.opt.num_threads = ncnn::get_big_cpu_count();
    char param_path[256];
    char model_path[256];
    sprintf(param_path, "%s.param", modeltype);
    sprintf(model_path, "%s.bin", modeltype);
    face_embeding_net.load_param(mgr, param_path);
    face_embeding_net.load_model(mgr, model_path);


    return 0;
}

void FaceRecognizer::add_face(ncnn::Mat &img) {
    ncnn::Mat embd = FaceRecognizer::forward(img);
    FaceRecognizer::saved_embedding.emplace_back(embd);
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
    if (v1.size() != v2.size() || !v1.size())
        return 0;
    float ret = 0.0, mod1 = 0.0, mod2 = 0.0;
    for (std::vector<double>::size_type i = 0; i != v1.size(); ++i) {
        ret += v1[i] * v2[i];
        mod1 += v1[i] * v1[i];
        mod2 += v2[i] * v2[i];
    }
    return ret / sqrt(mod1) / sqrt(mod2);
}

std::string FaceRecognizer::get_name(ncnn::Mat &in) {
    ncnn::Mat feat_a_ncnn = FaceRecognizer::saved_embedding[0];
    ncnn::Mat feat_b_ncnn = FaceRecognizer::saved_embedding[1];
    ncnn::Mat unknown_emd = FaceRecognizer::forward(in);
    std::vector<float> person_a_feature;
    std::vector<float> person_b_feature;
    std::vector<float> unknown_person;

    for (int j = 0; j < 128; j++) {
        person_a_feature.emplace_back(feat_a_ncnn[j]);
        person_b_feature.emplace_back(feat_b_ncnn[j]);
        unknown_person.emplace_back(unknown_emd[j]);
    }
//    normalize(person_a_feature);
//    normalize(person_b_feature);
//    normalize(unknown_person);
    float sim_a = calculate_cosine_similarity(person_a_feature, unknown_person);
    float sim_b = calculate_cosine_similarity(person_b_feature, unknown_person);

    if (sim_a > sim_b) {
        return "terry " + std::to_string(sim_a);
    }
    if (sim_a < sim_b) {
        return "robert " + std::to_string(sim_b);

    }
}

float FaceRecognizer::compare_face(ncnn::Mat &face_a, ncnn::Mat &face_b) {
    std::vector<float> person_a_feature;
    std::vector<float> person_b_feature;
    ncnn::Mat feat_a_ncnn = FaceRecognizer::forward(face_a);
    ncnn::Mat feat_b_ncnn = FaceRecognizer::forward(face_b);
    for (int j = 0; j < 128; j++) {
        person_a_feature.emplace_back(feat_a_ncnn[j]);
        person_b_feature.emplace_back(feat_b_ncnn[j]);
    }
    normalize(person_a_feature);
    normalize(person_b_feature);
    return calculate_cosine_similarity(person_a_feature, person_b_feature);
}

int FaceRecognizer::get_embeding(std::vector<FaceObject> &faceobjects) {
    for (auto &obj: faceobjects) {


        ncnn::Mat in = ncnn::Mat::from_pixels_resize(obj.trans_image.data,
                                                     ncnn::Mat::PIXEL_RGB,
                                                     obj.trans_image.cols, obj.trans_image.rows,
                                                     112, 112);

        obj.name = "Unknown";
        obj.name = FaceRecognizer::get_name(in);

    }
    return 0;
}

ncnn::Mat FaceRecognizer::forward(ncnn::Mat &in) {
    ncnn::Extractor ex = face_embeding_net.create_extractor();

    ex.set_light_mode(true);
    ex.input("data", in);
    ncnn::Mat out;
    ex.extract("fc1", out);
    return out;
}

void FaceRecognizer::clear_face_cache() {
    FaceRecognizer::saved_embedding.clear();
}
