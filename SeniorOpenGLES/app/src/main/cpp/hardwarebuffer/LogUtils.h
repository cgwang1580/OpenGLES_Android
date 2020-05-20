/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and
* confidential information.
*
* The information and code contained in this file is only for authorized ArcSoft employees
* to design, create, modify, or review.
*
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
*
* If you are not an intended recipient of this file, you must not copy, distribute, modify,
* or take any action in reliance on it.
*
* If you have received this file in error, please immediately notify ArcSoft and
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/
/*
* LogUtils.h
*
* Reference:
*
* Description:
*
* Add By xhliu,20150606
*
*/

#ifndef _LOGUTILS_H_20141028
#define _LOGUTILS_H_20141028

#include <string.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored"-Wunused-value"

#ifdef OUTPUT_LOGFILE
#include "LogRecord.h"
#endif

//Log tag
#define  LOG_TAG					"ArcSoft_C"

#if defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/types.h>

#endif

#if defined(ANDROID)//for Android Log defined

#include <android/log.h>

//the log defines
#define  LOGI(...)					__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)					__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)					__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGV(...)					__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

//end for Android Log
#elif defined(_WIN32)

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

void inline MyLogA(const char* ptszFormat, ...)
{
	va_list vlArgs;
	va_start(vlArgs, ptszFormat);

	char szText[1024] = { 0 };
	vsprintf(szText, ptszFormat, vlArgs);
	strcat(szText, "\n");
	OutputDebugStringA(szText);
	va_end(vlArgs);

}
#define LOGD MyLogA
#define LOGE MyLogA
#define LOGV MyLogA
#define LOGI MyLogA

#elif defined(__linux__)

#define DEBUG_INFO(fmt, args...) printf(""#fmt"\r\n", ##args)

#define LOGD DEBUG_INFO
#define LOGE DEBUG_INFO
#define LOGV DEBUG_INFO
#define LOGI DEBUG_INFO

#endif


//JNI_NORMAL is normal log switch
//JNI_PERFORMANCE is the prerformance log switch

#ifdef JNI_NORMAL

#define LOGCATE LOGE
#define LOGCATD LOGD
#define LOGCATV LOGV

#define ENTER_FUNCTION	LOGCATE("%s:%s [TID%lu]Enter<---", __FILE__, __FUNCTION__,LogUtils::GetCurrentThreadID());
#define EXIT_FUNCTION	LOGCATE("%s:%s [TID%lu]Exit--->", __FILE__, __FUNCTION__,LogUtils::GetCurrentThreadID());
#define EXIT_FUNCTION2(_res_)	LOGCATE("%s:%s[TID%lu], return=%ld Exit--->", __FILE__, __FUNCTION__,LogUtils::GetCurrentThreadID(), (MRESULT)(_res_));
#define CHECKPOINT(_POSITION_)	LOGCATE("%s:%s[TID%lu] Checkpoint[%s] at %ld====", __FILE__, __FUNCTION__,LogUtils::GetCurrentThreadID(),_POSITION_ ,__LINE__);

#else
#define LOGCATE
#define LOGCATD
#define LOGCATV

#define ENTER_FUNCTION
#define EXIT_FUNCTION
#define EXIT_FUNCTION2(_res_)
#define CHECKPOINT

#endif	//end of JNI_LOG


//the performance logs
#ifdef JNI_PERFORMANCE
//auto time print
#define AUTO_TIME_PRINT(_x_)  CMyAutoTimePrint _print_1((_x_),false,true);
#define AUTO_TIME_PRINT_EX(_x_)  CMyAutoTimePrint _print_2((_x_),false,false);

#define BEGIN_TIME(FUN) {\
	LOGE("%s:%s[TID%lu] start", __FILE__, FUN, LogUtils::GetCurrentThreadID()); \
	long long t0 = LogUtils::MyGetCurrentTime();

#define END_TIME(FUN) \
	long long t1 = LogUtils::MyGetCurrentTime(); \
	LOGE("%s:%s[TID%lu] cost time %ldms", __FILE__, FUN, LogUtils::GetCurrentThreadID(), (long)(t1 - t0)); }


#else
#define AUTO_TIME_PRINT(_x_)
#define AUTO_TIME_PRINT_EX(_x_)
#define BEGIN_TIME(FUN)
#define END_TIME(FUN)

#endif

/*
LOG to files
*/
#ifdef OUTPUT_LOGFILE

#define INIT_LOGFILE		LogRecord::InitLogRecord
#define RESET_LOGFILE		LogRecord::ResetLogRecord
#define CLOSE_LOGFILE		LogRecord::CloseLogRecord()
#define LOGFILE_D(...)		LogRecord::Log(LogRecord::LEVEL_DETAIL,LOG_TAG,true,true,__VA_ARGS__)
#define LOGFILE_I(...)		LogRecord::Log(LogRecord::LEVEL_INFO,LOG_TAG,true,true,__VA_ARGS__)
#define LOGFILE_E(...)		LogRecord::Log(LogRecord::LEVEL_ERROR,LOG_TAG,true,true,__VA_ARGS__)
#define LOGFILE_P(...)		LogRecord::Log(LogRecord::LEVEL_PERFORMANCE,LOG_TAG,true,true,__VA_ARGS__)
#define LOGFILE_W(...)		LogRecord::Log(LogRecord::LEVEL_WARNING,LOG_TAG,true,true,__VA_ARGS__)


