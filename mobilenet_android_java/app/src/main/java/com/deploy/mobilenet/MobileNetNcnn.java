package com.deploy.mobilenet;
import android.content.res.AssetManager;
import android.view.Surface;

public class MobileNetNcnn {
    public native boolean loadModel(AssetManager mgr, int cpugpu);
    public native boolean openCamera(int facing);
    public native boolean closeCamera();
    public native boolean setOutputWindow(Surface surface);

    static {
        System.loadLibrary("mobilenetncnn");
    }
}
