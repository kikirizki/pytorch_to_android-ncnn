package com.deploy.facerecognitionkt

import android.content.res.AssetManager
import android.view.Surface
import android.graphics.Bitmap




class FaceRecognition {
    external fun loadModel(mgr: AssetManager?, modelid: Int,modelid2:Int, cpugpu: Int): Boolean
    external fun openCamera(facing: Int): Boolean
    external fun closeCamera(): Boolean
    external fun setOutputWindow(surface: Surface?): Boolean
    external fun addFace(face_a: Bitmap?): Boolean


    init
    {
        System.loadLibrary("face_recognition")
    }
}