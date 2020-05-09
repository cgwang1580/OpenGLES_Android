//
// Created by wcg3031 on 2020/5/7.
//

#include "AHardwareBufferHelper.h"
#include "DrawHelper.h"
#include "MyDefineUtils.h"
#include "LogAndroid.h"
#include "cstring"

AHardwareBufferHelper::AHardwareBufferHelper() :
		pEGLDisplay(nullptr),
		pEGLContext(nullptr),
		pEGLImageKHR(nullptr),
		pAHardwareBuffer(nullptr),
		bCreated(false),
		mOESTextureId(0)
{
	LOGD("GraphicBufferHelper");
	USAGE = (AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN|AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN);
}

AHardwareBufferHelper::~AHardwareBufferHelper()
{
	LOGD("~GraphicBufferHelper");
	setCreateState(false);
}

int AHardwareBufferHelper::createGPUBuffer (const int nWidth, const int nHeight, const int format, const GLuint textureId)
{
	LOGD("createGPUBuffer");
	int ret;
	if (bCreated)
		return ERROR_OK;
	AHardwareBuffer_Desc aBufferDesc{0};
	aBufferDesc.width = (uint32_t)nWidth;
	aBufferDesc.height = (uint32_t)nHeight;
	aBufferDesc.layers = 1;
	aBufferDesc.format = (uint32_t)format;
	aBufferDesc.usage = (uint32_t)USAGE;
	aBufferDesc.stride = (uint32_t)aBufferDesc.width;
	aBufferDesc.rfu0 = 0;
	aBufferDesc.rfu1 = 0;

	ret = AHardwareBuffer_allocate (&aBufferDesc, &pAHardwareBuffer);
	LOGD("AHardwareBuffer_allocate ret = %d", ret);
	if (ERROR_OK != ret)
	{
		LOGE("AHardwareBuffer_allocate error");
		return ret;
	}
	LOGD("AHardwareBuffer_allocate pAHardwareBuffer = %p", pAHardwareBuffer);

	EGLClientBuffer eglClientBuffer = eglGetNativeClientBufferANDROID (pAHardwareBuffer);
	DrawHelper::CheckEGLError("eglGetNativeClientBufferANDROID");

	pEGLImageKHR = eglCreateImageKHR (pEGLDisplay, pEGLContext, EGL_NATIVE_BUFFER_ANDROID, eglClientBuffer, nullptr);
	DrawHelper::CheckEGLError("eglCreateImageKHR");
	if (EGL_NO_IMAGE_KHR == pEGLImageKHR)
	{
		LOGE("eglCreateImageKHR error");
		return ERROR_GL_STATUS;
	}

	glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
	DrawHelper::CheckEGLError("glBindTexture");
	glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)pEGLImageKHR);
	DrawHelper::CheckEGLError("glEGLImageTargetTexture2DOES");
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	DrawHelper::CheckEGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	DrawHelper::CheckEGLError("glTexParameteri");
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

	setCreateState(true);

	return ret;
}

void AHardwareBufferHelper::destroyGPUBuffer ()
{
	LOGD("destroyGPUBuffer");
	AHardwareBuffer_release(pAHardwareBuffer);
	pAHardwareBuffer = nullptr;
	eglDestroyImageKHR(pEGLDisplay, pEGLImageKHR);
	pEGLDisplay = nullptr;
	pEGLImageKHR = nullptr;
	setCreateState(false);
}

int AHardwareBufferHelper::getGPUBufferDate(LPMyImageInfo lpMyImageInfo)
{
	LOGD("getGPUBufferDate");
	CHECK_NULL_INPUT(lpMyImageInfo)

	int fence = -1;
	unsigned char *pSrcPlane = nullptr;
	int ret = AHardwareBuffer_lock (pAHardwareBuffer, USAGE, fence, NULL, (void **)&pSrcPlane);
	LOGD("getGPUBufferDate AHardwareBuffer_lock ret = %d", ret);
	if (ERROR_OK != ret)
	{
		LOGE("getGPUBufferDate AHardwareBuffer_lock error");
		return ret;
	}

	AHardwareBuffer_Desc aOutBufferDesc{0};
	AHardwareBuffer_describe (pAHardwareBuffer, &aOutBufferDesc);
	LOGD("getGPUBufferDate AHardwareBuffer_describe width = %d, height = %d, format = %d",
			aOutBufferDesc.width, aOutBufferDesc.height, aOutBufferDesc.format);
	lpMyImageInfo->width = aOutBufferDesc.width;
	lpMyImageInfo->height = aOutBufferDesc.height;
	convertHardwareFormat2Image(aOutBufferDesc.format, lpMyImageInfo->format);
	lpMyImageInfo->channel[0] = aOutBufferDesc.stride;
	long lSize = 0;
	if (NULL == lpMyImageInfo->buffer[0])
	{
		lSize = OpenImageHelper::AllocMyImageInfo(lpMyImageInfo);
		LOGD("getGPUBufferDate AllocMyImageInfo lSize = %ld", lSize);
		if (0 == lSize)
			return ERROR_IMAGE;
	}
	memcpy(lpMyImageInfo->buffer[0], pSrcPlane, (size_t)lSize);
	ret = AHardwareBuffer_unlock(pAHardwareBuffer, &fence);
	LOGD("getGPUBufferDate AHardwareBuffer_unlock ret = %d", ret);
	if (ERROR_OK != ret)
	{
		LOGE("getGPUBufferDate AHardwareBuffer_unlock error");
		return ret;
	}
	return ERROR_OK;
}

bool AHardwareBufferHelper::getCreateState()
{
	return bCreated;
}

void AHardwareBufferHelper::convertImageFormat2Hardware(const int srcFormat, int &dstFormat)
{
	LOGD("convertImageFormat2Hardware srcFormat = %d", srcFormat);
	switch (srcFormat)
	{
		case MY_FORMAT_RGBA:
			dstFormat = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM;
			break;
		case MY_FORMAT_RGB:
			dstFormat = AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM;
			break;
		case MY_FORMAT_NV21:
		case MY_FORMAT_NV12:
			dstFormat = AHARDWAREBUFFER_FORMAT_Y8Cb8Cr8_420;
			break;
		default:
			break;
	}
	LOGD("convertImageFormat2Hardware dstFormat = %d", dstFormat);
}

void AHardwareBufferHelper::convertHardwareFormat2Image(const int srcFormat, int &dstFormat)
{
	LOGD("convertHardwareFormat2Image srcFormat = %d", srcFormat);
	switch (srcFormat)
	{
		case AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM:
			dstFormat = MY_FORMAT_RGBA;
			break;
		case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM:
			dstFormat = MY_FORMAT_RGB;
			break;
		case AHARDWAREBUFFER_FORMAT_Y8Cb8Cr8_420:
			dstFormat = MY_FORMAT_NV21;
			break;
		default:
			break;
	}
	LOGD("convertHardwareFormat2Image dstFormat = %d", dstFormat);
}

void AHardwareBufferHelper::setCreateState (const bool state)
{
	LOGD("setCreateState state = %d", state);
	bCreated = state;
}