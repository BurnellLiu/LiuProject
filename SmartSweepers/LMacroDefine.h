#ifndef _MACRODEFINE_H
#define _MACRODEFINE_H

#include <stdlib.h>
#include <math.h>
#include <time.h>


#include <string>

#include <Windows.h>

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

//-----------------------------------------------------------------------
// 键盘按键检测宏(异步)
//-----------------------------------------------------------------------
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//-----------------------------------------------------------------------
// 安全删除GDI对象宏宏
//-----------------------------------------------------------------------
#ifndef SAFE_DELETE_GDI
#define SAFE_DELETE_GDI(p) \
do\
{\
	if ((p) != NULL)\
	{ \
		DeleteObject(p);\
		p = NULL; \
	}\
}while(0)
#endif



/// @高精度计时器类
class LTimer
{
public:
	LTimer()
	{
		// 获取CPU的时钟频率(即每秒的滴答数)
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_performanceFreq);
	}

	/// @brief 开始计时
	void Start()
	{
		m_startTime = 0;
		m_time = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	}

	/// @brief 结束计时
	void End()
	{
		LONGLONG currentTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		m_time = currentTime - m_startTime;
	}

	/// @brief 获取计时时间
	/// @return 计时时间(单位毫秒)
	double Time()
	{
		double time = 0.0f;
		time = (double)(m_time * 1000)/(double)m_performanceFreq;
		return time;
	}

private:
	LONGLONG m_time; ///< 计时时间 
	LONGLONG m_startTime; ///< 计时器开始时间 
	LONGLONG m_performanceFreq; ///< CPU时钟频率(即每秒钟的滴答数)
};


#endif