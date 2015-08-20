
#ifndef _MAIN_H_
#define _MAIN_H_

#include "LWindowGDI.h"

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
	LBackBufferDC m_backDC;
};

#endif