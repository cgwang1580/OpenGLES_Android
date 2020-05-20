//
// Created by chh7563 on 2019/6/4.
//

//link EGL android GLESv3

#include <cstring>
#include <merror.h>
#include "HardwareBufferHelper.h"
#include "LogUtils.h"
#include "GLUtils.h"

#define CHECK_GL_ERROR(...)   LOGCATE("CHECK_GL_ERROR %s glGetError = %ld, line = %d, ",  __FUNCTION__, glGetError(), __LINE__)


///* same as HAL_PIXEL_FORMAT_BGRA_8888 */
//      AHARDWAREBUFFER_FORMAT_B8G8R8A8_UNORM           = 5,
///* same as HAL_PIXEL_FORMAT_YV12 */
//		AHARDWAREBUFFER_FORMAT_YV12                     = 0x32315659,
///* same as HAL_PIXEL_FORMAT_Y8 */
//		AHARDWAREBUFFER_FORMAT_Y8                       = 0x20203859,
///* same as HAL_PIXEL_FORMAT_Y16 */
//		AHARDWAREBUFFER_FORMAT_Y16                      = 0x20363159,
///* same as HAL_PIXEL_FORMAT_RAW16 */
//		AHARDWAREBUFFER_FORMAT_RAW16                    = 0x20,
///* same as HAL_PIXEL_FORMAT_RAW10 */
//		AHARDWAREBUFFER_FORMAT_RAW10                    = 0x25,
///* same as HAL_PIXEL_FORMAT_RAW12 */
//		AHARDWAREBUFFER_FORMAT_RAW12                    = 0x26,
///* same as HAL_PIXEL_FORMAT_RAW_OPAQUE */
//		AHARDWAREBUFFER_FORMAT_RAW_OPAQUE               = 0x24,
///* same as HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED */
//		AHARDWAREBUFFER_FORMAT_IMPLEMENTATION_DEFINED   = 0x22,
///* same as HAL_PIXEL_FORMAT_YCBCR_420_888 */
//		AHARDWAREBUFFER_FORMAT_Y8Cb8Cr8_420             = 0x23,
///* same as HAL_PIXEL_FORMAT_YCBCR_422_SP */
//		AHARDWAREBUFFER_FORMAT_YCbCr_422_SP             = 0x10,
///* same as HAL_PIXEL_FORMAT_YCRCB_420_SP */
//		AHARDWAREBUFFER_FORMAT_YCrCb_420_SP             = 0x11,
///* same as HAL_PIXEL_FORMAT_YCBCR_422_I */
//		AHARDWAREBUFFER_FORMAT_YCbCr_422_I              = 0x14

int GetHardwareBufferFormat(MInt32 nFormat)
{
	int hardwareFormat = 0x00;
	switch (nFormat)
	{
		case ASVL_PAF_NV21:
			hardwareFormat = 0x11; // AHARDWAREBUFFER_FORMAT_YCrCb_420_SP
			break;
		case ASVL_PAF_YV12:
			hardwareFormat = 0x32315659; // AHARDWAREBUFFER_FORMAT_B8G8R8A8_UNORM
			break;
		default:
			// do not support
			break;
	}

	return hardwareFormat;
}

const static GLfloat OES_FBO_COORS[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
};

// sync gpu and cpu max time when we need gpu write then cpu read.
#define GPU_CPU_BUFFER_MAX_SYNC_TIME (500 * 1000 * 1000)

//Single vertex shader
static const char VERTEX_SHADER[] =
		"#version 300 es\n"
		"in vec4 aPosition;\n"
		"in vec2 aTextureCoors;\n"
		"out vec2 vTextureCoors;\n"
		"\n"
		"void main()\n"
		"{\n"
		"\tvTextureCoors = aTextureCoors;\n"
		"\tgl_Position = aPosition;\n"
		"}";

// Pixel shader, RGB to YUV conversion.
static const char FRAGMENT_SHADER[] =
		"#version 300 es\n"
		"#extension GL_EXT_YUV_target: require\n"
		"\n"
		"precision mediump float;\n"
		"in vec2 vTextureCoors;\n"
		"layout(yuv) out vec4 outColor;\n"
		"uniform sampler2D s_texture;\n"
		"void main()\n"
		"{\n"
		"    yuvCscStandardEXT conv_standard = itu_601_full_range;\n"
		"\tvec4 rgbaColor = texture(s_texture, vTextureCoors);\n"
		"\tvec3 rgbColor = vec3(rgbaColor.r, rgbaColor.g, rgbaColor.b);\n"
		"\t\n"
		"\tvec3 yuv = rgb_2_yuv(rgbColor, conv_standard);\n"
		"\toutColor = vec4(yuv, 1.0);\n"
		"     \n"
		"}";

