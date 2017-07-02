
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <vector>
using std::vector;

#include "LWindowGDI.h"

#include "Mandelbrot.h"


struct CenterPoint 
{
    double X;
    double Y;
};

class LGameWindow : public LWindow
{
public:
	LGameWindow();
	virtual ~LGameWindow();

	virtual void Exe();

private:
	void InitGame();
	void PaintGame();
	void RunGame();
	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

private:
	LBackBufferDC m_backDC; // ����������

    MandelbrotParam m_param; // ���²����ز���
    MandelbrotImage m_image; // ���²�����ͼ�����

    double m_dis; // ���²����ؼ���ռ�, �������¾������ĵ�ľ���
    double m_scale; // ���²����ؼ���ռ�����ϵ��

    unsigned int m_cPointIndex; // ���ĵ�����
    vector<CenterPoint> m_cPointList; // ���ĵ��б�

    unsigned int m_frameCount; // ����֡����
    unsigned int m_fps; // ÿ��֡�ٶ�
    double m_timeM; // ��ʱ, ��λ����
};

#endif