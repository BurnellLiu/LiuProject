
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "LWindowGDI.h"

#include "Mandelbrot.h"

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
};

#endif