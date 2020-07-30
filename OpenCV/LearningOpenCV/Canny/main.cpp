
#include <opencv2/opencv.hpp>

int main()
{
    IplImage* pImgSrc = NULL;
    IplImage* pImgCanny = NULL;

    pImgSrc = cvLoadImage("Lena.jpg", CV_LOAD_IMAGE_GRAYSCALE); // 以灰度图的方式加载图片

    pImgCanny = cvCreateImage(cvGetSize(pImgSrc), IPL_DEPTH_8U, 1); //创建图片

    // Canny边缘检测
    cvCanny(pImgSrc, pImgCanny, 50, 150, 3);

    cvNamedWindow("Lena", CV_WINDOW_AUTOSIZE);
    cvShowImage("Lena", pImgSrc);

    cvNamedWindow("Canny", CV_WINDOW_AUTOSIZE);
    cvShowImage("Canny", pImgCanny);

    cvWaitKey(0);

    cvDestroyWindow("Canny");
    cvDestroyWindow("Lena");
    cvReleaseImage(&pImgSrc);
    cvReleaseImage(&pImgCanny);
    return 0;
}