//
// Created by wcg3031 on 2020/6/4.
//

#include "LogAndroid.h"
#include "SampleRender3DMesh.h"

#define ModelPath "/sdcard/OpenGLESTest/nanosuit/nanosuit.obj"

SampleRender3DMesh::SampleRender3DMesh ()
	:m_pModel(nullptr), m_pShaderHelper(nullptr)
{
	LOGD("SampleRender3DMesh::SampleRender3DMesh");
	if (!m_pModel)
		m_pModel = new Model (ModelPath);

	if (!m_pShaderHelper)
		m_pShaderHelper = new ShaderHelper (pVertex_shader_0, pFragment_shader_0);
	if (m_pShaderHelper)
	{
		int state = m_pShaderHelper->getShaderHelperState();
		LOGD("SampleRender3DMesh state = %d", state);
	}
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

	glm::mat4 model = glm::mat4 (1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down

	glm::vec3 Position = glm::vec3 (0.f, 0.f, 3.f);
	glm::vec3 Target = glm::vec3 (0.f, 0.f, 0.f);
	glm::vec3 Up = glm::vec3 (0.f, 1.f, 0.f);
	glm::mat4 view = glm::lookAt(Position, Target, Up);
	LOGOUT_MAT4_MATH(view, "SampleRender3DMesh::OnDrawFrame view");

	GLint viewPort[4]{0};
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)viewPort[2] / (float)viewPort[3], 0.1f, 100.0f);

	glm::mat4 MVP = projection * view * model;
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