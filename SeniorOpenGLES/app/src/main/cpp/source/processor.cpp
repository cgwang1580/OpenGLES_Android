//
// Created by wcg3031 on 2020/4/1.
//

#include "processor_inner.h"
#include "LogAndroid.h"
#include "GLES3/gl3.h"
#include <string>
#include <MyDefineUtils.h>
#include <OpenImageHelper.h>
#include <SampleRender3D.h>
#include "shader_content.h"
#include "unistd.h"
#include "draw_utils.h"

#define TEST_IMAGE_PATH_0	"/sdcard/OpenGLESTest/television.png"
#define TEST_IMAGE_PATH_1	"/sdcard/OpenGLESTest/testlib.png"
#define TEST_IMAGE_PATH_YUV_0	"/sdcard/OpenGLESTest/Img_test_1_640X480.nv12"

int onSurfaceCreated (PHandle *ppProcessorHandle)
{
	LOGD("onSurfaceCreated");

	if (nullptr == ppProcessorHandle || nullptr != *ppProcessorHandle)
	{
		LOGE("onSurfaceCreated ppProcessorHandle is NULL");
		return ERROR_INPUT;
	}

	*ppProcessorHandle = (LPProcessorHandle)malloc (sizeof (ProcessorHandle));
	CHECK_NULL_MALLOC (*ppProcessorHandle);
	LPProcessorHandle MyProcessorHandle = (LPProcessorHandle)*ppProcessorHandle;
	memset(MyProcessorHandle, 0, sizeof(ProcessorHandle));

	/*int ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetTriangle, triangle_vertex_shader, triangle_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetTriangle ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetTexture, texture_vertex_shader, texture_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetTexture ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetFBO, fbo_vertex_shader, fbo_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetFBO ret = %d", ret);

	ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetFBONormal, fbo_vertex_shader, fbo_normal_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetFBONormal ret = %d", ret);

	*//*ret = CreateShaderHelper(&MyProcessorHandle->mShaderSetHardwareNormal, hardware_vertex_shader, hardware_normal_fragment_shader);
	LOGD("onSurfaceCreated CreateShaderHelper mShaderSetFBONormal ret = %d", ret);*//*

	if (nullptr != MyProcessorHandle->lpMyImageInfo)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo);
	}
	MyProcessorHandle->lpMyImageInfo = (LPMyImageInfo)malloc(sizeof(MyImageInfo));
	CHECK_NULL_MALLOC(MyProcessorHandle->lpMyImageInfo);
	memset (MyProcessorHandle->lpMyImageInfo, 0 , sizeof(MyImageInfo));

	if (nullptr != MyProcessorHandle->lpMyImageInfo_YUV)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo_YUV);
	}
	MyProcessorHandle->lpMyImageInfo_YUV = (LPMyImageInfo)malloc(sizeof(MyImageInfo));
	CHECK_NULL_MALLOC(MyProcessorHandle->lpMyImageInfo_YUV);
	memset(MyProcessorHandle->lpMyImageInfo_YUV, 0, sizeof(MyImageInfo));

	*//*if (nullptr == MyProcessorHandle->pHardwareBufferHelper)
	{
		MyProcessorHandle->pHardwareBufferHelper = new AHardwareBufferHelper();
	}*//*

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (nullptr == MyProcessorHandle->m_pSampleTransform)
	{
		MyProcessorHandle->m_pSampleTransform = new SampleTransform ();
		MyProcessorHandle->m_pSampleTransform->InitSample();
	}*/

	if (nullptr == MyProcessorHandle->m_pSampleRender3D)
	{
		MyProcessorHandle->m_pSampleRender3D = new SampleRender3D ();
		MyProcessorHandle->m_pSampleRender3D->InitSample();
	}

	return ERROR_OK;
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (nullptr != MyProcessorHandle->lpMyImageInfo && nullptr == MyProcessorHandle->lpMyImageInfo->buffer[0])
	{
		OpenImageHelper::LoadPngFromFile(TEST_IMAGE_PATH_0, MyProcessorHandle->lpMyImageInfo);
		//OpenImageHelper::SaveImageToPng (MyProcessorHandle->lpMyImageInfo, "/sdcard/OpenGLESTest/testpng.png");
	}

	if (nullptr != MyProcessorHandle->lpMyImageInfo_YUV && nullptr == MyProcessorHandle->lpMyImageInfo_YUV->buffer[0])
	{
		OpenImageHelper::LoadYuvFromFile(TEST_IMAGE_PATH_YUV_0, MyProcessorHandle->lpMyImageInfo_YUV);
		/*char sPath[MAX_PATH]{0};
		sprintf(sPath, "/sdcard/OpenGLESTest/Save_0_%dx%d.nv12", MyProcessorHandle->lpMyImageInfo_YUV->channel[0],
				MyProcessorHandle->lpMyImageInfo_YUV->height);
		OpenImageHelper::SaveImageToYuv(MyProcessorHandle->lpMyImageInfo_YUV, sPath);*/

	}
	int nDrawType = 5;
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
		case 3:
			ret = drawByHardwareBuffer(nullptr, MyProcessorHandle->pHardwareBufferHelper, MyProcessorHandle->lpMyImageInfo);
			LOGD("onDrawFrame drawByHardwareBuffer ret = %d", ret);
			break;
		case 4:
			ret = MyProcessorHandle->m_pSampleTransform->OnDrawFrame();
			break;
		case 5:
			ret = MyProcessorHandle->m_pSampleRender3D->OnDrawFrame();
			break;
		default:
			LOGD("onDrawFrame nDrawType = %d", nDrawType);
			break;
	}
	sleep(1);
	return ret;
}

