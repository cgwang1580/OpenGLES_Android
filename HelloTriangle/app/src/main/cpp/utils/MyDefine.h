//
// Created by wcg3031 on 2020/4/1.
//

#ifndef HELLOTRIANGLE_MYDEFINE_H
#define HELLOTRIANGLE_MYDEFINE_H

#include "processor.h"

#define SafeFree(_p_)				if (_p_) {free (_p_); _p_ = NULL;}
#define CHECK_NULL_MALLOC(_p_)		if (!_p_) {return ERROR_MEMORY;}
#define CHECK_NULL_INPUT(_p_)		if (!_p_) {return ERROR_INPUT;}

#endif //HELLOTRIANGLE_MYDEFINE_H
