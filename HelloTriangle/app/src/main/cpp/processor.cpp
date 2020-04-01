//
// Created by wcg3031 on 2020/4/1.
//

#include "processor.h"
#include "LogAndroid.h"
#include "GLES3/gl3.h"
#include <string>
#include <MyDefine.h>
#include "unistd.h"

int onSurfaceCreated (LPPROCESSOR_HANDLE *ppProcessorHandle)
{
	MYLOGD("onSurfaceCreated");

	if (NULL == ppProcessorHandle || NULL != *ppProcessorHandle)
	{
		MYLOGE("onSurfaceCreated ppProcessorHandle is not NULL");
		return ERROR_INPUT;
	}

	*ppProcessorHandle = (LPPROCESSOR_HANDLE)malloc (sizeof (PROCESSOR_HANDLE));
	CHECK_NULL (*ppProcessorHandle);
	LPPROCESSOR_HANDLE MyProcessorHandle = *ppProcessorHandle;
	memset(MyProcessorHandle, 0, sizeof(PROCESSOR_HANDLE));

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return 0;
}

int onSurfaceChanged (const LPPROCESSOR_HANDLE pProcessorHandle)
{
	MYLOGD("onSurfaceChanged");
	return 0;
}

int onDrawFrame (const LPPROCESSOR_HANDLE pProcessorHandle)
{
	MYLOGD("onDrawFrame");
	CHECK_NULL(pProcessorHandle)
	++pProcessorHandle->mRenderTime;

	float r = 0;
	float g = 0;
	float b = 0;

	pProcessorHandle->r += 0.1;
	pProcessorHandle->g += 0.1;
	pProcessorHandle->b += 0.1;

	r = pProcessorHandle->r;
	g = pProcessorHandle->g;
	b = pProcessorHandle->b;
	r = r > 1 ? 1.0f : r;
	g = g > 1 ? 1.0f : g;
	b = b > 1 ? 1.0f : b;
	MYLOGD("onDrawFrame r = %f, g = %f, b = %f", r, g, b);
	glClearColor(r, g, b, 1.0);
	//glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sleep(1);
	return 0;
}

int onSurfaceDestroyed (LPPROCESSOR_HANDLE *ppProcessorHandle)
{
	MYLOGD("onSurfaceDestroyed");

	CHECK_NULL(*ppProcessorHandle);
	LPPROCESSOR_HANDLE MyProcessorHandle = *ppProcessorHandle;
	SafeFree (MyProcessorHandle);

	return 0;
}