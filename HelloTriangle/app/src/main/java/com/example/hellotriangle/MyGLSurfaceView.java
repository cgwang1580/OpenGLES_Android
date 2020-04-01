package com.example.hellotriangle;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView{

    private final static String TAG = "MyGLSurfaceView";
    private GLSurfaceView mGLSurfaceView = null;
    private MyGLRenderer mGLRenderer = null;
    protected int mRenderTime = 0;

    MyGLSurfaceView () {}

    public void Init (Context context) {

        MyLog.d(TAG, "Init");
        mGLSurfaceView = ((Activity)context).findViewById(R.id.gl_surface_view);

        mGLSurfaceView.setEGLContextClientVersion(3);
        mGLRenderer = new MyGLRenderer(this);
        mGLSurfaceView.setRenderer(mGLRenderer);
        mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public void requestRender () {
        MyLog.d(TAG, "requestRender");
        if (null != mGLSurfaceView && mRenderTime < 10) {
            mGLSurfaceView.requestRender();
        }
    }
}
