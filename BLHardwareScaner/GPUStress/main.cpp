


#include <cstdio>

#include <Windows.h>
#include <gdiplus.h>
#include <gdiplusinit.h>



using namespace Gdiplus;


#include "Mandelbrot.h"

#pragma  comment(lib, "gdiplus.lib")

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

int main()
{
    MandelbrotParam param;
    param.RealMin = -2.0;
    param.RealMax = 2.0;
    param.ImgMin = -2.0;
    param.ImgMax = 2.0;
    param.MaxIter = 512;

    MandelbrotImage image;
    image.Width = 800;
    image.Height = 800;
    image.PData = new unsigned int[image.Width * image.Height];

    GPUGenerateMandelbrot(param, image);

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