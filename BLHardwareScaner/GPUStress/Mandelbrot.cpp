

#include "Mandelbrot.h"

#include <amp.h>
#include <amp_math.h>

using namespace concurrency;
using namespace fast_math;

/// <SUMMARY>
/// HSB模式转换为RGB模式
/// </SUMMARY>
/// <PARAM name="hue" dir="IN">
/// 色相, 在0~360°的标准色轮上, 色相是按位置度量的, 取值范围0.0~1.0
/// </PARAM>
/// <PARAM name="saturate" dir="IN">
/// 饱和度, 表示色彩的纯度, 取值范围0.0~1.0
/// </PARAM>
/// <PARAM name="bright" dir="IN">
/// 亮度, 表示色彩的明亮度, 取值范围0.0~1.0
/// </PARAM>
/// <RETURN>
/// 返回RGB颜色值
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

    const double REAL_MIN = param.RealMin;
    const double REAL_MAX = param.RealMax;
    const double IMG_MIN = param.ImgMin;
    const double IMG_MAX = param.ImgMax;
    const unsigned int HEIGHT = image.Height;
    const unsigned int WIDTH = image.Width;
    const unsigned int MAX_ITER = param.MaxIter;

    // 实部递进比例
    const double SCALE_REAL = (REAL_MAX - REAL_MIN) / WIDTH;
    // 虚部递进比例
    const double SCALE_IMG = (IMG_MAX - IMG_MIN) / HEIGHT;

    // 定义图像在显存中的映射
    array_view<unsigned int, 2> imageView(HEIGHT, WIDTH, image.PData);
    // 放弃内存到显存的复制
    imageView.discard_data();

    // 使用GPU进行并行运算
    parallel_for_each(imageView.extent, [=](index<2> i) restrict(amp)
    {
        int iReal = i[1]; // 列索引, 也就是X轴(实轴)
        int iImg = i[0]; // 行索引, 也就是Y轴(虚轴)

        /*
        曼德勃罗特集迭代公式Zn+1=(Zn)^2+C
        */

        // 每个点对应的C
        double cReal = REAL_MIN + (double)iReal * SCALE_REAL;
        double cImg = IMG_MIN + (double)(HEIGHT - iImg) * SCALE_IMG;

        // Zn初始为0 0
        double zReal = 0;
        double zImg = 0;

        const double MAX_LENGTH = 4.0;
        double length = 0;
        double temp = 0;
        unsigned int count = 0;
        do 
        {
            count++;

            // 计算Zn+1的实部
            temp = zReal * zReal - zImg * zImg + cReal;

            // 计算Zn+1的虚部
            zImg = 2 * zReal * zImg + cImg;

            zReal = temp;

            length = zReal * zReal + zImg * zImg;

        } while ((count < MAX_ITER) && (length < MAX_LENGTH));
        
        // 计算色相
        float n = count / 64.0f; 
        float h = 1.0f - 2.0f * fabs(0.5f - n + floor(n));
        
        // 逃逸点的亮度为0, 逃逸点即是cout == MAX_ITER
        float bfactor = direct3d::clamp((float)(MAX_ITER - count), 0.0f, 1.0f);

        imageView[i] = HSB2RGB(h, 0.75f, (1.0f - h * h * 0.83f) * bfactor);
    });

    // 将显存中的数据拷贝回内存
    imageView.synchronize();

    return true;
}