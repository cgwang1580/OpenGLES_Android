//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_PROCESSOR_INNER_H
#define HELLOTRIANGLE_PROCESSOR_INNER_H

#include "ShaderHelper.h"
#include "processor.h"
#include <string>
#include <AHardwareBufferHelper.h>
#include <SampleTransform.h>
#include <SampleRender3D.h>
#include <SampleDrawFBO.h>

using namespace std;

typedef struct __tag_color_set_
{
	float r;
	float g;
	float b;
	float alpha;
}ColorSet, *LPColorSet;

typedef struct __shader_set_
{
	string vertexShader;
	string fragmentShader;
	ShaderHelper *pShaderHelper;
}ShaderSet, *LPShaderSet;

typedef struct __tag_processor_handle
{
	int mRenderTime;
	ColorSet mColorSet;
	ShaderSet mShaderSetTriangle;
	ShaderSet mShaderSetTexture;
	ShaderSet mShaderSetFBO;
	ShaderSet mShaderSetFBONormal;
	ShaderSet mShaderSetHardware;
	ShaderSet mShaderSetHardwareNormal;
	LPMyImageInfo lpMyImageInfo;
	LPMyImageInfo lpMyImageInfo_YUV;
	AHardwareBufferHelper *pHardwareBufferHelper;
	SampleTransform *m_pSampleTransform;
	SampleRender3D *m_pSampleRender3D;
	SampleDrawFBO * m_pSampleDrawFBO;
} ProcessorHandle, *LPProcessorHandle;

#endif //HELLOTRIANGLE_PROCESSOR_H