HardwareBufferHelper::HardwareBufferHelper()
{
	m_PHwBuffer       = NULL;
	m_EglImage        = EGL_NO_IMAGE_KHR;
	m_EGLClientBuffer = NULL;
	m_Program  = 0;

	m_eglCreateImageKHR               = NULL;
	m_eglDestroyImageKHR              = NULL;
	m_glEGLImageTargetTexture2DOES    = NULL;
	m_eglGetNativeClientBufferANDROID = NULL;

	memset(&m_HwDesiredDesc, 0, sizeof(m_HwDesiredDesc));
	memset(&m_HwResultDesc, 0, sizeof(m_HwResultDesc));

	m_Vbo                  = 0;
	m_Vao                  = 0;
	m_VertexCoorsHandle       = 0;
	m_TextureCoorsHandle = 0;
	m_RGBSamplerHandle   = 0;

	m_SrcOesTextureId = 0;
	m_DstFbo          = 0;
	m_DstOesTextureId = 0;

	m_DynamicFence = NULL;
}

HardwareBufferHelper::~HardwareBufferHelper()
{

}

MRESULT HardwareBufferHelper::Init(MInt32 nImgWidth, MInt32 nImgHeight, MInt32 nImgFormat)
{
	LOGCATE("HardwareBufferHelper::Init [imgW, imgH] = [%d, %d], nImgFormat = %x", nImgWidth, nImgHeight, nImgFormat);
	MRESULT nRet = MOK;
	do
	{
		// gpu write and cpu read.
		m_HwDesiredDesc.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT;

		m_HwDesiredDesc.rfu0   = 0;
		m_HwDesiredDesc.rfu1   = 0;
		m_HwDesiredDesc.format = GetHardwareBufferFormat(nImgFormat);
		m_HwDesiredDesc.width  = nImgWidth;
		m_HwDesiredDesc.height = nImgHeight;
		m_HwDesiredDesc.stride = nImgWidth; // Row stride in pixels, ignored for AHardwareBuffer_allocate()
		m_HwDesiredDesc.layers = 1;

		nRet = AHardwareBuffer_allocate(&m_HwDesiredDesc, &m_PHwBuffer);
		if (nRet)
		{
			LOGCATE("HardwareBufferHelper::Init AllocateBuffer = %p , nRet = %d", m_PHwBuffer, nRet);
			nRet = MERR_NO_MEMORY;
			break;
		}
		LOGCATE("HardwareBufferHelper::Init buffer pitch = %d", m_HwDesiredDesc.stride);

		// Get egl functions.
		{
			m_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR");
			m_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR");
			m_glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) eglGetProcAddress(
					"glEGLImageTargetTexture2DOES");
			m_eglGetNativeClientBufferANDROID = (PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC) eglGetProcAddress(
					"eglGetNativeClientBufferANDROID");
			if (!m_eglCreateImageKHR || !m_eglDestroyImageKHR || !m_glEGLImageTargetTexture2DOES ||
				!m_eglGetNativeClientBufferANDROID)
			{
				LOGCATE("HardwareBufferHelper::Init Can't find EGLImage related functions.");
				nRet = MERR_UNSUPPORTED;
				break;
			}

			EGLint eglImageAttubs[] = {
					EGL_WIDTH,  int(m_HwResultDesc.width),
					EGL_HEIGHT, int(m_HwResultDesc.height),
					EGL_IMAGE_PRESERVED_KHR,
					EGL_TRUE,
					EGL_NONE
			};

			if (MNull != m_PHwBuffer)
			{
				AHardwareBuffer_describe(m_PHwBuffer, &m_HwResultDesc);
				LOGCATE("HardwareBufferHelper::Init m_HwResultDesc.width=%d, m_HwResultDesc.height=%d, m_HwResultDesc.stride = %d",
						m_HwResultDesc.width, m_HwResultDesc.height, m_HwResultDesc.stride);
			}
			else
			{
				LOGCATE("HardwareBufferHelper::Init AHardwareBuffer_allocate error.");
				nRet = MERR_INVALID_PARAM;
				break;
			}

			m_EGLClientBuffer = m_eglGetNativeClientBufferANDROID(m_PHwBuffer);
			if (MNull == m_EGLClientBuffer)
			{
				LOGCATE("HardwareBufferHelper::Init eglGetNativeClientBufferANDROID error.");
				nRet = MERR_UNKNOWN;
				break;
			}

			m_EglImage = m_eglCreateImageKHR(eglGetCurrentDisplay(), EGL_NO_CONTEXT,
										   EGL_NATIVE_BUFFER_ANDROID, m_EGLClientBuffer,
										   eglImageAttubs);
			if (EGL_NO_IMAGE_KHR == m_EglImage)
			{
				LOGCATE("HardwareBufferHelper::Init eglCreateImageKHR error.");
				nRet = MERR_UNKNOWN;
				break;
			}
		}


		m_Program = GLUtils::CreateProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_VertexShader,
										   m_FragShader);
		if (!m_Program)
		{
			CHECK_GL_ERROR();
			LOGCATE("GLByteFlowRender::CreateProgram Could not create program.");
			break;
		}

		m_RGBSamplerHandle = static_cast<GLuint>(glGetUniformLocation(m_Program, "s_texture"));
		m_VertexCoorsHandle = (GLuint) glGetAttribLocation(m_Program, "aPosition");
		m_TextureCoorsHandle = (GLuint) glGetAttribLocation(m_Program, "aTextureCoors");

		InitGLBuffer();

		InitDstOesTextureId();

		ConnectDstTextureIdToImageBuffer();

		InitDstOesFbo();
	}
	while (false);

	if (nRet != MOK)
	{
		UnInit();
	}

	return nRet;
}

