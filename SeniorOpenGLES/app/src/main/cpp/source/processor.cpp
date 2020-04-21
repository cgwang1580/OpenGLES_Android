//
// Created by wcg3031 on 2020/4/1.
//

#include "processor_inner.h"
#include "LogAndroid.h"
#include "GLES3/gl3.h"
#include <string>
#include <MyDefineUtils.h>
#include "../shader/shader_content.h"
#include "unistd.h"
#include "draw_utils.h"

int onSurfaceCreated (PHandle *ppProcessorHandle)
{
	LOGD("onSurfaceCreated");

	if (NULL == ppProcessorHandle || NULL != *ppProcessorHandle)
	{
		LOGE("onSurfaceCreated ppProcessorHandle is NULL");
		return ERROR_INPUT;
	}

	*ppProcessorHandle = (LPProcessorHandle)malloc (sizeof (ProcessorHandle));
	CHECK_NULL_MALLOC (*ppProcessorHandle);
	LPProcessorHandle MyProcessorHandle = (LPProcessorHandle)*ppProcessorHandle;
	memset(MyProcessorHandle, 0, sizeof(ProcessorHandle));

	int ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetTriangle, triangle_vertex_shader, triangle_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetTriangle ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetTexture, texture_vertex_shader, texture_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetTexture ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetFBO, fbo_vertex_shader, fbo_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetFBO ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetFBONormal, fbo_vertex_shader, fbo_normal_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetFBONormal ret = %d", ret);

	if (NULL != MyProcessorHandle->lpMyImageInfo)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo);
	}
	MyProcessorHandle->lpMyImageInfo = (LPMyImageInfo)malloc(sizeof(MyImageInfo));
	CHECK_NULL_MALLOC(MyProcessorHandle->lpMyImageInfo);
	memset (MyProcessorHandle->lpMyImageInfo, 0 , sizeof(MyImageInfo));

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return ret;
}

int onSurfaceChanged (const PHandle pProcessorHandle, const int width, const int height)
{
	LOGD("onSurfaceChanged");
	glViewport(0, 0, width, height);
	return 0;
}

int onDrawFrame (const PHandle pProcessorHandle)
{
	LOGD("onDrawFrame");
	CHECK_NULL_INPUT (pProcessorHandle)
	LPProcessorHandle MyProcessorHandle = (LPProcessorHandle)pProcessorHandle;
	++MyProcessorHandle->mRenderTime;

	int ret = ERROR_OK;
	/*float r = 0;
	float g = 0;
	float b = 0;
	MyProcessorHandle->mColorSet.alpha = 1.0;
	float alpha = MyProcessorHandle->mColorSet.alpha;

	MyProcessorHandle->mColorSet.r += 0.1;
	MyProcessorHandle->mColorSet.g += 0.1;
	MyProcessorHandle->mColorSet.b += 0.1;

	r = MyProcessorHandle->mColorSet.r;
	g = MyProcessorHandle->mColorSet.g;
	b = MyProcessorHandle->mColorSet.b;
	r = r > 1 ? 1.0f : r;
	g = g > 1 ? 1.0f : g;
	b = b > 1 ? 1.0f : b;
	LOGD("onDrawFrame r = %f, g = %f, b = %f, alpha = %f", r, g, b, alpha);
	glClearColor(r, g, b, alpha);*/
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (NULL != MyProcessorHandle->lpMyImageInfo && NULL == MyProcessorHandle->lpMyImageInfo->buffer[0])
	{
		char IMAGE_PATH [MAX_PATH] {"/sdcard/OpenGLESTest/testlib.png"};
		OpenImageHelper::LoadPngFromFile(IMAGE_PATH, MyProcessorHandle->lpMyImageInfo);
		//OpenImageHelper::SaveImageToPng (MyProcessorHandle->lpMyImageInfo, "/sdcard/OpenGLESTest/testpng.png");
	}
	int nDrawType = 2;

	switch (nDrawType)
	{
		case 0:
			ret = drawTriangle (MyProcessorHandle->mShaderSetTriangle.pShaderHelper);
			LOGD("onDrawFrame drawTriangle ret = %d", ret);
			break;
		case 1:
			ret = drawTexture(MyProcessorHandle->mShaderSetTexture.pShaderHelper, MyProcessorHandle->lpMyImageInfo);
			LOGD("onDrawFrame drawTexture ret = %d", ret);
			break;
		case 2:
			ret = drawFBO(MyProcessorHandle->mShaderSetFBO.pShaderHelper, MyProcessorHandle->mShaderSetFBONormal.pShaderHelper,
						  MyProcessorHandle->lpMyImageInfo);
			LOGD("onDrawFrame drawFBO ret = %d", ret);
			break;
		default:
			break;
	}
	sleep(1);
	return 0;
}

int onSurfaceDestroyed (PHandle *ppProcessorHandle)
{
	LOGD("onSurfaceDestroyed");

	CHECK_NULL_INPUT (*ppProcessorHandle);
	LPProcessorHandle MyProcessorHandle = (LPProcessorHandle)*ppProcessorHandle;
	SafeDelete (MyProcessorHandle->mShaderSetTriangle.pShaderHelper);
	SafeDelete (MyProcessorHandle->mShaderSetTexture.pShaderHelper);
	if (NULL != MyProcessorHandle->lpMyImageInfo)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo);
		SafeFree(MyProcessorHandle->lpMyImageInfo);
	}
	SafeFree (MyProcessorHandle);

	return 0;
}