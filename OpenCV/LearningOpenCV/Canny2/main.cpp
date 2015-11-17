
#include "opencv2\\opencv.hpp"


IplImage* g_pImgSrc = NULL; // ԭͼ
IplImage* g_pImgGray = NULL; // �Ҷ�ͼ
IplImage* g_pImgGrayEdge = NULL; // �Ҷ�ͼ�ı�Եͼ
IplImage* g_pImgSrcEdge = NULL; // ԭʼͼ�ı�Եͼ

// ������Ļص�����
void OnTrachbar(int h)
{
    // ��Ե���, �ú���ֻ���ܵ�ͨ��ͼ��(�Ҷ�ͼ)
    // �����ı�Եͼ�е�����ֻ��0x00��0xff
    // 0x00Ϊ��ɫ, 0xffΪ��ɫ, ��ɫ�ĵ�Ϊ��Ե�� 
    cvCanny(g_pImgGray, g_pImgGrayEdge, (float)h, (float)h * 3, 3); 
    
    cvZero(g_pImgSrcEdge);

    // ����ʱֻ��������ͼƬ��ֵΪ0xff�ĵ�
    cvCopy(g_pImgSrc, g_pImgSrcEdge, g_pImgGrayEdge);

    cvShowImage("Canny", g_pImgSrcEdge);
    cvShowImage("Test", g_pImgGrayEdge);
}

int main()
{
    // ����ԭʼͼƬ
    g_pImgSrc = cvLoadImage("Fruits.jpg", CV_LOAD_IMAGE_COLOR);
    if (g_pImgSrc == NULL)
    {
        goto SAFE_EXIT;
    }

    // ����ͼƬ
    g_pImgGray = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 1);
    g_pImgGrayEdge = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 1);
    g_pImgSrcEdge = cvCreateImage(cvGetSize(g_pImgSrc), IPL_DEPTH_8U, 3);

    // ת��Ϊ�Ҷ�ͼ
    cvCvtColor(g_pImgSrc, g_pImgGray, CV_BGR2GRAY); 

    // �������ںͻ�����
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