
#ifndef _UIDEFINES_H_
#define _UIDEFINES_H_

#include <Windows.h>


/// @brief 获取UI放大系数
///  
/// @return 放大系数
inline float GetUIResizeRatio()
{
    
    // 获取当前系统DPI, 当前系统DPI除以设计时DPI值, 则得到UI放大系数
    const float DESIGN_DPI = 96.0f; // 设计时DPI
    HDC hdc = GetDC(NULL);
    float ratioX = GetDeviceCaps(hdc, LOGPIXELSX)/DESIGN_DPI;
    float ratioY = GetDeviceCaps(hdc, LOGPIXELSY)/DESIGN_DPI;
    float ratio = ratioX > ratioY ? ratioX : ratioY;

    ReleaseDC(NULL, hdc);

    if (ratio < 1.0f)
        ratio = 1.0f;
    return ratio;
}

/// @brief 根据DPI动态调整UI大小宏
#ifndef RESIZE_UI
#define RESIZE_UI \
do\
{\
    float uiRatio = GetUIResizeRatio();\
    int width = this->geometry().width() * uiRatio;\
    int height = this->geometry().height() * uiRatio;\
    this->setFixedSize(width, height);\
} while (0)

#endif

#endif