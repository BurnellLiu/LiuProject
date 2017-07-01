

#include "Mandelbrot.h"

#include <amp.h>
#include <amp_math.h>

using namespace concurrency;
using namespace fast_math;

/// <SUMMARY>
/// HSBģʽת��ΪRGBģʽ
/// </SUMMARY>
/// <PARAM name="hue" dir="IN">
/// ɫ��, ��0~360��ı�׼ɫ����, ɫ���ǰ�λ�ö�����, ȡֵ��Χ0.0~1.0
/// </PARAM>
/// <PARAM name="saturate" dir="IN">
/// ���Ͷ�, ��ʾɫ�ʵĴ���, ȡֵ��Χ0.0~1.0
/// </PARAM>
/// <PARAM name="bright" dir="IN">
/// ����, ��ʾɫ�ʵ�������, ȡֵ��Χ0.0~1.0
/// </PARAM>
/// <RETURN>
/// ����RGB��ɫֵ
/// </RETURN>
unsigned int HSB2RGB(IN float hue, IN float saturate, IN float bright) restrict(amp)
{

    float red, green, blue;
    float h = (hue * 256) / 60;
    float p = bright * (1 - saturate);
    float q = bright * (1 - saturate * (h - (int)h));
    float t = bright * (1 - saturate * (1 - (h - (int)h)));

    switch ((int)h) {
    case 0:
        red = bright, green = t, blue = p;
        break;
    case 1:
        red = q, green = bright, blue = p;
        break;
    case 2:
        red = p, green = bright, blue = t;
        break;
    case 3:
        red = p, green = q, blue = bright;
        break;
    case 4:
        red = t, green = p, blue = bright;
        break;
    case 5:
    case 6:
        red = bright, green = p, blue = q;
        break;
    }

    unsigned int ired, igreen, iblue;
    ired = (unsigned int)(red * 255.0f);
    igreen = (unsigned int)(green * 255.0f);
    iblue = (unsigned int)(blue * 255.0f);

    return 0xff000000 | (ired << 16) | (igreen << 8) | iblue;
}


bool GPUGenerateMandelbrot(IN MandelbrotParam& param, INOUT MandelbrotImage& image)
{
    // ������
    if (image.Width == 0 || 
        image.Height == 0 || 
        image.PData == 0)
    {
        return false;
    }
    if (param.MaxIter == 0 || 
        param.RealMin >= param.RealMax || 
        param.ImgMin >= param.ImgMax)
    {
        return false;
    }

    const double REAL_MIN = param.RealMin;
    const double REAL_MAX = param.RealMax;
    const double IMG_MIN = param.ImgMin;
    const double IMG_MAX = param.ImgMax;
    const unsigned int HEIGHT = image.Height;
    const unsigned int WIDTH = image.Width;
    const unsigned int MAX_ITER = param.MaxIter;

    // ʵ���ݽ�����
    const double SCALE_REAL = (REAL_MAX - REAL_MIN) / WIDTH;
    // �鲿�ݽ�����
    const double SCALE_IMG = (IMG_MAX - IMG_MIN) / HEIGHT;

    // ����ͼ�����Դ��е�ӳ��
    array_view<unsigned int, 2> imageView(HEIGHT, WIDTH, image.PData);
    // �����ڴ浽�Դ�ĸ���
    imageView.discard_data();

    // ʹ��GPU���в�������
    parallel_for_each(imageView.extent, [=](index<2> i) restrict(amp)
    {
        int iReal = i[1]; // ������, Ҳ����X��(ʵ��)
        int iImg = i[0]; // ������, Ҳ����Y��(����)

        /*
        ���²����ؼ�������ʽZn+1=(Zn)^2+C
        */

        // ÿ�����Ӧ��C
        double cReal = REAL_MIN + (double)iReal * SCALE_REAL;
        double cImg = IMG_MIN + (double)(HEIGHT - iImg) * SCALE_IMG;

        // Zn��ʼΪ0 0
        double zReal = 0;
        double zImg = 0;

        const double MAX_LENGTH = 4.0;
        double length = 0;
        double temp = 0;
        unsigned int count = 0;
        do 
        {
            count++;

            // ����Zn+1��ʵ��
            temp = zReal * zReal - zImg * zImg + cReal;

            // ����Zn+1���鲿
            zImg = 2 * zReal * zImg + cImg;

            zReal = temp;

            length = zReal * zReal + zImg * zImg;

        } while ((count < MAX_ITER) && (length < MAX_LENGTH));
        
        // ����ɫ��
        float n = count / 64.0f; 
        float h = 1.0f - 2.0f * fabs(0.5f - n + floor(n));
        
        // ���ݵ������Ϊ0, ���ݵ㼴��cout == MAX_ITER
        float bfactor = direct3d::clamp((float)(MAX_ITER - count), 0.0f, 1.0f);

        imageView[i] = HSB2RGB(h, 0.75f, (1.0f - h * h * 0.83f) * bfactor);
    });

    // ���Դ��е����ݿ������ڴ�
    imageView.synchronize();

    return true;
}