package com.sohu.opengl;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by wangyan on 2019/3/6
 */
public class NativeColorRenderer implements GLSurfaceView.Renderer {

    private int color;

    public NativeColorRenderer(int color) {
        this.color = color;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        NativeMethodHelper.surfaceCreated(color);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeMethodHelper.surfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeMethodHelper.onDrawFrame();
    }
}
