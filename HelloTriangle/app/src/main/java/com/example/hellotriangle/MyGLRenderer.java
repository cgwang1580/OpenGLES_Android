package com.example.hellotriangle;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLRenderer implements GLSurfaceView.Renderer {

    private final static String TAG = "MyGLRenderer";
    private MyGLSurfaceView mGLSurfaceView;

    MyGLRenderer (MyGLSurfaceView glSurfaceView) {
        mGLSurfaceView = glSurfaceView;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        MyLog.d(TAG, "onSurfaceCreated");
        GLES30.glClearColor(0, 1.0f, 0, 1.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        MyLog.d(TAG, "onSurfaceChanged");
        GLES30.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        MyLog.d(TAG, "onDrawFrame");
        GLES30.glClear (GL10.GL_COLOR_BUFFER_BIT);
        mGLSurfaceView.mRenderTime = mGLSurfaceView.mRenderTime + 1;
        mGLSurfaceView.requestRender();
    }
}
