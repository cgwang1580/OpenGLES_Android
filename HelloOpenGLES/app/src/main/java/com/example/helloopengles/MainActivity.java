package com.example.helloopengles;

import android.Manifest;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Toast;

import com.example.Permission.PermissionHelper;
import com.example.Permission.PermissionInterface;

public class MainActivity extends AppCompatActivity {

    private final static String PROCESSOR_NAME = "processor.draw";

    static {
        System.loadLibrary(PROCESSOR_NAME);
    }

    private final static String TAG = "MainActivity";

    private final static String []PermissionList = new String []{Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE , Manifest.permission.CAMERA};

    MyGLSurfaceView myGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        MyLog.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestPermission ();
    }

    public void requestPermission () {
        PermissionHelper.MyRequestPermission(this, PermissionList, new PermissionInterface() {
            @Override
            public int doPermissionSucceed() {
                Toast.makeText(MainActivity.this, "onCreate doPermissionSucceed", Toast.LENGTH_SHORT).show();
                return 0;
            }

            @Override
            public int doPermissionFailed() {
                Toast.makeText(MainActivity.this, "onCreate doPermissionFailed", Toast.LENGTH_SHORT).show();
                return 0;
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        PermissionHelper.onMyRequestPermissionsResult(requestCode, permissions, grantResults);
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    protected void onResume () {
        MyLog.d(TAG, "onResume");
        super.onResume();
        if (null == myGLSurfaceView) {
            InitGLSurfaceView(this);
        }
        //myGLSurfaceView.MyGLSurfaceResume();
        myGLSurfaceView.requestRender();
    }

    @Override
    protected void onPause () {
        MyLog.d(TAG, "onPause");
        super.onPause();
        if (null != myGLSurfaceView) {
            myGLSurfaceView.MyGLSurfacePause ();
        }
    }

    @Override
    protected void onDestroy() {
        MyLog.d(TAG, "onDestroy");
        super.onDestroy();
    }

    public void InitGLSurfaceView (Context context) {
        MyLog.d(TAG, "InitGLSurfaceView");
        myGLSurfaceView = new MyGLSurfaceView();
        myGLSurfaceView.Init(context);
    }
}
