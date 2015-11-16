
#include "opencv2\\opencv.hpp"


IplImage* g_pImgSrc = NULL; // ԭͼ
IplImage* g_pImgGray = NULL; // �Ҷ�ͼ

int main()
{
    g_pImgSrc = cvLoadImage("Fruits.jpg", CV_LOAD_IMAGE_COLOR);
    if (g_pImgSrc == NULL)
    {
        goto SAFE_EXIT;
    }

    g_pImgGray = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 1);
    cvCvtColor(g_pImgSrc, g_pImgGray, CV_BGR2GRAY);

    cvNamedWindow("Gray", CV_WINDOW_AUTOSIZE);
    cvShowImage("Gray", g_pImgGray);

    cvWaitKey(0);

SAFE_EXIT:
    cvReleaseImage(&g_pImgGray);
    cvReleaseImage(&g_pImgSrc);
    return 0;
}