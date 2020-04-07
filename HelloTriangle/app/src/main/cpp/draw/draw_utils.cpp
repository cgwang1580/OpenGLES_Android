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
	CHECK_NULL_MALLOC(pShaderSet->pShaderHelper);

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

int drawTexture (Shader_Helper *pShaderHelper, const int nWidth, const int nHeight, const unsigned char *pData)
{
	MYLOGD("drawTexture");
	CHECK_NULL_INPUT(pShaderHelper)
	CHECK_NULL_INPUT(pData)

	OpenImageHelper::LoadPngFromFile ("/sdcard/testlib.png");
}