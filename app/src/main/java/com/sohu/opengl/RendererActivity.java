package com.sohu.opengl;

import android.app.Activity;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.Nullable;

/**
 * Created by wangyan on 2019/3/6
 */
public class RendererActivity extends Activity {

    private GLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setupViews();
    }

    private void setupViews() {
        mGLSurfaceView = new GLSurfaceView(this);
        setContentView(mGLSurfaceView);

        mGLSurfaceView.setEGLContextClientVersion(3);
//        GLSurfaceView.Renderer renderer = new ColorRenderer(Color.RED);
        GLSurfaceView.Renderer renderer = new NativeColorRenderer(Color.RED);

        mGLSurfaceView.setRenderer(renderer);
    }
}
