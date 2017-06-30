


#include <cstdio>

#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusinit.h>



using namespace Gdiplus;


#include "Mandelbrot.h"

#pragma  comment(lib, "gdiplus.lib")

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
        time = (double)(m_time * 1000) / (double)m_performanceFreq;
        return time;
    }

private:
    LONGLONG m_time; ///< ��ʱʱ�� 
    LONGLONG m_startTime; ///< ��ʱ����ʼʱ�� 
    LONGLONG m_performanceFreq; ///< CPUʱ��Ƶ��(��ÿ���ӵĵδ���)
};

int   GetEncoderClsid(const   WCHAR*   format, CLSID*   pClsid)
{
    UINT     num = 0;                     //   number   of   image   encoders 
    UINT     size = 0;                   //   size   of   the   image   encoder   array   in   bytes 

    ImageCodecInfo*   pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return   -1;     //   Failure 

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return   -1;     //   Failure 

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return   j;     //   Success 
        }
    }

    free(pImageCodecInfo);
    return   -1;     //   Failure 
}

int main22()
{
    double CENTER_REAL = -0.56220389591658670447;
    double CEMTER_IMG = 0.64281771463971582037;
    double DIS = 0.000000001;
    MandelbrotParam param;
    param.RealMin = CENTER_REAL - DIS;
    param.RealMax = CENTER_REAL + DIS;
    param.ImgMin = CEMTER_IMG - DIS;
    param.ImgMax = CEMTER_IMG + DIS;
    param.MaxIter = 256;

    MandelbrotImage image;
    image.Width = 800;
    image.Height = 800;
    image.PData = new unsigned int[image.Width * image.Height];

    LTimer timer;

    timer.Start();

    GPUGenerateMandelbrot(param, image);

    timer.End();

    printf("Time: %f\n", timer.Time());

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    Bitmap output(800, 800, 800 * 4, PixelFormat32bppARGB, reinterpret_cast<BYTE*>(image.PData));

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);

    output.Save(L"test.png", &pngClsid, nullptr);

    system("pause");

    if (image.PData != 0)
    {
        delete[] image.PData;
        image.PData = 0;
    }
    return 0;
}