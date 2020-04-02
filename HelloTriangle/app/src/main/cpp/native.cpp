#include <jni.h>
#include <string>
#include "header_out/processor.h"

PHandle pProcessorHandle = NULL;

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_hellotriangle_MyGLRenderer_onSurfaceCreatedJNI(JNIEnv *env, jobject thiz)
{
    // TODO: implement onSurfaceCreatedJNI()
    int ret = onSurfaceCreated(&pProcessorHandle);
	return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_hellotriangle_MyGLRenderer_onSurfaceChangedJNI(JNIEnv *env, jobject thiz, jint width, jint height)
{
    // TODO: implement onSurfaceChangedJNI()
    int ret = onSurfaceChanged(pProcessorHandle, width, height);
    return ret;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_hellotriangle_MyGLRenderer_onDrawFrameJNI(JNIEnv *env, jobject thiz)
{
    // TODO: implement onDrawFrameJNI()
    int ret = onDrawFrame(pProcessorHandle);
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_example_hellotriangle_MyGLRenderer_onSurfaceDestroyedJNI(JNIEnv *env, jobject thiz)
{
	// TODO: implement onSurfaceDestroyedJNI()
    int ret = onSurfaceDestroyed(&pProcessorHandle);
    return ret;
}