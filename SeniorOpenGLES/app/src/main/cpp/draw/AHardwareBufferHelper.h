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
#include <ShaderHelper.h>
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

	GLuint m_VAO;
	GLuint m_VBO[3];

	ShaderHelper *m_pShaderHelper;

	GLuint mTextureColorId;
	GLuint mOESTextureId;
	GLuint mDstFBO;
	int mWidth;
	int mHeight;
	int mRenderNum;

	void convertImageFormat2Hardware(const int srcFormat, int &dstFormat);
	void convertHardwareFormat2Image(const int srcFormat, int &dstFormat);
	void setCreateState (const bool state);
	void setHardwareContext();
	void initDstOesTextureId();
	void initDstOesFbo();
	void connectDstTextureIdToImageBuffer();
	int getGPUBufferData(LPMyImageInfo lpMyImageInfo);
	int initGLBuffer ();
	void unInitGLBuffer ();

public:
	AHardwareBufferHelper ();
	~AHardwareBufferHelper();
	int createGPUBuffer (const int nWidth, const int nHeight, const int format);
	void destroyGPUBuffer ();
	int onDrawFrame (const GLuint colorTextureId, LPMyImageInfo lpMyImageInfo);
	bool getCreateState();
	int getRenderNum ();
};

#endif //SENIOROPENGLES_AHARDWAREBUFFERHELPER_H
