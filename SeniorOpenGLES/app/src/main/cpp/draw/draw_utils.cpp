//
// Created by wcg3031 on 2020/4/2.
//

#include <HardwareBufferHelper.h>
#include "draw_utils.h"
#include "LogAndroid.h"
#include "shader_content.h"
#include "Shader_Helper.h"
#include "MyDefineUtils.h"
#include "Utils.h"
#include "OpenImageHelper.h"
#include "DrawHelper.h"

int CreateShaderHelper (LPShaderSet pShaderSet, const string vShader, const string fShader)
{
	LOGD("CreateShaderHelper");
	CHECK_NULL_INPUT(pShaderSet)

	pShaderSet->vertexShader = vShader;
	pShaderSet->fragmentShader = fShader;

	char *sVertexShader = (char*)pShaderSet->vertexShader.c_str();
	char *sFragShader = (char*)pShaderSet->fragmentShader.c_str();

	LOGD("CreateShaderHelper sVertexShader = %s\n sFragShader = %s\n", sVertexShader, sFragShader);

	if (pShaderSet->pShaderHelper)
	{
		LOGD("CreateShaderHelper pShaderSet->pShaderHelper is not null");
		delete(pShaderSet->pShaderHelper);
		pShaderSet->pShaderHelper = nullptr;
	}

	pShaderSet->pShaderHelper = new Shader_Helper (sVertexShader, sFragShader);
	CHECK_NULL_MALLOC(pShaderSet->pShaderHelper)

	return 0;
}

int drawTriangle (Shader_Helper *pShaderHelper)
{
	LOGD("drawTriangle");
	CHECK_NULL_INPUT(pShaderHelper)

	float vertex[]{
		0.5f, 0, 0,
		0, 0.5f, 0,
		-0.5f, 0, 0
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	float vertex_multi[]{
		0.75f, -0.75f, 0,
		0.75f, -0.25f, 0,
		-0.5f, -0.75f, 0,
		-0.5f, -0.25f, 0,
	};
	int index_multi[]{
		0, 1, 2,
		1, 2, 3,
	};
	GLuint VAO2, VBO2, EBO;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_multi), vertex_multi, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_multi), index_multi, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	pShaderHelper->use();
	pShaderHelper->setfloat("g_color", 0.2f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	pShaderHelper->setfloat("g_color", 1.0f);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, sizeof(index_multi)/ sizeof(int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO2);

	return 0;
}

int drawTexture (Shader_Helper *pShaderHelper, const LPMyImageInfo lpMyImageInfo)
{
	LOGD("drawTexture");
	CHECK_NULL_INPUT(pShaderHelper)
	CHECK_NULL_INPUT(lpMyImageInfo)

	float vertex_texture [] {
		1.0f, -0.5, 0,		1.0f, 0, 0,      	1.0f, 1.0f,
		1.0f, 0.5f, 0,		0, 1.0f, 0,   		1.0f, 0,
		-1.0f, 0.5f, 0,		0, 0, 1.0f,  		0, 0,
		-1.0, -0.5, 0,		1.0f, 1.0f, 0,     	0, 1.0f
	};

	int vertex_index [] {
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture), vertex_texture, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_index), vertex_index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (NULL != lpMyImageInfo->buffer[0]) {
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, lpMyImageInfo->width, lpMyImageInfo->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, lpMyImageInfo->buffer[0]);
		glGenerateMipmap (GL_TEXTURE_2D);
	} else {
		LOGE("drawTexture myImageInfo.buffer is NULL");
	}
	OpenImageHelper::FreeMyImageInfo(lpMyImageInfo);

	pShaderHelper->use();
	pShaderHelper->setInt("texture1", 0);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(vertex_index)/ sizeof(int), GL_UNSIGNED_INT, 0);

	GLint viewPort[4] {0};
	glGetIntegerv(GL_VIEWPORT, viewPort);
	LOGD("drawTexture viewPort %d %d %d %d", viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
	int width = viewPort[2];
	/*int height1 = 0.25 * viewPort[3];
	int height2 = 0.75 * viewPort[3];*/
	int height = viewPort[3];
	MyImageInfo myImageInfo {0};
	myImageInfo.width = width;
	myImageInfo.height = height;
	myImageInfo.format = MY_FORMAT_RGBA;
	myImageInfo.channel[0] = myImageInfo.width;
	OpenImageHelper::AllocMyImageInfo(&myImageInfo);
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
	START_TIME ("glReadPixels")
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, myImageInfo.buffer[0]);
	STOP_TIME ("glReadPixels")
	OpenImageHelper::ExchangeImageCoordinateY(&myImageInfo);
	OpenImageHelper::SaveImageToPng(&myImageInfo, "/sdcard/OpenGLESTest/texture.png");
	OpenImageHelper::FreeMyImageInfo(&myImageInfo);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteTextures(1, &texture1);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	return ERROR_OK;
}

