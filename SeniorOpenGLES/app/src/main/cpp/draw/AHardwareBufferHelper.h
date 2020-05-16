//
// Created by wcg3031 on 2020/5/7.
//

#ifndef SENIOROPENGLES_AHARDWAREBUFFERHELPER_H
#define SENIOROPENGLES_AHARDWAREBUFFERHELPER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <android/hardware_buffer.h>
#include "MyImageInfo.h"

/**
 * environment
 * link EGL android GLESv3 lib
 * __ANDROID_API__ >= 26
 */

typedef enum {
	MY_AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNKNOWN = 5,
	MY_AHARDWAREBUFFER_FORMAT_YCrCb_420_SP = 0x11
}MY_DEFINE_HARDWAREBUFFER_FORMAT;

class AHardwareBufferHelper
{
private:
	EGLDisplay pEGLDisplay;
	EGLContext pEGLContext;
	EGLImageKHR pEGLImageKHR;
	AHardwareBuffer *pAHardwareBuffer;
	bool bCreated;
	uint64_t USAGE;

	GLuint mOESTextureId;

	void convertImageFormat2Hardware(const int srcFormat, int &dstFormat);
	void convertHardwareFormat2Image(const int srcFormat, int &dstFormat);
	void setCreateState (const bool state);
	void setHardwareContext();

public:
	AHardwareBufferHelper ();
	~AHardwareBufferHelper();

	int createGPUBuffer (const int nWidth, const int nHeight, const int format, const GLuint textureId);
	void destroyGPUBuffer ();
	int getGPUBufferData(LPMyImageInfo lpMyImageInfo);
	bool getCreateState();
};

#endif //SENIOROPENGLES_AHARDWAREBUFFERHELPER_H
