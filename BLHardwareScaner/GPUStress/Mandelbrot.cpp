

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

struct IndexColor
{
    double Index;
    unsigned int R;
    unsigned int G;
    unsigned int B;
};

unsigned int ColorMap(double index) restrict(amp)
{
    if (index < 0.0 || index > 1.0)
        return 0;

    IndexColor COLOR_MAP[33] = {
        {0.0    ,   59,  76, 192}, // #3B4CC0
        {0.03125,   68,  90, 204}, // #445ACC
        {0.0625 ,   77, 104, 215}, // #4D68D7
        {0.09375,   87, 117, 225}, // #5775E1
        {0.125  ,   98, 130, 234}, // #6282EA
        {0.15625,  108, 142, 241}, // #6C8EF1
        {0.1875 ,  119, 154, 247}, // #779AF7
        {0.21875,  130, 165, 251}, // #82A5FB
        {0.25   ,  141, 176, 254}, // #8DB0FE
        {0.28125,  152, 185, 255}, // #98B9FF
        {0.3125 ,  163, 194, 255}, // #A3C2FF
        {0.34375,  174, 201, 253}, // #AEC9FD
        {0.375  ,  184, 208, 249}, // #B8D0F9
        {0.40625,  194, 213, 244}, // #C2D5F4
        {0.4375 ,  204, 217, 238}, // #CCD9EE
        {0.46875,  213, 219, 230}, // #D5DBE6
        {0.5    ,  221, 221, 221}, // #DDDDDD
        {0.53125,  229, 216, 209}, // #E5D8D1
        {0.5625 ,  236, 211, 197}, // #ECD3C5
        {0.59375,  241, 204, 185}, // #F1CCB9
        {0.625  ,  245, 196, 173}, // #F5C4AD
        {0.65625,  247, 187, 160}, // #F7BBA0
        {0.6875 ,  247, 177, 148}, // #F7B194
        {0.71875,  247, 166, 135}, // #F7A687
        {0.75   ,  244, 154, 123}, // #F49A7B
        {0.78125,  241, 141, 111}, // #F18D6F
        {0.8125 ,  236, 127,  99}, // #EC7F63
        {0.84375,  229, 112,  88}, // #E57058
        {0.875  ,  222,  96,  77}, // #DE604D
        {0.90625,  213,  80,  66}, // #D55042
        {0.9375 ,  203,  62,  56}, // #CB3E38
        {0.96875,  192,  40,  47}, // #C0282F
        {1.0    ,  180,   4,  38}  // #B40426
    };
        

    unsigned int R = 0;
    unsigned int G = 0;
    unsigned int B = 0;

    for (int i = 0; i < 32; i++)
    {
        if (index >= COLOR_MAP[i].Index &&
            index < COLOR_MAP[i + 1].Index)
        {
            R = COLOR_MAP[i].R;
            G = COLOR_MAP[i].G;
            B = COLOR_MAP[i].B;
        }
    }

    if (index == 1.0)
    {
        R = COLOR_MAP[32].R;
        G = COLOR_MAP[32].G;
        B = COLOR_MAP[32].B;
    }


    return 0xff000000 | (R << 16) | (G << 8) | B;
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
    // 不进行内存到显存的复制
    imageView.discard_data();



    // 使用GPU进行并行运算
    parallel_for_each(imageView.extent, [=](index<2> i) restrict(amp)
    {
        int iReal = i[1];
        int iImg = i[0];

        /*
        曼德勃罗特集迭代公式Zn+1=(Zn)^2+C
        */

        // 每个点对应的C
        double cReal = REAL_MIN + (double)iReal * SCALE_REAL;
        double cImg = IMG_MIN + (double)(HEIGHT - iImg) * SCALE_IMG;

        // Zn
        double zReal = 0;
        double zImg = 0;

        const double MAX_LENGTH = 4.0;
        double length = 0;
        double temp = 0;
        unsigned int count = 0;
        do 
        {
            count++;

            temp = zReal * zReal - zImg * zImg + cReal;
            zImg = 2 * zReal * zImg + cImg;
            zReal = temp;

            length = zReal * zReal + zImg * zImg;

        } while ((count < MAX_ITER) && (length < MAX_LENGTH));

//         double n = count * 0.0078125f; // n = count / 128.0f; 
// 
//         double h = 1.0 - 2.0 * fabs(0.5 - n + floor(n));
// 
//         //turn points at maximum iteration to black
//         double bfactor = direct3d::clamp((double)(MAX_ITER - count), 0.0f, 1.0f);

        // imageView[i] = set_hsb(h, 0.7f, (1.0f - h * h * 0.83f) * bfactor);
        imageView[i] = ColorMap((double)count / (double)MAX_ITER);
    });

    imageView.synchronize();

    return true;
}