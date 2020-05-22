//
// Created by wcg3031 on 2020/4/2.
//

#ifndef HELLOTRIANGLE_DRAW_UTILS_H
#define HELLOTRIANGLE_DRAW_UTILS_H

#include "ShaderHelper.h"
#include "processor_inner.h"

int CreateShaderHelper (LPShaderSet pShaderSet, const string vShader, const string fShader);

int drawTriangle (ShaderHelper *pShaderHelper);

int drawTexture (ShaderHelper *pShaderHelper, const LPMyImageInfo lpMyImageInfo);

int drawFBO (ShaderHelper *pShaderHelperFBO, ShaderHelper *pShaderHelperNormal, const LPMyImageInfo lpMyImageInfo);

int drawByHardwareBuffer (ShaderHelper *pShaderHelperHardwareNormal, const AHardwareBufferHelper *pHardwareBufferHelper, const LPMyImageInfo lpMyImageInfo);

int drawByHardwareBuffer2 (const LPMyImageInfo lpMyImageInfo);

#endif //HELLOTRIANGLE_DRAW_UTILS_H
