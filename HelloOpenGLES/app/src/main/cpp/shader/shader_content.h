//
// Created by wcg3031 on 2020/4/2.
//

#ifndef HELLOTRIANGLE_SHADER_CONTENT_H
#define HELLOTRIANGLE_SHADER_CONTENT_H

#include "string"

using namespace std;

static const string triangle_vertex_shader = "#version 300 es\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "void main()\n"
									 "{\n"
									 "    gl_Position = vec4(aPos, 1.0);\n"
									 "}";

static const string triangle_fragment_shader = "#version 300 es\n"
											   "precision mediump float;\n"
											   "uniform float g_color;\n"
											   "out vec4 FragColor;\n"
											   "\n"
											   "void main()\n"
											   "{\n"
											   "    FragColor = vec4(1.0f, g_color, 0.2f, 1.0f);\n"
											   "}";



static const string texture_vertex_shader = "#version 300 es\n"
											"\n"
											"layout (location = 0) in vec3 aPos;\n"
											"layout (location = 1) in vec3 aColor;\n"
											"layout (location = 2) in vec2 tPos;\n"
											"out vec2 TexCoord;\n"
											"out vec3 TexColor;\n"
											"\n"
											"void main()\n"
											"{\n"
											"    gl_Position = vec4(aPos, 1.0);\n"
											"    TexCoord = tPos;\n"
											"    TexColor = aColor;\n"
											"}";

static const string texture_fragment_shader = "#version 300 es\n"
											  "precision mediump float;\n"
											  "\n"
											  "out vec4 FragColor;\n"
											  "in vec2 TexCoord;\n"
											  "in vec3 TexColor;\n"
											  "uniform sampler2D texture1;\n"
											  "\n"
											  "void main()\n"
											  "{\n"
											  "    FragColor = texture (texture1, TexCoord) * vec4 (TexColor, 1.0);\n"
											  "}";

#endif //HELLOTRIANGLE_SHADER_CONTENT_H