int drawFBO (Shader_Helper *pShaderHelperFBO, Shader_Helper *pShaderHelperNormal, const LPMyImageInfo lpMyImageInfo)
{
	LOGD ("drawFBO");

	GLint viewPort[4] {0};
	glGetIntegerv(GL_VIEWPORT, viewPort);
	LOGD("drawFBO viewPort %d %d %d %d", viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
	int nScreenWidth = viewPort[2];
	int nScreenHeight = viewPort[3];

	GLfloat vVertices[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
	};

	GLfloat vTexCoors[] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
	};

	GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

	GLuint VBO[3];
	glGenBuffers(3, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vTexCoors), vTexCoors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	/// !!! need call glBindBuffer to bind GL_ELEMENT_ARRAY_BUFFER again
	/// before glBindVertexArray GL_NONE !!!///
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
	glBindVertexArray(GL_NONE);

	/*GLfloat vVertices[] = {
			-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,
			1.0f, -1.0f, 0.0f,		1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f,		0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,		1.0f, 0.0f,
	};
	GLuint indices[] = { 0, 1, 2, 1, 3, 2 };
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);*/

	int nImageWidth = lpMyImageInfo->width;
	int nImageHeight = lpMyImageInfo->height;

	// create a color texture as src
	GLuint textureFboId, textureColorId;
	const GLenum targetRgb = GL_TEXTURE_2D;
	DrawHelper::GetOneTexture(targetRgb, &textureColorId);
	glBindTexture(targetRgb, textureColorId);
	if (NULL != lpMyImageInfo->buffer[0]) {
		glTexImage2D (targetRgb, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lpMyImageInfo->buffer[0]);
		glGenerateMipmap (targetRgb);
	} else {
		LOGE("drawTexture myImageInfo.buffer is NULL");
	}
	//OpenImageHelper::FreeMyImageInfo(lpMyImageInfo);

	// create a texture as FBO color attachment
	DrawHelper::GetOneTexture(targetRgb, &textureFboId);
	glBindTexture(targetRgb, textureFboId);
	glTexImage2D (targetRgb, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(targetRgb, GL_NONE);

	// create frame buffer object
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// bind color texture
	glBindTexture(GL_TEXTURE_2D, textureFboId);
	// attach a texture to frame buffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFboId, 0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	// check frame buffer state
	GLenum tmpStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != tmpStatus)
	{
		LOGE("glCheckFramebufferStatus tmpStatus = %d", tmpStatus);
		return ERROR_GL_STATUS;
	}

	// draw offscreen
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glViewport(0, 0, nImageWidth, nImageHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	pShaderHelperFBO->use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorId);
	pShaderHelperFBO->setInt("screenTexture", 0);
	DrawHelper::CheckGLError("drawFBO");
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)0);
	DrawHelper::CheckGLError("drawFBO glDrawElements");
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindVertexArray(GL_NONE);

	// save texture for test
	SRECT sRect{0, 0, nImageWidth, nImageHeight};
	DrawHelper::SaveRenderImage (sRect, GL_RGBA, "/sdcard/OpenGLESTest/SaveRender.png");

	// normal render
	glViewport(0, 0, nScreenWidth, nScreenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	pShaderHelperNormal->use();
	pShaderHelperNormal->setInt("texture1", 1);
	DrawHelper::CheckGLError("drawFBO normal render setInt");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFboId);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)0);
	DrawHelper::CheckGLError("drawFBO normal render glDrawElements");
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindVertexArray(GL_NONE);
	glActiveTexture(GL_TEXTURE0);

	glDeleteTextures(1, &textureColorId);
	glDeleteTextures(1, &textureFboId);
	glDeleteFramebuffers(1, &FBO);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteBuffers(1, &VBO[2]);
	/*glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
	glDeleteVertexArrays(1, &VAO);
	return ERROR_OK;
}

int drawByHardwareBuffer (Shader_Helper *pShaderHelperHardwareNormal, const AHardwareBufferHelper *pHardwareBufferHelper, LPMyImageInfo const lpMyImageInfo)
{
	LOGD ("drawByHardwareBuffer");

	int ret = ERROR_OK;
	//CHECK_NULL_INPUT(pShaderHelperHardwareNormal)
	CHECK_NULL_INPUT(pHardwareBufferHelper)
	CHECK_NULL_INPUT(lpMyImageInfo)
	CHECK_NULL_INPUT(lpMyImageInfo->buffer[0])

	GLint viewPort[4] {0};
	glGetIntegerv(GL_VIEWPORT, viewPort);
	LOGD("drawByHardwareBuffer viewPort %d %d %d %d", viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
	int nScreenWidth = viewPort[2];
	int nScreenHeight = viewPort[3];

	int nImageWidth = lpMyImageInfo->width;
	int nImageHeight = lpMyImageInfo->height;
	int nImageFormat = lpMyImageInfo->format;

	GLuint textureColorId = GL_NONE;
	MyImageInfo myImageInfo{0};
	do
	{
		const GLenum TargetColor = GL_TEXTURE_2D;
		const GLenum targetOES = GL_TEXTURE_EXTERNAL_OES;

		// create a color texture as src
		DrawHelper::GetOneTexture(TargetColor, &textureColorId);
		DrawHelper::CheckGLError("GetOneTexture");
		glActiveTexture(GL_TEXTURE0);
		DrawHelper::CheckGLError("glActiveTexture");
		glBindTexture(TargetColor, textureColorId);
		DrawHelper::CheckGLError("glBindTexture");
		if (NULL != lpMyImageInfo->buffer[0]) {
			glTexImage2D (TargetColor, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lpMyImageInfo->buffer[0]);
			DrawHelper::CheckGLError("glTexImage2D");
			glGenerateMipmap (TargetColor);
			DrawHelper::CheckGLError("glGenerateMipmap");
		} else {
			LOGE("drawByHardwareBuffer myImageInfo.buffer is NULL");
		}
		glBindTexture(TargetColor, GL_NONE);
		DrawHelper::CheckGLError("glBindTexture");

		auto *pBufferHelper = (AHardwareBufferHelper *)pHardwareBufferHelper;
		if (!pBufferHelper->getCreateState())
		{
			nImageFormat = MY_FORMAT_NV21;
			ret = pBufferHelper->createGPUBuffer(nImageWidth, nImageHeight, nImageFormat);
			LOGE("drawByHardwareBuffer createGPUBuffer ret = %d", ret);
			if (ERROR_OK != ret)
				break;
		}
		ret = pBufferHelper->onDrawFrame(textureColorId, &myImageInfo);
		if (MY_FORMAT_NV21 == myImageInfo.format || MY_FORMAT_NV12 == myImageInfo.format)
		{
			char sPath[MAX_PATH]{0};
			sprintf(sPath, "/sdcard/OpenGLESTest/gpu_%d_%dX%d.NV21", pBufferHelper->getRenderNum(), myImageInfo.channel[0], myImageInfo.height);
			OpenImageHelper::SaveImageToYuv(&myImageInfo, sPath);
		}
		if (ERROR_OK != ret)
			break;
	}while (false);

	OpenImageHelper::FreeMyImageInfo(&myImageInfo);
	glDeleteTextures(1, &textureColorId);

	return ret;
}

int drawByHardwareBuffer2 (const LPMyImageInfo lpMyImageInfo)
{
	LOGD ("drawByHardwareBuffer");

	int ret = ERROR_OK;
	CHECK_NULL_INPUT(lpMyImageInfo)
	CHECK_NULL_INPUT(lpMyImageInfo->buffer[0])

	GLint viewPort[4] {0};
	glGetIntegerv(GL_VIEWPORT, viewPort);
	LOGD("drawByHardwareBuffer viewPort %d %d %d %d", viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
	int nScreenWidth = viewPort[2];
	int nScreenHeight = viewPort[3];

	int nImageWidth = lpMyImageInfo->width;
	int nImageHeight = lpMyImageInfo->height;
	int nImageFormat = lpMyImageInfo->format;

	GLuint textureColorId = GL_NONE;
	do
	{
		const GLenum TargetColor = GL_TEXTURE_2D;

		// create a color texture as src
		DrawHelper::GetOneTexture(TargetColor, &textureColorId);
		DrawHelper::CheckGLError("GetOneTexture");
		glActiveTexture(GL_TEXTURE0);
		DrawHelper::CheckGLError("glActiveTexture");
		glBindTexture(TargetColor, textureColorId);
		DrawHelper::CheckGLError("glBindTexture");
		if (NULL != lpMyImageInfo->buffer[0]) {
			glTexImage2D (TargetColor, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lpMyImageInfo->buffer[0]);
			DrawHelper::CheckGLError("glTexImage2D");
			glGenerateMipmap (TargetColor);
			DrawHelper::CheckGLError("glGenerateMipmap");
		} else {
			LOGE("drawByHardwareBuffer myImageInfo.buffer is NULL");
		}
		glBindTexture(TargetColor, GL_NONE);
		DrawHelper::CheckGLError("glBindTexture");

		int imgW = lpMyImageInfo->width;
		int imgH = lpMyImageInfo->height;
		ASVLOFFSCREEN outputImg = {0};
		outputImg.i32Width = imgW;
		outputImg.i32Height = imgH;
		outputImg.pi32Pitch[0] = imgW;
		outputImg.pi32Pitch[1] = imgW;
		outputImg.u32PixelArrayFormat = ASVL_PAF_NV21;
		long lSize = (long)(imgW * imgH * 1.5);
		outputImg.ppu8Plane[0] = static_cast<MUInt8 *>(malloc(lSize));
		outputImg.ppu8Plane[1] = outputImg.ppu8Plane[0] + outputImg.pi32Pitch[0] * outputImg.i32Height;
		HardwareBufferHelper *hardwareBufferHelper = new HardwareBufferHelper();
		hardwareBufferHelper->Init(imgW, imgH, ASVL_PAF_NV21);
		hardwareBufferHelper->DrawFrame(textureColorId, &outputImg);

		char sPath[MAX_PATH]{0};
		sprintf(sPath, "/sdcard/OpenGLESTest/gpu_1_%dX%d.NV21", outputImg.pi32Pitch[0], outputImg.i32Height);
		FILE *fp = fopen(sPath, "wb");
		fwrite(outputImg.ppu8Plane[0], 1, lSize, fp);
		fclose(fp);

		/*if (MY_FORMAT_NV21 == myImageInfo.format || MY_FORMAT_NV12 == myImageInfo.format)
		{
			char sPath[MAX_PATH]{0};
			sprintf(sPath, "/sdcard/OpenGLESTest/gpu_0_%dx%d.nv12", myImageInfo.channel[0], myImageInfo.height);
			OpenImageHelper::SaveImageToYuv(&myImageInfo, sPath);
		}*/
		free(outputImg.ppu8Plane[0]);
		hardwareBufferHelper->UnInit();
		delete hardwareBufferHelper;
		hardwareBufferHelper = NULL;

		if (ERROR_OK != ret)
			break;

	}while (false);



	glDeleteTextures(1, &textureColorId);

	return ret;
}