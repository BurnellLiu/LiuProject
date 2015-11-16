
#include "opencv2/opencv.hpp"

int main()
{
    const char* szImgName = "Lena.jpg";

    IplImage* pImgSrc = NULL;
    pImgSrc = cvLoadImage(szImgName, CV_LOAD_IMAGE_UNCHANGED); // ����ͼƬ, ���ط�ʽΪԭͼ����
    IplImage* pImgDest = NULL;
    CvSize imgSize = cvGetSize(pImgSrc);
    pImgDest = cvCreateImage(imgSize, pImgSrc->depth, pImgSrc->nChannels); // ����ͼƬָ����С, ��Ⱥ�ͨ����
    cvCopy(pImgSrc, pImgDest, NULL); // ����ͼƬ
    cvSaveImage("Lena2.jpg", pImgDest); // ��ͼƬд���ļ�

    cvNamedWindow(szImgName, CV_WINDOW_AUTOSIZE); // ����һ������, ���ڴ�СΪ�Զ�

    cvShowImage(szImgName, pImgDest); // �ڴ�������ʾͼƬ

    cvWaitKey(0); // �ȴ�����, ����Ϊ�ȴ���ʱ��(����), 0��ʾһֱ�ȴ�, �ȵ������ͷ��ذ���ֵ

    cvDestroyWindow(szImgName); // ���ٴ���
    cvReleaseImage(&pImgSrc); // �ͷ�ͼƬ��Դ
    cvReleaseImage(&pImgDest);

    return 0;
}