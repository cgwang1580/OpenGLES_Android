package com.cgwang1580.helloopengles;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView{

    private final static String TAG = "MyGLSurfaceView";
    private final static int GLES_VERSION = 3;
    protected GLSurfaceView mGLSurfaceView = null;
    protected MyGLRenderer mGLRenderer = null;
    protected int mRenderTime = 0;

    MyGLSurfaceView () {}

    protected void Init (Context context) {

        MyLog.d(TAG, "Init");
        mGLSurfaceView = ((Activity)context).findViewById(R.id.gl_surface_view);

        mGLSurfaceView.setEGLContextClientVersion(GLES_VERSION);
        mGLRenderer = new MyGLRenderer(this);
        mGLSurfaceView.setRenderer(mGLRenderer);
        mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    protected void requestRender () {
        MyLog.d(TAG, "requestRender");
        if (null != mGLSurfaceView && mRenderTime < 10) {
            mGLSurfaceView.requestRender();
        }
    }

    protected void MyGLSurfaceResume () {
        if (null != mGLSurfaceView) {
            mGLSurfaceView.queueEvent(new Runnable() {
                @Override
                public void run() {
                    if (!mGLRenderer.getCreateState()) {
                        mGLRenderer.GLSurfaceCreated();
                    }
                }
            });
        }
    }

    protected void MyGLSurfacePause () {
        if (null != mGLSurfaceView) {
            mGLSurfaceView.queueEvent(new Runnable() {
                @Override
                public void run() {
                    mGLRenderer.GLSurfaceDestroyed();
                }
            });
            mRenderTime = 0;
        }
    }
}
