
#include "MainWindow.h"

#define ZOOM_IN 1.005 // �Ŵ�ϵ��
#define ZOOM_OUT 0.995 // ��Сϵ��

LGameWindow::LGameWindow()
{
    


}

LGameWindow::~LGameWindow()
{
    if (m_image.PData != NULL)
    {
        delete[] m_image.PData;
        m_image.PData = 0;
    }

}

void LGameWindow::InitGame()
{
    int width = this->GetClientWidth();
    int height = this->GetClientHeight();

    // ��ʼ������������
    HWND hWnd = this->GetWndHandle();
    m_backDC.Init(hWnd, width, height);
    
    // ������������
    m_param.MaxIter = 256;

    // ����ͼ����Ҫ���ڴ�
    m_image.Width = width;
    m_image.Height = height;
    m_image.PData = new unsigned int[m_image.Width * m_image.Height];

    // ��ʼ������Ϊ0.999
    m_dis = 0.999;
    // ��ʼ��Ϊ��Сϵ��
    m_scale = ZOOM_OUT;

    CenterPoint cPoint;
    cPoint.X = -1.71070602293477591616;
    cPoint.Y  = -0.00045779170422538751;
    m_cPointList.push_back(cPoint);

    cPoint.X = -0.84001874276769517014;
    cPoint.Y = 0.22430391745363548184;
    m_cPointList.push_back(cPoint);

    cPoint.X = -1.54111536041941143971;
    cPoint.Y = 0.00000000000000126555;
    m_cPointList.push_back(cPoint);

    cPoint.X = -0.56220389591658670447;
    cPoint.Y = 0.64281771463971582037;
    m_cPointList.push_back(cPoint);

    m_cPointIndex = 0;

}

void LGameWindow::RunGame()
{
    static unsigned int g_changeCount = 0;

    // ȡ�����ĵ�λ��
    CenterPoint cPoint = m_cPointList[m_cPointIndex];

    // ���²����ؼ���ռ�
    m_param.RealMin = cPoint.X - m_dis;
    m_param.RealMax = cPoint.X + m_dis;
    m_param.ImgMin = cPoint.Y - m_dis;
    m_param.ImgMax = cPoint.Y + m_dis;

    GPUGenerateMandelbrot(m_param, m_image);

    // �������ֵ�ﵽ��С, ������ϵ����Ϊ�Ŵ�
    if (m_dis <= 0.0000000001)
    {
        m_scale = ZOOM_IN;
        g_changeCount++;
    }
    
    // �������ֵ�ﵽ���, ������ϵ����Ϊ��С
    if (m_dis >= 1.0)
    {
        m_scale = ZOOM_OUT;
        g_changeCount++;
    }

    m_dis = m_dis * m_scale;

    // �л���������ĵ�
    if (g_changeCount == 2)
    {
        g_changeCount = 0;

        m_cPointIndex++;
        if (m_cPointIndex == m_cPointList.size())
            m_cPointIndex = 0;
    }
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

    // ����֡���ʺ��ۼƺ�ʱ
    char strText[256] = {0};
    int length = sprintf_s(strText, 256, "FPS: %u", m_fps);
    SetTextColor(hBackDC, RGB(255, 0, 0));
    SetBkMode(hBackDC, TRANSPARENT); // �����ı�����͸��
    TextOutA(hBackDC, 10, 10, strText, length);
    length = sprintf_s(strText, 256, "Time: %.1lfM", m_timeM);
    TextOutA(hBackDC, 10, 30, strText, length);
    SetBkMode(hBackDC, OPAQUE); // �����ı�������͸��

    m_backDC.CopyToFrontDC();

}

void LGameWindow::Exe()
{
	this->InitGame();

	bool bDone = false;
	
    m_timeM = 0.0;
    m_fps = 0;
    m_frameCount = 0;
    
    LTimer timer;

    MSG msg;
	while(!bDone)
	{
        if (m_frameCount == 0)
        {
            timer.Start();
        }
        m_frameCount++;

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

        // ˢ��UI
        InvalidateRect(this->GetWndHandle(), NULL, TRUE);
        UpdateWindow(this->GetWndHandle());

        timer.End();
        if (timer.Time() >= 1000.0)
        {
            m_fps = (unsigned int)(m_frameCount * 1000 / timer.Time());
            m_frameCount = 0;

            // �ۼƺ�ʱ
            m_timeM += timer.Time() / 60000.0;
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
			// ���ڴ�С�ı����³�ʼ���󱸻�����
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