
#include "opencv2/opencv.hpp"

int main()
{
    const char* szImgName = "Lena.jpg";

    IplImage* pImgSrc = NULL;
    pImgSrc = cvLoadImage(szImgName, CV_LOAD_IMAGE_UNCHANGED); // 加载图片, 加载方式为原图加载
    IplImage* pImgDest = NULL;
    CvSize imgSize = cvGetSize(pImgSrc);
    pImgDest = cvCreateImage(imgSize, pImgSrc->depth, pImgSrc->nChannels); // 创建图片指定大小, 深度和通道数
    cvCopy(pImgSrc, pImgDest, NULL); // 复制图片
    cvSaveImage("Lena2.jpg", pImgDest); // 把图片写入文件

    cvNamedWindow(szImgName, CV_WINDOW_AUTOSIZE); // 创建一个窗口, 窗口大小为自动

    cvShowImage(szImgName, pImgDest); // 在窗口上显示图片

    cvWaitKey(0); // 等待按键, 参数为等待的时间(毫秒), 0表示一直等待, 等到按键就返回按键值

    cvDestroyWindow(szImgName); // 销毁窗口
    cvReleaseImage(&pImgSrc); // 释放图片资源
    cvReleaseImage(&pImgDest);

    return 0;
}