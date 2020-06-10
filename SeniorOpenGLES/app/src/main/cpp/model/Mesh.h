//
// Created by wcg3031 on 2020/6/4.
//

#pragma once

#include <string>
#include <vector>
#include "ShaderHelper.h"
#include "glm/glm.hpp"

#include "DrawHelper.h"

//using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normals;
	glm::vec2 TexCoords;

	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh
{
public:

	Mesh (const std::vector <Vertex> vertices, const std::vector<GLuint> indices, const std::vector<Texture> texture)
	{
		LOGD("Mesh::Mesh");
		Vertex tempVertex {glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f)};
		for (auto val : vertices)
		{
			tempVertex.Position = val.Position;
			tempVertex.Normals = val.Normals;
			tempVertex.TexCoords = val.TexCoords;
			m_vVertices.push_back(tempVertex);
		}

		m_vIndices = std::vector <GLuint > (indices);

		Texture tempTexture {0};
		for (auto val : texture)
		{
			tempTexture.id = val.id;
			tempTexture.type = val.type;
		}
	}
	void DrawMesh (ShaderHelper *pShaderHelper)
	{
		LOGD("Mesh::DrawMesh");
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(GL_NONE);
	}

private:
	
	std::vector <Vertex> m_vVertices;
	std::vector <GLuint> m_vIndices;
	std::vector <Texture> m_vTextures;
	GLuint m_VAO, m_VBO, m_EBO;

	void initMesh ()
	{
		LOGD("Mesh::initMesh");
		glGenBuffers(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vVertices.size(), &m_vVertices[0].Position[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_vIndices.size(), &m_vIndices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normals));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindVertexArray(GL_NONE);
	}
	void unInit ()
	{
		LOGD("Mesh::unInit");
		for (int i = 0; i < m_vTextures.size(); ++i)
			SafeDeleteGLBuffer(1, &m_vTextures[i].id);
		SafeDeleteGLBuffer(1, &m_VAO);
		SafeDeleteGLBuffer(1, &m_VBO);
		SafeDeleteGLBuffer(1, &m_EBO);
	}
};
