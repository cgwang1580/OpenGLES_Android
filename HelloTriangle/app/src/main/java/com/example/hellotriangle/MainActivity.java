package com.example.hellotriangle;

import android.Manifest;
import android.content.Context;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.PermissionChecker;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.accessibility.AccessibilityManager;
import android.widget.TextView;
import android.widget.Toast;

import com.example.utils.PermissionHelper;
import com.example.utils.PermissionInterface;

public class MainActivity extends AppCompatActivity {

    private final static String PROCESSOR_NAME = "processor.draw";

    static {
        System.loadLibrary(PROCESSOR_NAME);
    }

    private final static String TAG = "MainActivity";

    private final static String []PermissionList = new String []{Manifest.permission.READ_EXTERNAL_STORAGE};

    MyGLSurfaceView myGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        requestPermission ();

        if (null == myGLSurfaceView) {
            InitGLSurfaceView(this);
        }
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
