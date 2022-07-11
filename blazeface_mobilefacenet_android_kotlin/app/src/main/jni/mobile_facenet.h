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

#ifndef POCKETNET_H
#define POCKETNET_H

#include <opencv2/core/core.hpp>
#include <net.h>
#include "misc.h"

class MobileFaceNet {
public:
    int load(AAssetManager *mgr, bool use_gpu = false);

    int get_embeding(std::vector<FaceObject> &faceobjects);

    void add_face(ncnn::Mat &img);

    float compare_face(ncnn::Mat &face_a, ncnn::Mat &face_b);
private:
    ncnn::Net pocketnet;
    std::vector<ncnn::Mat> saved_embedding ;
    ncnn::Mat forward(ncnn::Mat &in);

    std::string get_name(ncnn::Mat &in);


};

#endif // BLAZEFACE_H
