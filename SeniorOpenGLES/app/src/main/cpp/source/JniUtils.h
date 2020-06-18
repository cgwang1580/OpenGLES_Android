//
// Created by wcg3031 on 2020/6/18.
//

#pragma once

#include <LogAndroid.h>
#include <MyDefineUtils.h>

int ConvertMotionState (JNIEnv *env, jobject jMotionObject, MotionState &motionState)
{
	LOGD ("ConvertMotionState");
	CHECK_NULL_INPUT(env);
	CHECK_NULL_INPUT(jMotionObject);

	jclass jMotionClass = env->GetObjectClass(jMotionObject);
	if (nullptr == jMotionClass)
	{
		LOGE("ConvertMotionState jMotionClass is nullptr");
		return ERROR_INPUT;
	}

	jfieldID motionTypeID = env->GetFieldID(jMotionClass, "mMotionType", "I");
	jfieldID transformXID = env->GetFieldID(jMotionClass, "translate_x", "F");
	jfieldID transformYID = env->GetFieldID(jMotionClass, "translate_y", "F");
	jfieldID transformZID = env->GetFieldID(jMotionClass, "translate_z", "F");

	motionState.eMotionType = (MotionType)env->GetIntField(jMotionObject, motionTypeID);
	motionState.transform_x = env->GetFloatField(jMotionObject, transformXID);
	motionState.transform_y = env->GetFloatField(jMotionObject, transformYID);
	motionState.transform_z = env->GetFloatField(jMotionObject, transformZID);

	LOGD("ConvertMotionState motionState %d, (%f %f %f)", motionState.eMotionType,
			motionState.transform_x, motionState.transform_y, motionState.transform_z);
	env->DeleteLocalRef(jMotionClass);

	return ERROR_OK;
}