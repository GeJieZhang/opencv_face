package com.example.mylibrary;

import android.graphics.Bitmap;

public class FaceFactory {


    public int faceDetectionSaveInfo(Bitmap bitmap) {
        return JNITest.faceDetectionSaveInfo(bitmap);
    }


    public void loadCascade(String filePath) {
        JNITest.loadCascade(filePath);
    }
}

