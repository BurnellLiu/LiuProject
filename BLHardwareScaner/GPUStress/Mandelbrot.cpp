

#include "Mandelbrot.h"

#include <amp.h>
#include <amp_math.h>

using namespace concurrency;
using namespace fast_math;

unsigned int set_hsb(float hue, float saturate, float bright) restrict(amp)
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
    // 检查参数
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

    // 计算实部递进比例
    double scaleReal = (param.RealMax - param.RealMin) / image.Width;
    // 计算虚部递进比例
    double scaleImg = (param.ImgMax - param.ImgMin) / image.Height;

    // 定义图像在显存中的映射
    array_view<unsigned int, 2> imageView(image.Height, image.Width, image.PData);
    // 不进行内存到显存的复制
    imageView.discard_data();

    double realMin = param.RealMin;
    double imgMin = param.ImgMin;
    double height = image.Height;
    unsigned int maxIter = param.MaxIter;

    // 使用GPU进行并行运算
    parallel_for_each(imageView.extent, [=](index<2> i) restrict(amp)
    {
        int iReal = i[1];
        int iImg = i[0];

        /*
        曼德勃罗特集迭代公式Zn+1=(Zn)^2+C
        */

        // 每个点对应的C
        double cReal = realMin + (double)iReal * scaleReal;
        double cImg = imgMin + (double)(height - iImg) * scaleImg;

        // Zn
        double zReal = 0;
        double zImg = 0;

        double length = 0;
        double maxLength = 4.0;
        double temp = 0;
        unsigned int count = 0;
        do 
        {
            count++;

            temp = zReal * zReal - zImg * zImg + cReal;
            zImg = 2 * zReal * zImg + cImg;
            zReal = temp;

            length = zReal * zReal + zImg * zImg;

        } while ((count < maxIter) && (length < maxLength));

        double n = count * 0.0078125f; // n = count / 128.0f; 

        double h = 1.0f - 2.0f * fabs(0.5f - n + floor(n));

        //turn points at maximum iteration to black
        double bfactor = direct3d::clamp((double)(maxIter - count), 0.0f, 1.0f);

        imageView[i] = set_hsb(h, 0.7f, (1.0f - h * h * 0.83f) * bfactor);
    });

    imageView.synchronize();

    return true;
}