MVoid HardwareBufferHelper::UnInit()
{
	if (m_DstFbo)
	{
		glDeleteFramebuffers(1, &m_DstFbo);
		m_DstFbo = 0;
	}

	if (m_DstOesTextureId)
	{
		glDeleteTextures(1, &m_DstOesTextureId);
		m_DstOesTextureId = 0;
	}

	if (m_Vbo)
	{
		glDeleteBuffers(1, &m_Vbo);
		m_Vbo = 0;
	}

	if (m_Vao)
	{
		glDeleteVertexArrays(1, &m_Vao);
		m_Vao = 0;
	}

	if (EGL_NO_IMAGE_KHR != m_EglImage) {
		m_eglDestroyImageKHR(eglGetCurrentDisplay(), m_EglImage);
		m_EglImage = MNull;
	}

	// EGLClientBuffer must be destroyed no earlier than when all of its associated EGLImages are destroyed by eglDestroyImageKHR.
	if (MNull != m_EGLClientBuffer) {
		m_EGLClientBuffer = MNull;
	}

	if (MNull != m_PHwBuffer) {
		AHardwareBuffer_release(m_PHwBuffer);
		m_PHwBuffer = MNull;
	}


	if (MNull != m_DynamicFence)
	{
		glDeleteSync(m_DynamicFence);
		m_DynamicFence = MNull;
	}

}

void HardwareBufferHelper::InitGLBuffer()
{
	glGenBuffers(1, &m_Vbo);
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &m_Vao);
	CHECK_GL_ERROR();

	glBindVertexArray(m_Vao);
	CHECK_GL_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	CHECK_GL_ERROR();

	glBufferData(GL_ARRAY_BUFFER, sizeof(OES_FBO_COORS), OES_FBO_COORS, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glVertexAttribPointer(m_VertexCoorsHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(m_VertexCoorsHandle);
	CHECK_GL_ERROR();

	glVertexAttribPointer(m_TextureCoorsHandle, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
						  (const void *) (2 * sizeof(GLfloat)));
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(m_TextureCoorsHandle);
	CHECK_GL_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void HardwareBufferHelper::InitDstOesTextureId()
{
	glGenTextures(1, &m_DstOesTextureId);
	CHECK_GL_ERROR();

	GLuint target = GL_TEXTURE_EXTERNAL_OES;

	glBindTexture(target, m_DstOesTextureId);
	CHECK_GL_ERROR();
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECK_GL_ERROR();
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CHECK_GL_ERROR();
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR();
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR();

	glBindTexture(target, 0);
	CHECK_GL_ERROR();
}

void HardwareBufferHelper::InitDstOesFbo()
{
	GLuint target = GL_TEXTURE_EXTERNAL_OES;
	glBindTexture(target, m_DstOesTextureId);
	CHECK_GL_ERROR();

	glGenFramebuffers(1, &m_DstFbo);
	CHECK_GL_ERROR();
	GLint oriFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oriFbo);
	CHECK_GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, m_DstFbo);
	CHECK_GL_ERROR();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, m_DstOesTextureId, 0);
	CHECK_GL_ERROR();

	GLenum status = 0;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	LOGCATE("HardwareBufferHelper::InitDstOesFbo textureBindToFrameBuffer init status = %d", status);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		LOGCATE("HardwareBufferHelper::InitDstOesFbo textureBindToFrameBuffer init success");
	}
	CHECK_GL_ERROR();

	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);  // Black in YUV.

	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR();

	glBindTexture(target, 0);
	CHECK_GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, oriFbo);
	CHECK_GL_ERROR();
}

