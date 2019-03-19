package com.sohu.opengl;

/**
 * Created by wangyan on 2019/3/14
 */
public class NativeMethodHelper {

    static {
        System.loadLibrary("native-color");
    }

    public static native void surfaceCreated(int color);

    public static native void surfaceChanged(int width, int height);

    public static native void onDrawFrame();

    public static native int ping(String domain);
}
