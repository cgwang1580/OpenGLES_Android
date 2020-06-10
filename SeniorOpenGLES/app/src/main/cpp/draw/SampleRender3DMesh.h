//
// Created by wcg3031 on 2020/6/4.
//

#pragma once

#include "SampleBase.h"
#include "Mesh.h"
#include "Model.h"

class SampleRender3DMesh : public SampleBase
{
public:
	SampleRender3DMesh ();
	~SampleRender3DMesh ();
	RESULT OnDrawFrame ();

private:
	void createRenderGLBuffer ();
	void destroyRenderGLBuffer ();

	Model *m_pModel;
	ShaderHelper *m_pShaderHelper;

	const char *pVertex_shader_0 =
			GLES_VERSION_STRING
			R"(
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNom;
layout (location = 2) in vec2 aTexCoord;

out vec4 v_texCoord;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	v_texCoord = aTexCoord;
}
)";

	const char *pFragment_shader_0 =
			GLES_VERSION_STRING
			GLES_MEDIUM_STRING
			R"(
out vec4 FragColor;
in vec2 v_texCoord;
uniform sampler2D texture_diffuse1;
void main()
{
	FragColor = texture(texture_diffuse1, v_texCoord);
}
)";
};