void HardwareBufferHelper::ConnectDstTextureIdToImageBuffer()
{
	GLuint target = GL_TEXTURE_EXTERNAL_OES;
	glBindTexture(target, m_DstOesTextureId);
	CHECK_GL_ERROR();
	m_glEGLImageTargetTexture2DOES(target, m_EglImage); // this function likes glTexImage2D
	CHECK_GL_ERROR();
	glBindTexture(target, 0);
	CHECK_GL_ERROR();
}


MVoid HardwareBufferHelper::DrawFrame(MInt32 nSrcTextureId, ASVLOFFSCREEN *pOutImg)
{
	LOGCATE("HardwareBufferHelper::DrawFrame srcTextureID = %d", nSrcTextureId);
	m_SrcOesTextureId = nSrcTextureId;

	if (!m_Program)
	{
		LOGCATE("HardwareBufferHelper::DrawFrame m_Program == NULL");
		return;
	}

	GLint oriFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oriFbo);
	CHECK_GL_ERROR();
	GLint viewPorts[4] = {0};
	glGetIntegerv(GL_VIEWPORT, viewPorts);
	CHECK_GL_ERROR();

	glUseProgram(m_Program);

	CHECK_GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, m_DstFbo);
	CHECK_GL_ERROR();
	glViewport(0, 0, m_HwDesiredDesc.stride, m_HwDesiredDesc.height);
	CHECK_GL_ERROR();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR();

	glActiveTexture(GL_TEXTURE1);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, m_SrcOesTextureId);
	CHECK_GL_ERROR();
	glUniform1i(m_RGBSamplerHandle, 1);
	CHECK_GL_ERROR();
	glBindVertexArray(m_Vao);
	CHECK_GL_ERROR();

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	CHECK_GL_ERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	CHECK_GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, oriFbo);
	CHECK_GL_ERROR();
	glViewport(viewPorts[0], viewPorts[1], viewPorts[2], viewPorts[3]);
	CHECK_GL_ERROR();
	glUseProgram(0);
	CHECK_GL_ERROR();

	m_DynamicFence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	CHECK_GL_ERROR();

	ReadHardwareBuffer(pOutImg);
}

void HardwareBufferHelper::ReadHardwareBuffer(LPASVLOFFSCREEN pDstImg)
{
	LOGCATE("HardwareBufferHelper::ReadHardwareBuffer pOutImg->ppu8Plane[0] = %p", pDstImg->ppu8Plane[0]);
	if (pDstImg->ppu8Plane[0] == NULL)
		return;

	GLenum syncStatus = GL_CONDITION_SATISFIED;
	// Here, we use glClientWaitSync(OpenGL ES3.0) instead of glFinish() to improve the sync performance between GPU and CPU.
	syncStatus = glClientWaitSync(m_DynamicFence, GL_SYNC_FLUSH_COMMANDS_BIT, GPU_CPU_BUFFER_MAX_SYNC_TIME);
	CHECK_GL_ERROR();
	glDeleteSync(m_DynamicFence);
	CHECK_GL_ERROR();
	m_DynamicFence = MNull;
	LOGCATE("HardwareBufferHelper::ReadHardwareBuffer syncStatus = %x",syncStatus);

	unsigned char *ptrReader = MNull;
	// cpu reading now.
	AHardwareBuffer_lock(m_PHwBuffer, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN, -1, MNull,
						 (void **) &ptrReader);

	// Here, please make sure offscreen's pitch is the same as hardware buffer's stride
	if (m_HwDesiredDesc.stride == m_HwResultDesc.stride)
	{
		memcpy(pDstImg->ppu8Plane[0], ptrReader, m_HwResultDesc.stride * m_HwResultDesc.height * 3 / 2);
	}
	else
	{
		for (int i = 0; i < m_HwResultDesc.height * 1.5; ++i)
		{
			memcpy(pDstImg->ppu8Plane[0] + i * pDstImg->pi32Pitch[0],
				   ptrReader + i * m_HwResultDesc.stride, pDstImg->i32Width);
		}
	}

//	FUN_BEGIN_TIME("DumpOffscreen")
//		DumpOffscreen(pDstImg, "/sdcard/DCIM", "res");
//	FUN_END_TIME("DumpOffscreen")

	int32_t fence = -1;
	AHardwareBuffer_unlock(m_PHwBuffer, &fence);
}

