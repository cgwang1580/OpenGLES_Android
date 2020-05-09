//
// Created by wcg3031 on 2020/4/21.
//

#ifndef SENIOROPENGLES_DRAWHELPER_H
#define SENIOROPENGLES_DRAWHELPER_H

#include "GLES3/gl3.h"
#include "EGL/egl.h"
#include <string>
#include "OpenImageHelper.h"

class DrawHelper
{
public:
	static void GetOneTexture(const GLenum target, GLuint *pTexture)
	{
		LOGD("getOneTexture");
		glGenTextures(1, pTexture);
		glBindTexture(target, *pTexture);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(target, GL_NONE);
	}

	static void CheckGLError(const char *TAG)
	{
		GLenum error = glGetError();
		if (EGL_SUCCESS != error)
			LOGE("%s CheckGLError error_code = %d", TAG, error);
	}

	static void CheckEGLError(const char *TAG)
	{
		EGLint error = eglGetError();
		if (EGL_SUCCESS != error)
			LOGE("%s CheckEGLError error_code = %d", TAG, error);
	}

	static void SaveRenderImage (const SRECT sRect, const GLenum format, std::string sPath)
	{
		LOGD("SaveRenderImage");
		LOGD("SaveRenderImage sRect = (%d,%d,%d,%d), format = %d, sPath = %s", sRect.left, sRect.top,
				sRect.right, sRect.bottom, format, sPath.c_str());
		int width = sRect.right - sRect.left;
		int height = sRect.bottom - sRect.top;
		MyImageInfo myImageInfo {0};
		myImageInfo.width = width;
		myImageInfo.height = height;
		switch (format)
		{
			case GL_RGB:
				myImageInfo.format = MY_FORMAT_RGB;
				break;
			case GL_RGBA:
				myImageInfo.format = MY_FORMAT_RGBA;
				break;
			default:
				myImageInfo.format = MY_FORMAT_RGB;
				break;
		}
		myImageInfo.channel[0] = myImageInfo.width;
		OpenImageHelper::AllocMyImageInfo(&myImageInfo);
		START_TIME ("SaveRenderImage glReadPixels")
			glReadPixels(0, 0, width, height, format, GL_UNSIGNED_BYTE, myImageInfo.buffer[0]);
		STOP_TIME ("SaveRenderImage glReadPixels")
		OpenImageHelper::ExchangeImageCoordinateY(&myImageInfo);
		OpenImageHelper::SaveImageToPng(&myImageInfo, sPath.c_str());
		OpenImageHelper::FreeMyImageInfo(&myImageInfo);
	}
};

#endif //SENIOROPENGLES_DRAWHELPER_H
