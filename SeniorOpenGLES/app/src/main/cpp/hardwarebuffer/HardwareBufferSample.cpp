#include <malloc.h>
#include "HardwareBufferHelper.h"
#include "amcomdef.h"
#include "asvloffscreen.h"


void main()
{
	// OpenGLES Environment
	// only support NV12 or NV21
	// link EGL android GLESv3 lib
	// __ANDROID_API__ >= 26
	
	int imgW = 1920, imgH = 1080, rgbaTextureId;
	
	HardwareBufferHelper *hardwareBufferHelper = new HardwareBufferHelper();
    hardwareBufferHelper->Init(imgW, imgH, ASVL_PAF_NV21);
	
	ASVLOFFSCREEN outputImg = {0};
	outputImg.i32Width = imgW;
	outputImg.i32Height = imgH;
	outputImg.pi32Pitch[0] = imgW;
	outputImg.pi32Pitch[1] = imgW;
	outputImg.u32PixelArrayFormat = ASVL_PAF_NV21;
	outputImg.ppu8Plane[0] = static_cast<MUInt8 *>(malloc(imgW * imgH * 1.5));
	outputImg.ppu8Plane[1] = outputImg.ppu8Plane[0] + outputImg.pi32Pitch[0] * outputImg.i32Height;

	hardwareBufferHelper->DrawFrame(rgbaTextureId, &outputImg);

	free(outputImg.ppu8Plane[0]);
	
	hardwareBufferHelper->UnInit();
	delete hardwareBufferHelper;
	hardwareBufferHelper = NULL;
	
}