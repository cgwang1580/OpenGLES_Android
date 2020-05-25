//
// Created by wcg3031 on 2020/5/21.
//

#pragma once

#include "SampleBase.h"

class SampleTransform : public SampleBase
{
public:
	SampleTransform ();
	~SampleTransform();

	virtual RESULT InitSample ();
	virtual void UnInitSample ();
	virtual RESULT OnDrawFrame ();

private:
	RESULT createShader();
	RESULT creteGLBuffer ();

	const char triangle_vertex_shader[MAX_CONTENT] = "#version 300 es\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "void main()\n"
										  "{\n"
										  "    gl_Position = vec4(aPos, 1.0);\n"
										  "}";

	const char triangle_fragment_shader[MAX_CONTENT] = "#version 300 es\n"
											"precision mediump float;\n"
											"out vec4 FragColor;\n"
											"\n"
											"void main()\n"
											"{\n"
											"    FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
											"}";
};



