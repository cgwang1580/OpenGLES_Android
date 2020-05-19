//
// Created by wcg3031 on 2020/5/7.
//

#include "AHardwareBufferHelper.h"
#include "DrawHelper.h"
#include "MyDefineUtils.h"
#include "LogAndroid.h"
#include "cstring"

const GLfloat vVertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
};

const GLfloat vTexCoors[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
};

const GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

AHardwareBufferHelper::AHardwareBufferHelper() :
		pEGLDisplay(nullptr),
		pEGLContext(nullptr),
		pEGLImageKHR(nullptr),
		pAHardwareBuffer(nullptr),
		bCreated(false),
		mTextureColorId(GL_NONE),
		mOESTextureId(GL_NONE),
		mDstFBO (GL_NONE),
		m_VAO(GL_NONE)
{
	LOGD("GraphicBufferHelper");
	USAGE = (AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN|AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN);
	memset(m_VBO, 0, sizeof(m_VBO));
}

AHardwareBufferHelper::~AHardwareBufferHelper()
{
	LOGD("~GraphicBufferHelper");
	setCreateState(false);
}

int AHardwareBufferHelper::createGPUBuffer (const int nWidth, const int nHeight, const int format)
{
	LOGD("createGPUBuffer");
	int ret = ERROR_OK;
	if (bCreated)
		return ERROR_OK;
	AHardwareBuffer_Desc aBufferDesc{0};
	aBufferDesc.width = (uint32_t)nWidth;
	aBufferDesc.height = (uint32_t)nHeight;
	aBufferDesc.layers = 1;
	int nBufferFormat = 0;
	convertImageFormat2Hardware(format, nBufferFormat);
	aBufferDesc.format = (uint32_t)nBufferFormat;
	aBufferDesc.usage = (uint32_t)USAGE;
	aBufferDesc.stride = (uint32_t)aBufferDesc.width;
	aBufferDesc.rfu0 = 0;
	aBufferDesc.rfu1 = 0;

	mWidth = aBufferDesc.width;
	mHeight = aBufferDesc.height;

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

	setHardwareContext();
	pEGLImageKHR = eglCreateImageKHR (pEGLDisplay, pEGLContext, EGL_NATIVE_BUFFER_ANDROID, eglClientBuffer, nullptr);
	DrawHelper::CheckEGLError("eglCreateImageKHR");
	if (EGL_NO_IMAGE_KHR == pEGLImageKHR)
	{
		LOGE("eglCreateImageKHR error");
		return ERROR_GL_STATUS;
	}

	initGLBuffer();
	initDstOesTextureId();
	connectDstTextureIdToImageBuffer();
	initDstOesFbo();

	setCreateState(true);

	return ret;
}

void AHardwareBufferHelper::destroyGPUBuffer ()
{
	LOGD("destroyGPUBuffer");
	if (pAHardwareBuffer)
	{
		AHardwareBuffer_release(pAHardwareBuffer);
		pAHardwareBuffer = nullptr;
	}
	if (pEGLImageKHR)
	{
		eglDestroyImageKHR(pEGLDisplay, pEGLImageKHR);
		pEGLImageKHR = nullptr;
	}
	pEGLDisplay = nullptr;
	if (mTextureColorId)
		glDeleteTextures(1, &mTextureColorId);
	if(mOESTextureId)
		glDeleteTextures(1, &mOESTextureId);
	if (mDstFBO)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mDstFBO);
		glDeleteFramebuffers(1, &mDstFBO);
	}
	unInitGLBuffer();
	setCreateState(false);
}

