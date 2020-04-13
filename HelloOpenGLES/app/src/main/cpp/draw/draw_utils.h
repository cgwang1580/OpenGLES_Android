//
// Created by wcg3031 on 2020/4/2.
//

#ifndef HELLOTRIANGLE_DRAW_UTILS_H
#define HELLOTRIANGLE_DRAW_UTILS_H

#include "Shader_Helper.h"
#include "processor_inner.h"

int CreateShaderHelper (LPShaderSet pShaderSet, const string vShader, const string fShader);

int drawTriangle (Shader_Helper *pShaderHelper);

int drawTexture (Shader_Helper *pShaderHelper);

int drawTexture (Shader_Helper *pShaderHelper, const int nWidth, const int nHeight, const unsigned char *pData);

#endif //HELLOTRIANGLE_DRAW_UTILS_H
