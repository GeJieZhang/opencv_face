package com.example.mylibrary;

import android.graphics.Bitmap;

public class JNITest {


    static {
        System.loadLibrary("native-lib");
    }

    public String getStr(){
        return "恭喜你jar包调用成功！";
    }


    public static native String stringFromJNI();


    public static native int faceDetectionSaveInfo(Bitmap bitmap);

    public static native void loadCascade(String filePath);
}
