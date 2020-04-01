package com.example.hellotriangle;

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
        //GLES30.glClearColor(0, 1.0f, 0, 1.0f);
        int ret = onSurfaceCreatedJNI ();
        MyLog.d(TAG, "onSurfaceCreatedJNI ret = " + ret);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        MyLog.d(TAG, "onSurfaceChanged");
        //GLES30.glViewport(0, 0, width, height);
        int ret = onSurfaceChangedJNI ();
        MyLog.d(TAG, "onSurfaceChangedJNI ret = " + ret);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        MyLog.d(TAG, "onDrawFrame");
        //GLES30.glClear (GL10.GL_COLOR_BUFFER_BIT);
        int ret = onDrawFrameJNI ();
        MyLog.d(TAG, "onDrawFrameJNI ret = " + ret);
        mGLSurfaceView.mRenderTime = mGLSurfaceView.mRenderTime + 1;
        mGLSurfaceView.requestRender();
    }

    public void surfaceDestroyed () {
        MyLog.d(TAG, "surfaceDestroyed");
        int ret = onSurfaceDestroyedJNI ();
        MyLog.d(TAG, "onSurfaceDestroyedJNI ret = " + ret);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int onSurfaceCreatedJNI();
    public native int onSurfaceChangedJNI ();
    public native int onDrawFrameJNI ();
    public native int onSurfaceDestroyedJNI ();
}
