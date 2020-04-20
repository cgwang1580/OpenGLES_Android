#include "LogAndroid.h"
#include "Shader_Helper.h"

Shader_Helper::Shader_Helper(const char* vertexShaderCode, const char* fragmentShaderCode) {

	//cout << "MyShader begin" << endl;
	LOGD("Shader_Helper begin");

	// compile shader.cpp
	unsigned int vertexShader, fragmentShader;
	int result = 0;
	char infoLog[MY_MAX_PATH * 2] = { 0 };

	do {
		// vertex shader.cpp
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShader);
		// get compile shader.cpp result
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(vertexShader, MY_MAX_PATH * 2, NULL, infoLog);
			//cout << "glCompileShader vertexShader error, " << infoLog << endl;
			LOGE ("glCompileShader vertexShader error, %s", infoLog);
			break;
		}

		// fragment shader.cpp
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);
		// get compile shader.cpp result
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(fragmentShader, MY_MAX_PATH * 2, NULL, infoLog);
			//cout << "glCompileShader fragmentShader error, " << infoLog << endl;
			LOGE ("glCompileShader fragmentShader error, %s ", infoLog);
			break;
		}

		// shader.cpp program
		progreamId = glCreateProgram();
		glAttachShader(progreamId, vertexShader);
		glAttachShader(progreamId, fragmentShader);
		glLinkProgram(progreamId);
		// get link result
		glGetProgramiv(progreamId, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(progreamId, MY_MAX_PATH * 2, NULL, infoLog);
			//cout << "glLinkProgram progreamId error, " << infoLog << endl;
			LOGE ("glLinkProgram progreamId error, %s ", infoLog);
			break;
		}

		// delete shader.cpp
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	} while (false);
}

void Shader_Helper::use() {
	glUseProgram(progreamId);
}

void Shader_Helper::setBool (const string &name, bool value) const{
	glUniform1i(glGetUniformLocation(progreamId, name.c_str()), value);
}

void Shader_Helper::setInt(const string &name, int value) const {
	glUniform1i(glGetUniformLocation(progreamId, name.c_str()), value);
}

void Shader_Helper::setfloat(const string &name, float value) const {
	glUniform1f(glGetUniformLocation(progreamId, name.c_str()), value);
}

void Shader_Helper::setVec3f(const string &name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(progreamId, name.c_str()), value1, value2, value3);
}

/*
void Shader_Helper::setMat4(const string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(progreamId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}*/
