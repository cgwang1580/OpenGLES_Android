//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_PROCESSOR_H
#define HELLOTRIANGLE_PROCESSOR_H

#define ERROR_UNKNOWN	0x001
#define ERROR_INPUT		0x001 + 1
#define ERROR_MEMORY	0x001 + 2

#define PHandle			void*

int onSurfaceCreated (PHandle *ppProcessorHandle);
int onSurfaceChanged (const PHandle pProcessorHandle, const int width, const int height);
int onDrawFrame (const PHandle pProcessorHandle);
int onSurfaceDestroyed (PHandle *ppProcessorHandle);

#endif //HELLOTRIANGLE_PROCESSOR_H
