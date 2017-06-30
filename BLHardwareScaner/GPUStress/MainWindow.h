
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "LWindowGDI.h"

#include "Mandelbrot.h"

class LMainWindow : public LWindow
{
public:
	LMainWindow();
	virtual ~LMainWindow();

	virtual void Exe();

private:
	void InitGame();
	void PaintGame();
	void RunGame();
	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

private:
	LBackBufferDC m_backDC;

    MandelbrotParam m_param;
    MandelbrotImage m_image;
};

#endif