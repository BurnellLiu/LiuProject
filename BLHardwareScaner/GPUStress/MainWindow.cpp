
#include "MainWindow.h"

#define CENTER_REAL -0.56220389591658670447
#define  CEMTER_IMG 0.64281771463971582037
double  DIS = 1.0;
double DIF = 0.995;

LGameWindow::LGameWindow()
{
    


}

LGameWindow::~LGameWindow()
{

}

void LGameWindow::InitGame()
{
    int width = this->GetClientWidth();
    int height = this->GetClientHeight();

    // 初始化背景缓冲区
    HWND hWnd = this->GetWndHandle();
    m_backDC.Init(hWnd, width, height);


    m_param.RealMin = CENTER_REAL - DIS;
    m_param.RealMax = CENTER_REAL + DIS;
    m_param.ImgMin = CEMTER_IMG - DIS;
    m_param.ImgMax = CEMTER_IMG + DIS;
    m_param.MaxIter = 256;

    m_image.Width = width;
    m_image.Height = height;
    m_image.PData = new unsigned int[m_image.Width * m_image.Height];
}

void LGameWindow::RunGame()
{
    m_param.RealMin = CENTER_REAL - DIS;
    m_param.RealMax = CENTER_REAL + DIS;
    m_param.ImgMin = CEMTER_IMG - DIS;
    m_param.ImgMax = CEMTER_IMG + DIS;
    GPUGenerateMandelbrot(m_param, m_image);

    
    if (DIS < 0.000000001)
    {
        DIF = 1.005;
    }
    
    if (DIS > 1.0)
    {
        DIF = 0.995;
    }

    DIS = DIS * DIF;
}

void LGameWindow::PaintGame()
{
    m_backDC.Clear(255, 255, 255);

    HDC hBackDC = m_backDC.GetBackDC();

    int width = this->GetClientWidth();
    int height = this->GetClientHeight();

    SetStretchBltMode(hBackDC, COLORONCOLOR);
    BITMAPINFO  bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
    bmi.bmiHeader.biSizeImage = 0;
    StretchDIBits(hBackDC, 0, 0, width, height, 0, 0, width, height, m_image.PData, &bmi, DIB_RGB_COLORS, SRCCOPY);

    m_backDC.CopyToFrontDC();

}

void LGameWindow::Exe()
{
	this->InitGame();

	bool bDone = false;
	MSG msg;

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

        this->RunGame();

        InvalidateRect(this->GetWndHandle(), NULL, TRUE);
        UpdateWindow(this->GetWndHandle());


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
    mainWnd.SetSize(500, 500);
    mainWnd.SetSizingBorder(false);
    mainWnd.SetShowSysMenu(true);
    mainWnd.SetShowMinimizeBox(false);
    mainWnd.SetShowMaximizeBox(false);
    mainWnd.SetShowCaption(true);
    mainWnd.SetTitle("GPU Stress");

	mainWnd.Show();
	mainWnd.Exe();

	return 0;
}