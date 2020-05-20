//
// Created by chh7563 on 2019/6/4.
//

#ifndef ARC_HARDWAREBUFFERHELPER_H
#define ARC_HARDWAREBUFFERHELPER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <android/hardware_buffer.h>
#include <asvloffscreen.h>

class HardwareBufferHelper
{
public:
	HardwareBufferHelper();

	~HardwareBufferHelper();

	MRESULT Init(MInt32 nImgWidth, MInt32 nImgHeight, MInt32 nImgFormat);

	MVoid UnInit();

	MVoid DrawFrame(MInt32 nSrcTextureId, ASVLOFFSCREEN *pOutImg);

private:
	void InitGLBuffer();

	void InitDstOesTextureId();

	void InitDstOesFbo();

	void ConnectDstTextureIdToImageBuffer();

	void ReadHardwareBuffer(LPASVLOFFSCREEN pDstImg);

private:

	AHardwareBuffer     *m_PHwBuffer;
	AHardwareBuffer_Desc m_HwDesiredDesc;
	AHardwareBuffer_Desc m_HwResultDesc;
	EGLImageKHR          m_EglImage;
	EGLClientBuffer      m_EGLClientBuffer;

	PFNEGLCREATEIMAGEKHRPROC               m_eglCreateImageKHR;
	PFNEGLDESTROYIMAGEKHRPROC              m_eglDestroyImageKHR;
	PFNGLEGLIMAGETARGETTEXTURE2DOESPROC    m_glEGLImageTargetTexture2DOES;
	PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC m_eglGetNativeClientBufferANDROID;

	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragShader;

	GLuint m_Vbo;
	GLuint m_Vao;

	GLuint m_VertexCoorsHandle;
	GLuint m_TextureCoorsHandle;
	GLuint m_RGBSamplerHandle;

	GLuint m_SrcOesTextureId;
	GLuint m_DstFbo;
	GLuint m_DstOesTextureId;

	GLsync m_DynamicFence;
};


#endif //ARC_HARDWAREBUFFERHELPER_H
