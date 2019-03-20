package com.sohu.ping.utils;

import android.util.Log;

/**
 * Created by wangyan on 2019/3/20
 */
public class LogUtils {
    private static boolean isShowLog = true;
    private static final String DEFAULT_TAG = "ping";

    public static void i(String tag, String message) {
        if (isShowLog) {
            Log.i(DEFAULT_TAG, tag + " [" + message + "]");
        }
    }

    public static void i(String message) {
        if (isShowLog) {
            Log.i(DEFAULT_TAG, message);
        }
    }

    public static void d(String tag, String message) {
        if (isShowLog) {
            Log.d(DEFAULT_TAG, tag + " [" + message + "]");
        }
    }

    public static void d(String message) {
        if (isShowLog) {
            Log.d(DEFAULT_TAG, message);
        }
    }

    public static void w(String tag, String message) {
        if (isShowLog) {
            Log.w(DEFAULT_TAG, tag + " [" + message + "]");
        }
    }

    public static void w(String message) {
        if (isShowLog) {
            Log.w(DEFAULT_TAG, message);
        }
    }

    public static void e(String tag, String message, Throwable e) {
        if (isShowLog) {
            Log.e(DEFAULT_TAG, tag + " [" + message + "]", e);
        }
    }

    public static void e(String tag, Throwable e) {
        if (isShowLog) {
            Log.e(DEFAULT_TAG, tag + " [" + Log.getStackTraceString(e) + "]", e);
        }
    }

    public static void setDebug(boolean isDebug) {
        isShowLog = isDebug;
    }

    public static boolean isDebug() {
        return isShowLog;
    }
}
