package com.sohu.ping;

/**
 * Created by wangyan on 2019/3/14
 */
public class NativeMethodHelper {

    static {
        System.loadLibrary("native");
    }

    public static native String ping(String domain);
}
