package com.example.hellotriangle;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("processor");
    }

    MyGLSurfaceView myGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (null == myGLSurfaceView) {
            InitGLSurfaceView (this);
        }
    }

    @Override
    protected void onResume () {
        super.onResume();
        if (null != myGLSurfaceView) {
            myGLSurfaceView.requestRender();
        }
    }

    @Override
    protected void onPause () {
        super.onPause();
        if (null != myGLSurfaceView) {
            myGLSurfaceView.mGLRenderer.surfaceDestroyed();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    public void InitGLSurfaceView (Context context) {
        MyLog.d(TAG, "InitGLSurfaceView");
        myGLSurfaceView = new MyGLSurfaceView();
        myGLSurfaceView.Init(context);
    }
}
