package com.example.mylibrary;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.example.mylibrary.FaceFactory;
import com.example.mylibrary.JNITest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity3 extends AppCompatActivity {


    private ImageView iv_image;
    private Button btn_face;


    private File mCascadeFile;

    private Bitmap bitmap;

    private JNITest jniTest;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        jniTest=new JNITest();
        iv_image = findViewById(R.id.iv_image);
        btn_face = findViewById(R.id.btn_face);

        btn_face.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {


                jniTest.faceDetectionSaveInfo(bitmap);
            }
        });

        bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.ssr);

        iv_image.setImageBitmap(bitmap);

        copyCasecadeFile();

        jniTest.loadCascade(mCascadeFile.getAbsolutePath());
    }




    private void copyCasecadeFile() {
        try {
            // load cascade file from application resources
            InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");


            if (mCascadeFile.exists()) return;
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();

            cascadeDir.delete();

        } catch (IOException e) {
            e.printStackTrace();
            Log.e("TAG", "Failed to load cascade. Exception thrown: " + e);
        }
    }
}
