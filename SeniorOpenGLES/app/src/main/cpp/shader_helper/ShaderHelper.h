/***
** this is a class for shader 
** created by chauncy_wang at 5/7/2019
***/

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <GLES3/gl3.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#define MY_MAX_PATH		256

using namespace std;

class ShaderHelper {

public:
	// generator
	ShaderHelper(const char* vertexShaderCode, const char* fragmentShaderCode);
	int getShaderHelperState ();
	void use();
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	void setVec3f(const string &name, float value1, float value2, float value3) const;
	void setMat4(const string &name, const glm::mat4 &mat) const;

private:
	unsigned int m_nProgramId;
	int m_nShaderState;
};
