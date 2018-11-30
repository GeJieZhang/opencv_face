#include <jni.h>
#include <string>

#include <android/log.h>
#include <opencv2/opencv.hpp>

#include <android/bitmap.h>

using namespace cv;

#define LOG_TAG "MyJni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)



CascadeClassifier cascadeClassifier;

Mat bitmap2Mat(JNIEnv *pEnv, jobject pJobject);

void mat2Bitmap(JNIEnv *pEnv, Mat mat, jobject pJobject);

extern "C" JNIEXPORT jstring
JNICALL
Java_com_example_mylibrary_JNITest_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

Mat bitmap2Mat(JNIEnv *env, jobject bitmap) {
    // 1. 获取图片的宽高，以及格式信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    Mat mat;

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
        mat = Mat(info.height, info.width, CV_8UC4, pixels);
    } else if (info.format = ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGD("nMatToBitmap: CV_8UC2 -> RGBA_565");
        mat = Mat(info.height, info.width, CV_8UC2, pixels);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    return mat;
}

void mat2Bitmap(JNIEnv *env, Mat src, jobject bitmap) {
    // 1. 获取图片的宽高，以及格式信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_GRAY2RGBA);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_RGB2RGBA);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
            src.copyTo(tmp);
        }
    } else {
        // info.format == ANDROID_BITMAP_FORMAT_RGB_565
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
            cvtColor(src, tmp, COLOR_GRAY2BGR565);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGB2BGR565);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGBA2BGR565);
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_mylibrary_JNITest_faceDetectionSaveInfo(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobject bitmap) {

    // opencv 操作图片操作的都是 矩阵 Mat
    // 1. bitmap2Mat
    Mat mat = bitmap2Mat(env, bitmap);

    Mat grayMat;
    // 2. 转成灰度图，提升运算速度，灰度图所对应的 CV_8UC1 单颜色通道，信息量少 0-255 1u
    cvtColor(mat, grayMat, CV_RGBA2GRAY);

    // 3. 转换直方图均衡化补偿
    Mat equalizeMat;
    equalizeHist(grayMat, equalizeMat);

    // 4. 检测人脸，这是个大问题
    std::vector<Rect> faces;
    cascadeClassifier.detectMultiScale(equalizeMat, faces, 1.1, 5, 0 | CV_HAAR_SCALE_IMAGE,
                                       Size(160, 160));

    LOGD("检测到人脸的个数：%d", faces.size());
    if (faces.size() == 1) {
        Rect faceRect = faces[0];
        // 画一个框框，标记出人脸
        rectangle(mat, faceRect, Scalar(255, 155, 155), 3);
        mat2Bitmap(env, mat, bitmap);

        // 只裁剪人脸部分的直方均衡补偿
        Mat saveMat = Mat(equalizeMat, faceRect);
        // mat 保存成文件  png ,上传到服务器吧，接着下一张（眨眼，张嘴巴）
        imwrite("xxxx/xxx.png", equalizeMat);
        return 1;
    }
    return 0;

}




extern "C"
JNIEXPORT void JNICALL
Java_com_example_mylibrary_JNITest_loadCascade(JNIEnv *env, jobject instance,
                                                              jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);
    cascadeClassifier.load(filePath);
    env->ReleaseStringUTFChars(filePath_, filePath);

    LOGD("人脸识别级联分类器加载成功");
}