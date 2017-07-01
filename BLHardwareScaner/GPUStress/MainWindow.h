
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
	LBackBufferDC m_backDC; // 背景缓冲区

    MandelbrotParam m_param; // 曼德勃罗特参数
    MandelbrotImage m_image; // 曼德菠萝特图像参数
};

#endif