package com.deploy.facerecognitionkt

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.PixelFormat
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.view.WindowManager
import android.widget.AdapterView
import android.widget.Button
import android.widget.Spinner
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.io.IOException
import java.io.InputStream


class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {
    var face_recognizer = FaceRecognition()
    private var facing = 0
    private var spinnerCPUGPU: Spinner? = null
    private var spinnerDetectionModel: Spinner? = null
    private var spinnerRecognitionModel: Spinner? = null
    private var detection_model = 0
    private var recognition_model = 0
    private var current_cpugpu = 0
    private var cameraView: SurfaceView? = null
    private var yourSelectedImage: Bitmap? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        cameraView = findViewById<View>(R.id.cameraview) as SurfaceView
        cameraView!!.holder.setFormat(PixelFormat.RGBA_8888)
        cameraView!!.holder.addCallback(this)
        val buttonSwitchCamera = findViewById<View>(R.id.buttonSwitchCamera) as Button
        buttonSwitchCamera.setOnClickListener {
            val new_facing = 1 - facing
            face_recognizer.closeCamera()
            face_recognizer.openCamera(new_facing)
            facing = new_facing
        }
        spinnerDetectionModel = findViewById<View>(R.id.spinnerDetectionModel) as Spinner
        spinnerDetectionModel!!.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                arg0: AdapterView<*>?,
                arg1: View,
                position: Int,
                id: Long
            ) {
                if (position != detection_model) {
                    detection_model = position
                    reload()

                }
            }

            override fun onNothingSelected(arg0: AdapterView<*>?) {}
        }
        spinnerRecognitionModel = findViewById<View>(R.id.spinnerRecognitionModel) as Spinner
        spinnerRecognitionModel!!.onItemSelectedListener = object :AdapterView.OnItemSelectedListener{
            override fun onItemSelected(
                arg0: AdapterView<*>?,
                arg1: View,
                position: Int,
                id: Long
            ) {
                if (position != recognition_model) {
                    recognition_model = position
                    reload()

                }
            }

            override fun onNothingSelected(arg0: AdapterView<*>?) {}
        }
        spinnerCPUGPU = findViewById<View>(R.id.spinnerCPUGPU) as Spinner
        spinnerCPUGPU!!.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                arg0: AdapterView<*>?,
                arg1: View,
                position: Int,
                id: Long
            ) {
                if (position != current_cpugpu) {
                    current_cpugpu = position
                    reload()
                }
            }

            override fun onNothingSelected(arg0: AdapterView<*>?) {}
        }
        reload()
    }

    override fun surfaceCreated(surfaceHolder: SurfaceHolder) {
        face_recognizer.setOutputWindow(surfaceHolder.surface)
    }

    override fun surfaceChanged(surfaceHolder: SurfaceHolder, i: Int, i1: Int, i2: Int) {}
    override fun surfaceDestroyed(surfaceHolder: SurfaceHolder) {}
    public override fun onResume() {
        super.onResume()
        if (ContextCompat.checkSelfPermission(
                applicationContext,
                Manifest.permission.CAMERA
            ) == PackageManager.PERMISSION_DENIED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.CAMERA),
                REQUEST_CAMERA
            )
        }
        face_recognizer.openCamera(facing)
    }

    public override fun onPause() {
        super.onPause()
        face_recognizer.closeCamera()
    }

    fun getBitmapFromAsset(context: Context, filePath: String?): Bitmap? {
        val assetManager: AssetManager = context.getAssets()
        val istr: InputStream
        var bitmap: Bitmap? = null
        try {
            istr = assetManager.open(filePath!!)
            bitmap = BitmapFactory.decodeStream(istr)
        } catch (e: IOException) {
            // handle exception
        }
        return bitmap
    }


    private fun reload() {
        val ret_init = face_recognizer.loadModel(assets, detection_model,recognition_model, current_cpugpu)
        val terry_bitmap = getBitmapFromAsset(applicationContext, "terry.jpg");
        val robert_bitmap = getBitmapFromAsset(applicationContext, "robert.jpg");

        val terry_add_face = face_recognizer.addFace(terry_bitmap);
        val robert_add_face = face_recognizer.addFace(robert_bitmap);
        if (!terry_add_face) {
            Log.e("MainActivity", "terry add face failed")
        }
        if (!ret_init) {
            Log.e("MainActivity", "blazefacencnn loadModel failed")
        }
    }

    companion object {
        const val REQUEST_CAMERA = 100
    }
}