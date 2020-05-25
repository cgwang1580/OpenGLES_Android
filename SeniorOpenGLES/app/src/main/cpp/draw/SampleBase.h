//
// Created by wcg3031 on 2020/5/21.
//

#pragma once

#include <MyDefineUtils.h>
#include "ShaderHelper.h"

#define RESULT int

class SampleBase
{

public:
	virtual ~SampleBase() {
		SafeDelete(m_pShaderHelper);
	}

	virtual RESULT InitSample () = 0;
	virtual void UnInitSample () = 0;
	virtual RESULT OnDrawFrame () = 0;

protected:
	ShaderHelper *m_pShaderHelper;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_sVBO[2];
	GLuint m_Texture;
};