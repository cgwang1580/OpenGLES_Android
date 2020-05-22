//
// Created by wcg3031 on 2020/5/21.
//

#pragma once

#include "SampleBase.h"

class SampleTransform : public SampleBase
{
public:
	SampleTransform ();
	~SampleTransform();

	virtual RESULT InitSample ();
	virtual void UnInitSample ();
	virtual RESULT OnDrawFrame ();

private:
	virtual RESULT createShader();
	virtual RESULT creteGLBuffer ();
};