int AHardwareBufferHelper::getGPUBufferData(LPMyImageInfo lpMyImageInfo)
{
	LOGD("getGPUBufferDate");
	CHECK_NULL_INPUT(lpMyImageInfo)

	int fence = -1;
	unsigned char *pSrcPlane = nullptr;
	int ret = AHardwareBuffer_lock (pAHardwareBuffer, USAGE, fence, NULL, (void **)&pSrcPlane);
	LOGD("getGPUBufferDate AHardwareBuffer_lock ret = %d", ret);
	if (ERROR_OK != ret || nullptr == pSrcPlane)
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

int AHardwareBufferHelper::initGLBuffer ()
{
	LOGD("initGLBuffer");

	glGenBuffers(sizeof(m_VBO)/sizeof(GLuint), m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	/// !!! need call glBindBuffer to bind GL_ELEMENT_ARRAY_BUFFER again
	/// before glBindVertexArray GL_NONE !!!///
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[2]);
	glBindVertexArray(GL_NONE);

	return ERROR_OK;
}

void AHardwareBufferHelper::unInitGLBuffer ()
{
	LOGD("unInitGLBuffer");
	if (m_VAO)
	{
		glDeleteBuffers(1, &m_VAO);
	}
	for (auto val:m_VBO)
	{
		if (val)
			glDeleteBuffers(1, &val);
	}
}

int AHardwareBufferHelper::onDrawFrame (const GLuint colorTextureId, LPMyImageInfo lpMyImageInfo)
{
	LOGD("onDrawFrame colorTextureId = %d", colorTextureId);
	mTextureColorId = colorTextureId;

	GLint oriFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oriFbo);
	DrawHelper::CheckGLError("onDrawFrame glGetIntegerv");
	GLint viewPorts[4] = {0};
	glGetIntegerv(GL_VIEWPORT, viewPorts);
	DrawHelper::CheckGLError("onDrawFrame glGetIntegerv");

	glBindFramebuffer(GL_FRAMEBUFFER, mDstFBO);
	DrawHelper::CheckGLError("onDrawFrame glBindFramebuffer");
	glViewport(0, 0, mWidth, mHeight);
	DrawHelper::CheckGLError("onDrawFrame glViewport");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DrawHelper::CheckGLError("onDrawFrame glClearColor");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawHelper::CheckGLError("onDrawFrame glClear");

	glBindTexture(GL_TEXTURE_2D, mTextureColorId);
	DrawHelper::CheckGLError("onDrawFrame glBindTexture");

	glBindVertexArray(m_VAO);
	DrawHelper::CheckGLError("onDrawFrame glBindVertexArray");
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)0);
	DrawHelper::CheckGLError("onDrawFrame glDrawElements");
	glBindVertexArray(GL_NONE);
	DrawHelper::CheckGLError("onDrawFrame glBindVertexArray");

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	DrawHelper::CheckGLError("onDrawFrame glBindTexture");
	glBindFramebuffer(GL_FRAMEBUFFER, oriFbo);
	DrawHelper::CheckGLError("onDrawFrame glBindFramebuffer");
	glViewport(viewPorts[0], viewPorts[1], viewPorts[2], viewPorts[3]);
	DrawHelper::CheckGLError("onDrawFrame glViewport");

	int ret = ERROR_OK;
	START_TIME("getGPUBufferDate")
		ret = getGPUBufferData(lpMyImageInfo);
	STOP_TIME("getGPUBufferDate")
	LOGE("drawByHardwareBuffer getGPUBufferDate ret = %d", ret);
	return ret;
}

bool AHardwareBufferHelper::getCreateState()
{
	return bCreated;
}

void AHardwareBufferHelper::initDstOesTextureId ()
{
	LOGD("initDstOesTextureId");
	if (GL_NONE != mOESTextureId)
	{
		return;
	}
	const GLenum targetOES = GL_TEXTURE_EXTERNAL_OES;
	DrawHelper::GetOneTexture(targetOES, &mOESTextureId);
	DrawHelper::CheckGLError("initDstOesTextureId GetOneTexture");
}

