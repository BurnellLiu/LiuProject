
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <vector>
using std::vector;

#include "LWindowGDI.h"

#include "Mandelbrot.h"


struct CenterPoint 
{
    double X;
    double Y;
};

class LGameWindow : public LWindow
{
public:
	LGameWindow();
	virtual ~LGameWindow();

	virtual void Exe();

private:
	void InitGame();
	void PaintGame();
	void RunGame();
	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

private:
	LBackBufferDC m_backDC; // 背景缓冲区

    AccDevice m_defaultAcc; // 默认加速器设备

    MandelbrotParam<float> m_paramFloat; // 曼德勃罗特参数(单精度版)
    MandelbrotParam<double> m_paramDouble; // 曼德勃罗特参数(双精度版)
    MandelbrotImage m_image; // 曼德勃罗特图像参数

    double m_dis; // 曼德勃罗特计算空间, 左右上下距离中心点的距离
    double m_scale; // 曼德勃罗特计算空间缩放系数

    unsigned int m_cPointIndex; // 中心点索引
    vector<CenterPoint> m_cPointList; // 中心点列表

    unsigned int m_frameCount; // 画面帧计数
    unsigned int m_fps; // 每秒帧速度
    double m_timeM; // 耗时, 单位分钟
};

#endif