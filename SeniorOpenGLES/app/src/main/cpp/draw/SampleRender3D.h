//
// Created by wcg3031 on 2020/5/22.
//

#pragma once

#include "vec3.hpp"
#include "sample_code.hpp"
#include "SampleBase.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoord;
};

class SampleRender3D : public SampleBase
{
public:
	SampleRender3D ();
	~SampleRender3D();

	virtual RESULT InitSample ();
	virtual void UnInitSample ();
	virtual RESULT OnDrawFrame ();

private:
	virtual RESULT createShader();
	virtual void destroyShader();
	virtual RESULT creteGLBuffer ();
	virtual void destroyGLBuffer ();
	RESULT createRectBars ();
	RESULT convertVertex ();

	SimpleMesh m_SimpleMesh;
	vector <Vertex> m_VertexLists;
	vector <unsigned int> m_Indices;

	GLuint m_VBO_Position;
	GLuint m_VBO_Normal;
	GLuint m_VBO_Color;
};