void AHardwareBufferHelper::initDstOesFbo()
{
	LOGD("initDstOesFbo");
	const GLenum TargetOES = GL_TEXTURE_EXTERNAL_OES;

	glBindTexture(TargetOES, mOESTextureId);
	DrawHelper::CheckGLError("initDstOesFbo GetOneTexture");

	glGenFramebuffers(1, &mDstFBO);
	DrawHelper::CheckGLError("initDstOesFbo glGenFramebuffers");


	const GLenum TargetFrameBuffer = GL_FRAMEBUFFER;
	glBindFramebuffer(TargetFrameBuffer, mDstFBO);
	DrawHelper::CheckGLError("initDstOesFbo glBindFramebuffer");

	GLint oriFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oriFbo);
	DrawHelper::CheckGLError("initDstOesFbo glGetIntegerv");
	glBindFramebuffer(TargetFrameBuffer, mDstFBO);
	DrawHelper::CheckGLError("initDstOesFbo glBindFramebuffer");
	glFramebufferTexture2D(TargetFrameBuffer, GL_COLOR_ATTACHMENT0, TargetOES, mOESTextureId, 0);
	DrawHelper::CheckGLError("initDstOesFbo glFramebufferTexture2D");

	// check frame buffer state
	GLenum tmpStatus = glCheckFramebufferStatus(TargetFrameBuffer);
	if (GL_FRAMEBUFFER_COMPLETE != tmpStatus)
	{
		LOGE("initDstOesFbo glCheckFramebufferStatus tmpStatus = %d", tmpStatus);
		return;
	}
	DrawHelper::CheckGLError("initDstOesFbo glCheckFramebufferStatus");
	glBindFramebuffer(GL_FRAMEBUFFER, oriFbo);

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	DrawHelper::CheckGLError("initDstOesFbo glClearColor");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawHelper::CheckGLError("initDstOesFbo glClear");

	glBindTexture(TargetOES, GL_NONE);
	DrawHelper::CheckGLError("initDstOesFbo glBindTexture");
	glBindFramebuffer(TargetFrameBuffer, oriFbo);
	DrawHelper::CheckGLError("initDstOesFbo glBindFramebuffer");
}

void AHardwareBufferHelper::connectDstTextureIdToImageBuffer()
{
	LOGD("connectDstTextureIdToImageBuffer");
	const GLenum TargetOES = GL_TEXTURE_EXTERNAL_OES;
	glBindTexture(TargetOES, mOESTextureId);
	DrawHelper::CheckEGLError("connectDstTextureIdToImageBuffer glBindTexture");
	glEGLImageTargetTexture2DOES(TargetOES, (GLeglImageOES)pEGLImageKHR);
	DrawHelper::CheckEGLError("connectDstTextureIdToImageBuffer glEGLImageTargetTexture2DOES");
	glBindTexture(TargetOES, GL_NONE);
	DrawHelper::CheckEGLError("connectDstTextureIdToImageBuffer glBindTexture");
}

void AHardwareBufferHelper::convertImageFormat2Hardware(const int srcFormat, int &dstFormat)
{
	LOGD("convertImageFormat2Hardware srcFormat = %d", srcFormat);
	switch (srcFormat)
	{
		case MY_FORMAT_RGBA:
			dstFormat = MY_AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNKNOWN;
			break;
		case MY_FORMAT_RGB:
			dstFormat = AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM;
			break;
		case MY_FORMAT_NV21:
		case MY_FORMAT_NV12:
			dstFormat = MY_AHARDWAREBUFFER_FORMAT_YCrCb_420_SP;
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
		case MY_AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNKNOWN:
			dstFormat = MY_FORMAT_RGBA;
			break;
		case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM:
			dstFormat = MY_FORMAT_RGB;
			break;
		case MY_AHARDWAREBUFFER_FORMAT_YCrCb_420_SP:
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

void AHardwareBufferHelper::setHardwareContext()
{
	LOGD("setHardwareContext");
	pEGLDisplay = eglGetCurrentDisplay();
	pEGLContext = EGL_NO_CONTEXT;
}