#define AUTO_TIME_PRINT_TOFILE(x) CMyAutoTimePrint timerHelper_1(x,true,true);
#define AUTO_TIME_PRINT_TOFILE_EX(x) CMyAutoTimePrint timerHelper_2(x,true,false);

#else
#define INIT_LOGFILE
#define RESET_LOGFILE		
#define CLOSE_LOGFILE
#define LOGFILE_D(...)
#define LOGFILE_I(...)
#define LOGFILE_E(...)
#define LOGFILE_P(...)
#define LOGFILE_W(...)

#define AUTO_TIME_PRINT_TOFILE(x) 
#define AUTO_TIME_PRINT_TOFILE_EX(x) 

#endif



class LogUtils
{
public :

	static long long MyGetCurrentTime()
	{
#if defined(__linux__)
		struct timeval time;
		gettimeofday(&time, NULL);
		long long curTime = ((long long)(time.tv_sec))*1000+time.tv_usec/1000;
		return curTime;
#elif defined(_WIN32)
		return  (long long)timeGetTime();
#endif
	}

	static unsigned long GetCurrentThreadID()
	{
		unsigned long nThreadID = 0;
#if defined(__linux__)
		nThreadID = syscall(__NR_gettid);//pthread_self();
#elif defined(_WIN32)
		nThreadID = (unsigned long)(::GetCurrentThreadId());
#endif
		return nThreadID;
	}
};
#ifdef _WIN32
class CMyAutoTimePrint
{
public :
	CMyAutoTimePrint(const char *pFunc, bool bLogToFile = false, bool bTraceEnter = true)
	{
		m_bTraceEnter = bTraceEnter;
		m_bLogToFile = bLogToFile;

		if(pFunc)
		{
			strncpy(m_szText,pFunc,sizeof(m_szText) -1);
		}
		QueryPerformanceFrequency(&m_nFreq);
		QueryPerformanceCounter(&m_nBeginTime);
		if (bTraceEnter)
		{
			if (bLogToFile)
			{
				LOGFILE_P("AUTO_PERFORMANCE_TEST: %s, begin", m_szText);
			}
			else
			{
				LOGE("AUTO_PERFORMANCE_TEST[TID%lu]: %s, begin", LogUtils::GetCurrentThreadID(), m_szText);

			}
		}

	}
	~CMyAutoTimePrint()
	{
		QueryPerformanceCounter(&m_nEndTime);

		double time = (double)(m_nEndTime.QuadPart - m_nBeginTime.QuadPart)*1000 / (double)m_nFreq.QuadPart;
		if (m_bLogToFile)
		{

			LOGFILE_P("AUTO_PERFORMANCE_TEST: %s, end cost time %dms", m_szText, (int)(time));
		}
		else
		{
			LOGE("AUTO_PERFORMANCE_TEST[TID%lu]: %s, end cost time %dms", LogUtils::GetCurrentThreadID(), m_szText, (int)(time));

		}
	}
private :
	char m_szText[1024];
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER m_nEndTime;
	bool m_bLogToFile;
	bool m_bTraceEnter;

};
#else
class CMyAutoTimePrint
{
public:
	CMyAutoTimePrint(const char *pFunc, bool bLogToFile = false, bool bTraceEnter = true)
	{
		m_bTraceEnter = bTraceEnter;
		m_bLogToFile = bLogToFile;

		if (pFunc)
		{
			strncpy(m_szText, pFunc, sizeof(m_szText) - 1);
		}
		m_tStart = LogUtils::MyGetCurrentTime();
		if (bTraceEnter)
		{
			if (bLogToFile)
			{
				LOGFILE_P("AUTO_PERFORMANCE_TEST: %s, begin", m_szText);
			}
			else
			{
				LOGE("AUTO_PERFORMANCE_TEST[TID%lu]: %s, begin", LogUtils::GetCurrentThreadID(), m_szText);

			}
		}

	}
	~CMyAutoTimePrint()
	{
		if (m_bLogToFile)
		{

			LOGFILE_P("AUTO_PERFORMANCE_TEST: %s, end cost time %dms", m_szText, (int)(LogUtils::MyGetCurrentTime() - m_tStart));
		}
		else
		{
			LOGE("AUTO_PERFORMANCE_TEST[TID%lu]: %s, end cost time %dms", LogUtils::GetCurrentThreadID(), m_szText, (int)(LogUtils::MyGetCurrentTime() - m_tStart));

		}
	}
private:
	long long m_tStart;
	char m_szText[1024];
	bool m_bTraceEnter;
	bool m_bLogToFile;

};
#endif



#endif	//end of __JNICOMMONH__
