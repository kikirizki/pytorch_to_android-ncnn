package com.deploy.facerecognitionkt

import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import java.io.IOException
import java.io.InputStream

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
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

@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("com.deploy.facerecognitionkt", appContext.packageName)
    }
    @Test
    fun compareFaces(){
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        val bitmap = getBitmapFromAsset(appContext, "robert.jpg");
        val face_recognition = FaceRecognition();
        val distance = face_recognition.compareFaces(bitmap,bitmap);

        assertEquals(distance,0);

    }
}