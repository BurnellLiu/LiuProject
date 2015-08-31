
#ifndef _UIDEFINES_H_
#define _UIDEFINES_H_

#include <Windows.h>


/// @brief ��ȡUI�Ŵ�ϵ��
///  
/// @return �Ŵ�ϵ��
inline float GetUIResizeRatio()
{
    
    // ��ȡ��ǰϵͳDPI, ��ǰϵͳDPI�������ʱDPIֵ, ��õ�UI�Ŵ�ϵ��
    const float DESIGN_DPI = 96.0f; // ���ʱDPI
    HDC hdc = GetDC(NULL);
    float ratioX = GetDeviceCaps(hdc, LOGPIXELSX)/DESIGN_DPI;
    float ratioY = GetDeviceCaps(hdc, LOGPIXELSY)/DESIGN_DPI;
    float ratio = ratioX > ratioY ? ratioX : ratioY;

    ReleaseDC(NULL, hdc);

    if (ratio < 1.0f)
        ratio = 1.0f;
    return ratio;
}

/// @brief ����DPI��̬����UI��С��
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