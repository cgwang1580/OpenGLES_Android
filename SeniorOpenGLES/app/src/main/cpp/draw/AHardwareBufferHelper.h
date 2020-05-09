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

public:
	AHardwareBufferHelper ();
	~AHardwareBufferHelper();

	int createGPUBuffer (const int nWidth, const int nHeight, const int format, const GLuint textureId);
	void destroyGPUBuffer ();
	int getGPUBufferDate(LPMyImageInfo lpMyImageInfo);
	bool getCreateState();
};

#endif //SENIOROPENGLES_AHARDWAREBUFFERHELPER_H
