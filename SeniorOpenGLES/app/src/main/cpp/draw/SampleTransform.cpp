//
// Created by wcg3031 on 2020/5/21.
//

#include <LogAndroid.h>
#include <common.h>
#include <MyDefineUtils.h>
#include "SampleTransform.h"

#define CHECK_OK(_ret_)		if (ERROR_OK != _ret_) {return _ret_;}

const GLfloat vVertex[24] {
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
};

/*static const GLfloat vTexture[] {
	1.0f, 1.0f, 1.0f
};*/

const GLuint vIndex[36] {
		0, 1, 2,
		0, 2, 3,
		0, 3, 7,
		0, 4, 7,
		0, 1, 4,
		1, 4, 5,
		1, 2, 6,
		1, 5, 6,
		4, 5, 6,
		4, 6, 7,
		2, 3, 7,
		2, 6, 7
};

SampleTransform::SampleTransform()
{
	LOGD("SampleTransform::SampleTransform");
}

SampleTransform::~SampleTransform()
{
	LOGD("~SampleTransform::SampleTransform");
}

RESULT SampleTransform::createShader()
{
	LOGD("SampleTransform::createShader");
	m_pShaderHelper = new ShaderHelper (triangle_vertex_shader, triangle_fragment_shader);
	RESULT ret = m_pShaderHelper->getShaderHelperState();
	LOGD("createShader getShaderHelperState ret = %d", ret);
	return ret;
}

RESULT SampleTransform::creteGLBuffer ()
{
	LOGD("SampleTransform::creteGLBuffer");

	glGenBuffers(2, m_sVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_sVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertex), vVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndex), vIndex, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_sVBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(GL_NONE);

	return ERROR_OK;
}

RESULT SampleTransform::InitSample ()
{
	LOGD("SampleTransform::InitSample");
	int ret = ERROR_OK;
	createShader();
	CHECK_OK(ret);
	creteGLBuffer();
	CHECK_OK(ret);
	return ret;
}

void SampleTransform::UnInitSample ()
{
	LOGD("SampleTransform::UnInitSample");
	if (m_VAO) glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(2, m_sVBO);
	SafeDelete (m_pShaderHelper);
}

RESULT SampleTransform::OnDrawFrame ()
{
	LOGD("SampleTransform::onDrawFrame");
	m_pShaderHelper->use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(GL_NONE);
	return ERROR_OK;
}