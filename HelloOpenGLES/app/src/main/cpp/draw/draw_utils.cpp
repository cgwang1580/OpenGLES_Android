//
// Created by wcg3031 on 2020/4/2.
//

#include "draw_utils.h"
#include "LogAndroid.h"
#include "shader_content.h"
#include "Shader_Helper.h"
#include "MyDefineUtils.h"
#include "Utils.h"
#include "OpenImageHelper.h"

int CreateShaderHelper (LPShaderSet pShaderSet, const string vShader, const string fShader)
{
	MYLOGD("CreateShaderHelper");
	CHECK_NULL_INPUT(pShaderSet)

	pShaderSet->vertexShader = vShader;
	pShaderSet->fragmentShader = fShader;

	char *sVertexShader = (char*)pShaderSet->vertexShader.c_str();
	char *sFragShader = (char*)pShaderSet->fragmentShader.c_str();

	MYLOGD("CreateShaderHelper sVertexShader = %s\n sFragShader = %s\n", sVertexShader, sFragShader);

	if (pShaderSet->pShaderHelper)
	{
		MYLOGD("CreateShaderHelper pShaderSet->pShaderHelper is not null");
		delete(pShaderSet->pShaderHelper);
		pShaderSet->pShaderHelper = NULL;
	}

	pShaderSet->pShaderHelper = new Shader_Helper (sVertexShader, sFragShader);
	CHECK_NULL_MALLOC(pShaderSet->pShaderHelper)

	return 0;
}

int drawTriangle (Shader_Helper *pShaderHelper)
{
	MYLOGD("drawTriangle");
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
	MYLOGD("drawTexture");
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
		MYLOGE("drawTexture myImageInfo.buffer is NULL");
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
	MYLOGD("drawTexture viewPort %d %d %d %d", viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
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

int drawTexture (Shader_Helper *pShaderHelper, const int nWidth, const int nHeight, const unsigned char *pData)
{
	MYLOGD("drawTexture");
	CHECK_NULL_INPUT(pShaderHelper)
	CHECK_NULL_INPUT(pData)

	//OpenImageHelper::LoadPngFromFile ("/sdcard/testlib.png");
	return ERROR_OK;
}