int onSurfaceDestroyed (PHandle *ppProcessorHandle)
{
	LOGD("onSurfaceDestroyed");

	CHECK_NULL_INPUT (*ppProcessorHandle);
	LPProcessorHandle MyProcessorHandle = (LPProcessorHandle)*ppProcessorHandle;

	if (MyProcessorHandle->m_pSampleTransform)
	{
		MyProcessorHandle->m_pSampleTransform->UnInitSample();
		SafeDelete(MyProcessorHandle->m_pSampleTransform);
	}

	if (MyProcessorHandle->m_pSampleRender3D)
	{
		MyProcessorHandle->m_pSampleRender3D->UnInitSample();
		SafeDelete(MyProcessorHandle->m_pSampleRender3D);
	}

	SafeDelete(MyProcessorHandle->mShaderSetTriangle.pShaderHelper);
	SafeDelete(MyProcessorHandle->mShaderSetTexture.pShaderHelper);
	SafeDelete(MyProcessorHandle->mShaderSetFBO.pShaderHelper);
	SafeDelete(MyProcessorHandle->mShaderSetFBONormal.pShaderHelper);
	//SafeDelete(MyProcessorHandle->mShaderSetHardwareNormal.pShaderHelper);
	SafeDelete (MyProcessorHandle->mShaderSetTriangle.pShaderHelper);
	SafeDelete (MyProcessorHandle->mShaderSetTexture.pShaderHelper);
	SafeDelete (MyProcessorHandle->mShaderSetFBO.pShaderHelper);
	SafeDelete (MyProcessorHandle->mShaderSetFBONormal.pShaderHelper);
	if (MyProcessorHandle->pHardwareBufferHelper && MyProcessorHandle->pHardwareBufferHelper->getCreateState())
	{
		MyProcessorHandle->pHardwareBufferHelper->destroyGPUBuffer();
	}
	SafeDelete (MyProcessorHandle->pHardwareBufferHelper);
	if (nullptr != MyProcessorHandle->lpMyImageInfo)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo);
		SafeFree(MyProcessorHandle->lpMyImageInfo);
	}
	if (nullptr != MyProcessorHandle->lpMyImageInfo_YUV)
	{
		OpenImageHelper::FreeMyImageInfo(MyProcessorHandle->lpMyImageInfo_YUV);
		SafeFree(MyProcessorHandle->lpMyImageInfo_YUV);
	}
	SafeFree (MyProcessorHandle);

	return 0;
}