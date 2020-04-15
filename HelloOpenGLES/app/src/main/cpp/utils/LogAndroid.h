//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_LOGANDROID_H
#define HELLOTRIANGLE_LOGANDROID_H

#include "android/log.h"

#define LOG_TAG	"MY_LOG"

#define OUPUT_MY_LOG

#ifdef OUPUT_MY_LOG
#define MYLOGE(...)  __android_log_print (ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define MYLOGD(...)  __android_log_print (ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define MYLOGV(...)  __android_log_print (ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define MYLOGI(...)  __android_log_print (ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define MYLOGE(...)
#define MYLOGD(...)
#define MYLOGV(...)
#define MYLOGI(...)
#endif

#endif //HELLOTRIANGLE_LOGANDROID_H
