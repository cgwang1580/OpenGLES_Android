//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_PROCESSOR_H
#define HELLOTRIANGLE_PROCESSOR_H

#define ERROR_UNKNOWN	0x001
#define ERROR_INPUT		0x001 + 1
#define ERROR_MEMORY	0x001 + 2

typedef struct __tag_processor_handle
{
	int mRenderTime;
	float r;
	float g;
	float b;
} PROCESSOR_HANDLE, *LPPROCESSOR_HANDLE;

int onSurfaceCreated (LPPROCESSOR_HANDLE *ppProcessorHandle);
int onSurfaceChanged (const LPPROCESSOR_HANDLE pProcessorHandle);
int onDrawFrame (const LPPROCESSOR_HANDLE pProcessorHandle);
int onSurfaceDestroyed (LPPROCESSOR_HANDLE *ppProcessorHandle);

#endif //HELLOTRIANGLE_PROCESSOR_H
