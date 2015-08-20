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
// ���̰�������(�첽)
//-----------------------------------------------------------------------
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//-----------------------------------------------------------------------
// ��ȫɾ��GDI������
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



/// @�߾��ȼ�ʱ����
class LTimer
{
public:
	LTimer()
	{
		// ��ȡCPU��ʱ��Ƶ��(��ÿ��ĵδ���)
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_performanceFreq);
	}

	/// @brief ��ʼ��ʱ
	void Start()
	{
		m_startTime = 0;
		m_time = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	}

	/// @brief ������ʱ
	void End()
	{
		LONGLONG currentTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		m_time = currentTime - m_startTime;
	}

	/// @brief ��ȡ��ʱʱ��
	/// @return ��ʱʱ��(��λ����)
	double Time()
	{
		double time = 0.0f;
		time = (double)(m_time * 1000)/(double)m_performanceFreq;
		return time;
	}

private:
	LONGLONG m_time; ///< ��ʱʱ�� 
	LONGLONG m_startTime; ///< ��ʱ����ʼʱ�� 
	LONGLONG m_performanceFreq; ///< CPUʱ��Ƶ��(��ÿ���ӵĵδ���)
};


#endif