//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_PROCESSOR_H
#define HELLOTRIANGLE_PROCESSOR_H

typedef enum _enum_error_code_
{
	ERROR_OK = 0x00,
	ERROR_UNKNOWN,
	ERROR_INPUT,
	ERROR_MEMORY,
	ERROR_IMAGE
}ERROR_CODE;

#define PHandle			void*

int onSurfaceCreated (PHandle *ppProcessorHandle);
int onSurfaceChanged (const PHandle pProcessorHandle, const int width, const int height);
int onDrawFrame (const PHandle pProcessorHandle);
int onSurfaceDestroyed (PHandle *ppProcessorHandle);

#endif //HELLOTRIANGLE_PROCESSOR_H
