# Deploy Pytorch Model to Android 
Easy pipeline to deploy deep learning on android, powered by :
<p>
<img src="https://raw.githubusercontent.com/kikirizki/pytorch_to_android-ncnn/master/images/Pytorch_logo.png"  height="64" /> &
<img src="https://raw.githubusercontent.com/kikirizki/pytorch_to_android-ncnn/master/images/256-ncnn.png"  height="64" />
</p>

# Road Map
## Image Classification
- [x] MobilenetV2
- [ ] ViT
## Object Detecction
- [ ] YoloX
## Face Recogniton
- [x] MobileFaceNet + PocketNet 
- [ ] ElasticFace
- [ ] ArcFace
- [x] MobileFaceNet + BlazeFace 
## Semantic Segmentation
- [ ] Yolact
## Conversion Script from PyTorch to NCNN
- PocketNet [![Pytorch](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/kikirizki/pytorch_to_android-ncnn/blob/master/pytorch_pocketnet_to_ncnn.ipynb)
- MobileNet [![Pytorch](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/kikirizki/pytorch_to_android-ncnn/blob/master/pytorch_mobilenet_to_ncnn.ipynb)
## Demo
### BlazeFace + MobileFaceNet
![](https://raw.githubusercontent.com/kikirizki/pytorch_to_android-ncnn/master/demos/blazeface_mobilefacenet.gif)
### BlazeFace + PocketNet 128
![](https://raw.githubusercontent.com/kikirizki/pytorch_to_android-ncnn/master/demos/blazeface_pocketnet.gif)
## Reference 
- https://github.com/FeiGeChuanShu/ncnn_Android_blazeface
- https://github.com/nihui/ncnn
- https://github.com/fdbtrs/PocketNet
- https://github.com/liguiyuan/mobilefacenet-ncnn
- https://github.com/nihui/opencv-mobile
