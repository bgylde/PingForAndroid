package com.sohu.opengl;

import android.graphics.Color;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by wangyan on 2019/3/6
 */
public class ColorRenderer implements GLSurfaceView.Renderer {

    private int color;

    public ColorRenderer(int color) {
        this.color = color;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        float redF = (float) Color.red(color);
        float greenF = (float) Color.green(color);
        float blueF = (float) Color.blue(color);
        float alphaF = (float) Color.alpha(color);

        GLES30.glClearColor(redF, greenF, blueF, alphaF);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES30.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES30.glClear(GL10.GL_COLOR_BUFFER_BIT);
    }
}
