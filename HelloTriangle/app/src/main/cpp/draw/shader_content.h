//
// Created by wcg3031 on 2020/4/2.
//

#ifndef HELLOTRIANGLE_SHADER_CONTENT_H
#define HELLOTRIANGLE_SHADER_CONTENT_H

#include "string"

using namespace std;

static const string triangle_vertex_shader = "#version 300 es\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "out float g_color = 0.1f;\n"
									 "void main()\n"
									 "{\n"
									 "    gl_Position = vec4(aPos, 1.0);\n"
		  							 "	  g_color = "
									 "}";

static const string triangle_fragment_shader = "#version 300 es\n"
											   "precision mediump float;\n"
			  								   "in float g_color;"
											   "out vec4 FragColor;\n"
											   "\n"
											   "void main()\n"
											   "{\n"
											   "    FragColor = vec4(1.0f, g_color, 0.2f, 1.0f);\n"
											   "}";

#endif //HELLOTRIANGLE_SHADER_CONTENT_H
