//
// Created by wcg3031 on 2020/6/4.
//

#include <LogAndroid.h>
#include "SampleRender3DMesh.h"

#define ModelPath "/sdcard/OpenGLESTest/nanosuit/backpack.obj"

SampleRender3DMesh::SampleRender3DMesh ()
{
	LOGD("SampleRender3DMesh::SampleRender3DMesh");
	if (!m_pModel)
		m_pModel = new Model (ModelPath);

	if (m_pShaderHelper)
		m_pShaderHelper = new ShaderHelper (pVertex_shader_0, pFragment_shader_0);
};

SampleRender3DMesh::~SampleRender3DMesh ()
{
	LOGD("SampleRender3DMesh::~SampleRender3DMesh");
	SafeDelete(m_pModel);
	SafeDelete(m_pShaderHelper);
};

RESULT SampleRender3DMesh::OnDrawFrame ()
{
	LOGD("SampleRender3DMesh::OnDrawFrame");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShaderHelper->use();
	glm::mat4 MVP = glm::mat4 (1.0f);
	m_pShaderHelper->setMat4("MVP", MVP);

	if (m_pModel)
		m_pModel->Draw(m_pShaderHelper);

	glDisable(GL_DEPTH_TEST);
	return ERROR_OK;
}

void SampleRender3DMesh::createRenderGLBuffer ()
{
	LOGD("SampleRender3DMesh::createRenderGLBuffer");
	CreateGLBuffer();
}

void SampleRender3DMesh::destroyRenderGLBuffer ()
{
	LOGD("SampleRender3DMesh::destroyRenderGLBuffer");
	DestroyGLBuffer();
}