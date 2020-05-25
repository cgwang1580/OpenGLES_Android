//
// Created by wcg3031 on 2020/5/22.
//

#include "SampleRender3D.h"

#include "LogAndroid.h"
#include "common.h"
#include "MyDefineUtils.h"
#include "DrawHelper.h"

SampleRender3D::SampleRender3D()
{
	LOGD("SampleRender3D::SampleRender3D");
	m_VBO_Position = GL_NONE;
	m_VBO_Normal = GL_NONE;
	m_VBO_Color = GL_NONE;
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
	generateRecBarsTest(m_SimpleMesh, bar_center, bar_radius, bar_height, bar_width, num_bars);
	return ERROR_OK;
}

RESULT SampleRender3D::convertVertex ()
{
	LOGD("SampleRender3D::convertVertex");
	// get vertex
	Vertex tVertex;
	for (int i = 0; i < m_SimpleMesh.vertices.size(); ++i)
	{
		glm::vec3 aPosition ((float)(m_SimpleMesh.vertices[i][0]), m_SimpleMesh.vertices[i][1], m_SimpleMesh.vertices[i][2]);
		tVertex.Position = aPosition;
		glm::vec3 aNormal (m_SimpleMesh.normals[i][0], m_SimpleMesh.normals[i][1], m_SimpleMesh.normals[i][2]);
		tVertex.Normal = aNormal;
		m_VertexLists.push_back(tVertex);
	}

	for (auto val : m_SimpleMesh.faces)
	{
		m_Indices.push_back(val[0]);
		m_Indices.push_back(val[1]);
		m_Indices.push_back(val[2]);
	}

	return ERROR_OK;
}

void SampleRender3D::generateRecBarsTest(
		SimpleMesh& mesh,
		Vector3D<float>& center,
		float radius,
		float height,
		float width,
		int num_bars,
		Vector3D<unsigned char> default_color)
{
	float half_height = height / 2.f, half_width = width / 2.f;
	Vector3D<float> points[4] = {
			Vector3D<float>(radius, -half_width, -half_height),
			Vector3D<float>(radius, half_width, -half_height),
			Vector3D<float>(radius, half_width, half_height),
			Vector3D<float>(radius, -half_width, half_height)
	};

	mesh.vertices.clear();
	mesh.faces.clear();
	float delta_theta = M_PI * 2.f / float(num_bars);
	for (size_t k = 0; k < num_bars; k++) {
		float theta = k * delta_theta;
		float cos = cosf(theta);
		float sin = sinf(theta);
		for (size_t i = 0; i < 4; i++) {
			Vector3D<float> p;
			p[0] = cos * points[i][0] - sin * points[i][1];
			p[1] = sin * points[i][0] + cos * points[i][1];
			p[2] = points[i][2];
			mesh.vertices.push_back(center + p);
			mesh.colors.push_back(default_color);
		}
		mesh.faces.push_back(Vector3D<int>(4 * k, 4 * k + 1, 4 * k + 2));
		mesh.faces.push_back(Vector3D<int>(4 * k + 2, 4 * k + 3, 4 * k));
	}
}