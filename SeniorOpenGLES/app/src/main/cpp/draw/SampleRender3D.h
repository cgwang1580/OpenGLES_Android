//
// Created by wcg3031 on 2020/5/22.
//

#pragma once

#include "vec3.hpp"
#include "barHelper.h"
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
	~SampleRender3D ();

	virtual RESULT InitSample ();
	virtual void UnInitSample ();
	virtual RESULT OnDrawFrame ();

private:
	RESULT createShader();
	void destroyShader();
	RESULT creteGLBuffer ();
	void destroyGLBuffer ();
	RESULT createRectBars ();
	RESULT convertVertex ();

	SimpleMesh m_SimpleMesh;
	vector <Vertex> m_VertexLists;
	vector <unsigned int> m_Indices;

	GLuint m_VBO_Position;
	GLuint m_VBO_Normal;
	GLuint m_VBO_Color;

	const char triangle_vertex_shader[MAX_CONTENT] = "#version 300 es\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "layout (location = 1) in vec3 aNorm\n"
										  "layout (location = 2) in vec3 aColor\n"
										  "out vec3 oColor\n"
										  "void main()\n"
										  "{\n"
										  "    gl_Position = vec4(aPos, 1.0);\n"
										  "    oColor = aColor;\n"
										  "}";

	const char triangle_fragment_shader[MAX_CONTENT] = "#version 300 es\n"
											"precision mediump float;\n"
											"in vec3 oColor;\n"
											"out vec4 FragColor;\n"
											"\n"
											"void main()\n"
											"{\n"
											"    FragColor = (oColor, 1.0);\n"
											"}";
};




