//
// Created by wcg3031 on 2020/5/22.
//

#include "SampleRender3D.h"

#include "LogAndroid.h"
#include "common.h"
#include "MyDefineUtils.h"
#include "DrawHelper.h"

const char triangle_vertex_shader[] = "#version 300 es\n"
									  "layout (location = 0) in vec3 aPos;\n"
		                              "layout (location = 1) in vec3 aNorm\n"
		                              "layout (location = 2) in vec3 aColor\n"
		                              "out vec3 oColor\n"
									  "void main()\n"
									  "{\n"
									  "    gl_Position = vec4(aPos, 1.0);\n"
									  "    oColor = aColor;\n"
									  "}";

const char triangle_fragment_shader[] = "#version 300 es\n"
										"precision mediump float;\n"
										"in vec3 oColor;\n"
										"out vec4 FragColor;\n"
										"\n"
										"void main()\n"
										"{\n"
										"    FragColor = (oColor, 1.0);\n"
										"}";

SampleRender3D::SampleRender3D()
{
	LOGD("SampleRender3D::SampleRender3D");
	m_VAO = GL_NONE;
	m_VBO = GL_NONE;
	m_VBO_Position = GL_NONE;
	m_VBO_Normal = GL_NONE;
	m_VBO_Color = GL_NONE;
	m_pShaderHelper = nullptr;
}

SampleRender3D::~SampleRender3D()
{
	LOGD("~SampleRender3D::SampleRender3D");
}

RESULT SampleRender3D::InitSample ()
{
	LOGD("SampleRender3D::InitSample");
	int ret = ERROR_OK;
	createShader();
	CHECK_OK_RETURN(ret);
	createRectBars();
	CHECK_OK_RETURN(ret);
	creteGLBuffer();
	CHECK_OK_RETURN(ret);
	return ret;
}

void SampleRender3D::UnInitSample ()
{
	LOGD("SampleRender3D::UnInitSample");
	destroyShader();
	destroyGLBuffer ();
}

RESULT SampleRender3D::OnDrawFrame ()
{
	LOGD("SampleRender3D::onDrawFrame");
	m_pShaderHelper->use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_SimpleMesh.faces.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(GL_NONE);
	return ERROR_OK;
}

RESULT SampleRender3D::createShader()
{
	LOGD("SampleRender3D::createShader");
	m_pShaderHelper = new ShaderHelper (triangle_vertex_shader, triangle_fragment_shader);
	RESULT ret = m_pShaderHelper->getShaderHelperState();
	LOGD("createShader getShaderHelperState ret = %d", ret);
	return ret;
}

void SampleRender3D::destroyShader()
{
	LOGD("SampleRender3D::destroyShader");
	SafeDelete (m_pShaderHelper);
}

RESULT SampleRender3D::creteGLBuffer ()
{
	LOGD("SampleRender3D::creteGLBuffer");

	glGenBuffers(1, &m_VAO);
	glGenBuffers(1, &m_VBO_Position);
	glGenBuffers(1, &m_VBO_Normal);
	glGenBuffers(1, &m_VBO_Color);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_SimpleMesh.vertices), &m_SimpleMesh.vertices[0], GL_STATIC_DRAW);
	DrawHelper::CheckGLError("creteGLBuffer glBufferData");

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_SimpleMesh.normals), &m_SimpleMesh.normals[0], GL_STATIC_DRAW);
	DrawHelper::CheckGLError("creteGLBuffer glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_SimpleMesh.colors), &m_SimpleMesh.colors[0], GL_STATIC_DRAW);
	DrawHelper::CheckGLError("creteGLBuffer glBufferData");
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_SimpleMesh.faces), &m_SimpleMesh.faces[0], GL_STATIC_DRAW);
	DrawHelper::CheckGLError("creteGLBuffer glBufferData");

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	DrawHelper::CheckGLError("creteGLBuffer glVertexAttribPointer 0");

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	DrawHelper::CheckGLError("creteGLBuffer glVertexAttribPointer 1");

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Color);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	DrawHelper::CheckGLError("creteGLBuffer glVertexAttribPointer 2");

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBindVertexArray(GL_NONE);
	DrawHelper::CheckGLError("creteGLBuffer glBindVertexArray");

	return ERROR_OK;
}

void SampleRender3D::destroyGLBuffer ()
{
	LOGD("SampleRender3D::destroyGLBuffer");
	SafeDeleteGLBuffer (1, &m_VAO);
	SafeDeleteGLBuffer (1, &m_VBO_Position);
	SafeDeleteGLBuffer (1, &m_VBO_Normal);
	SafeDeleteGLBuffer (1, &m_VBO_Color);
	SafeDeleteGLBuffer (1, &m_EBO);
	m_VAO = m_VBO_Position = m_VBO_Normal = m_VBO_Color = m_EBO = GL_NONE;
}

RESULT SampleRender3D::createRectBars ()
{
	LOGD("SampleRender3D::createRectBars");
	Vector3D<float> bar_center(0.f, 0.f, 0.05f);
	float bar_radius = 0.12f, bar_height = 0.02f, bar_width = 0.01f;
	int num_bars = 16;
	generateRecBars(m_SimpleMesh, bar_center, bar_radius, bar_height, bar_width, num_bars);
	return ERROR_OK;
}

RESULT SampleRender3D::convertVertex ()
{
	LOGD("SampleRender3D::convertVertex");
	// get vertex
	Vertex tVertex;
	for (int i = 0; i < m_SimpleMesh.vertices.size(); ++i)
	{
		glm::vec3 aPosition = (m_SimpleMesh.vertices[0], m_SimpleMesh.vertices[1], m_SimpleMesh.vertices[2]);
		tVertex.Position = aPosition;
		glm::vec3 aNormal = (m_SimpleMesh.normals[0], m_SimpleMesh.normals[1], m_SimpleMesh.normals[2]);
		tVertex.Normal = aNormal;
		m_VertexLists.push_back(tVertex);
	}

	for (auto val : m_SimpleMesh.faces)
	{
		glm::vec3 aIndices = (val[0], val[1], val[2]);
		m_Indices.push_back(aIndices);
	}

	return ERROR_OK;
}