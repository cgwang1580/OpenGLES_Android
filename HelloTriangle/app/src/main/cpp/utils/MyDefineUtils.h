//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_MYDEFINEUTILS_H
#define HELLOTRIANGLE_MYDEFINEUTILS_H

#include "processor.h"

#define SafeFree(_p_)				if (_p_) {free (_p_); _p_ = NULL;}
#define CHECK_NULL_MALLOC(_p_)		if (!_p_) {return ERROR_MEMORY;}
#define CHECK_NULL_INPUT(_p_)		if (!_p_) {return ERROR_INPUT;}

#define CHECK_MALLOC_BREAK(_p_, _sInfo_)	if (NULL == _p_) { \
												MYLOGE ("%s malloc failed", _sInfo_); \
												break;}

#endif //HELLOTRIANGLE_MYDEFINEUTILS_H
