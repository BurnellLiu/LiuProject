
#include "opencv2\\opencv.hpp"


IplImage* g_pImgSrc = NULL; // 原图
IplImage* g_pImgGray = NULL; // 灰度图
IplImage* g_pImgGrayEdge = NULL; // 灰度图的边缘图
IplImage* g_pImgSrcEdge = NULL; // 原始图的边缘图

// 滑动块的回调函数
void OnTrachbar(int h)
{
    // 边缘检测, 该函数只接受单通道图像(灰度图)
    // 产生的边缘图中的数据只有0x00和0xff
    // 0x00为黑色, 0xff为白色, 白色的点为边缘点 
    cvCanny(g_pImgGray, g_pImgGrayEdge, (float)h, (float)h * 3, 3); 
    
    cvZero(g_pImgSrcEdge);

    // 拷贝时只拷贝掩码图片中值为0xff的点
    cvCopy(g_pImgSrc, g_pImgSrcEdge, g_pImgGrayEdge);

    cvShowImage("Canny", g_pImgSrcEdge);
    cvShowImage("Test", g_pImgGrayEdge);
}

int main()
{
    // 加载原始图片
    g_pImgSrc = cvLoadImage("Fruits.jpg", CV_LOAD_IMAGE_COLOR);
    if (g_pImgSrc == NULL)
    {
        goto SAFE_EXIT;
    }

    // 创建图片
    g_pImgGray = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 1);
    g_pImgGrayEdge = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 1);
    g_pImgSrcEdge = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 3);

    // 转换为灰度图
    cvCvtColor(g_pImgSrc, g_pImgGray, CV_BGR2GRAY); 

    // 创建窗口和滑动块
    cvNamedWindow("Canny", CV_WINDOW_AUTOSIZE);
    int threshold = 1;
    cvCreateTrackbar("Threshold", "Canny", &threshold, 100, OnTrachbar);
    cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);    
    OnTrachbar(0);

    cvWaitKey(0);

SAFE_EXIT:
    cvReleaseImage(&g_pImgSrcEdge);
    cvReleaseImage(&g_pImgGrayEdge);
    cvReleaseImage(&g_pImgGray);
    cvReleaseImage(&g_pImgSrc);
    cvDestroyWindow("Canny");
    return 0;
}