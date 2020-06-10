//
// Created by wcg3031 on 2020/4/1.
//

#pragma once

typedef void* 	PHandle;

int onSurfaceCreated (PHandle *ppProcessorHandle);
int onSurfaceChanged (const PHandle pProcessorHandle, const int width, const int height);
int onDrawFrame (const PHandle pProcessorHandle);
int onSurfaceDestroyed (PHandle *ppProcessorHandle);

