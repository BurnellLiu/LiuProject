
#include "main.h"

LGameWindow::LGameWindow()
{

}

LGameWindow::~LGameWindow()
{

}

void LGameWindow::InitGame()
{

}

void LGameWindow::RunGame()
{

}

void LGameWindow::PaintGame()
{

}

void LGameWindow::Exe()
{
	this->InitGame();

	bool bDone = false;
	MSG msg;

	LFrameTimer frameTimer(60);
	frameTimer.Start();

	while(!bDone)
	{
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				bDone = true;
			} 
			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		if (frameTimer.ReadyForNextFrameEx())
		{
			this->RunGame();

			InvalidateRect(GetWndHandle(), NULL, TRUE);
			UpdateWindow(GetWndHandle());
		}
		else
		{
			Sleep(0);
		}

	}
}



LRESULT LGameWindow::MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
	switch (message)
	{	
	case WM_DESTROY:
		PostQuitMessage (0);
		return 0;
	case WM_SIZE:
		{
			// 窗口大小改变重新初始化后备缓冲区
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			this->SetSize(width, height);
			HWND hWnd = this->GetWndHandle();
			m_backDC.Init(hWnd, width, height);
		}
		break;
	case WM_PAINT:
		{
			this->PaintGame();
		}
		break;
	default:
		break;
	}

	return DefWindowProc(GetWndHandle(), message, wParam, lParam);
}

int LMain()
{
	LGameWindow mainWnd;
	mainWnd.Show();
	mainWnd.Exe();

	return 0;
}