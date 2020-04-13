package com.example.helloopengles;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static com.example.utils.CommonDefine.ERROR_OK;

public class MyGLRenderer implements GLSurfaceView.Renderer {

    private final static String TAG = "MyGLRenderer";
    private MyGLSurfaceView mMyGLSurfaceView;

    public boolean getCreateState() {
        return bCreateState;
    }

    private boolean bCreateState = false;

    MyGLRenderer (MyGLSurfaceView glSurfaceView) {
        mMyGLSurfaceView = glSurfaceView;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        MyLog.d(TAG, "onSurfaceCreated");
        GLSurfaceCreated ();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        MyLog.d(TAG, "onSurfaceChanged");
        //GLES30.glViewport(0, 0, width, height);
        int ret = onSurfaceChangedJNI (width, height);
        MyLog.d(TAG, "onSurfaceChangedJNI ret = " + ret);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        MyLog.d(TAG, "onDrawFrame");
        //GLES30.glClear (GL10.GL_COLOR_BUFFER_BIT);
        int ret = onDrawFrameJNI ();
        MyLog.d(TAG, "onDrawFrameJNI ret = " + ret);
        mMyGLSurfaceView.mRenderTime = mMyGLSurfaceView.mRenderTime + 1;
        mMyGLSurfaceView.requestRender();
    }

    public void GLSurfaceCreated () {
        MyLog.d(TAG, "GLSurfaceCreated");
        int ret = onSurfaceCreatedJNI();
        MyLog.d(TAG, "onSurfaceCreatedJNI ret = " + ret);
        bCreateState = (ret == ERROR_OK);
    }

    public void GLSurfaceDestroyed() {
        MyLog.d(TAG, "GLSurfaceDestroyed");
        int ret = onSurfaceDestroyedJNI ();
        bCreateState = false;
        MyLog.d(TAG, "onSurfaceDestroyedJNI ret = " + ret);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int onSurfaceCreatedJNI();
    public native int onSurfaceChangedJNI (int width, int height);
    public native int onDrawFrameJNI ();
    public native int onSurfaceDestroyedJNI ();
}
