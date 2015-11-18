
#include "opencv2\\opencv.hpp"


int main()
{
	IplImage* pImgSrc = NULL; // ԭʼͼ
	IplImage* pImgContour = NULL; // ����ͼ

	CvMemStorage* pMemStorage = cvCreateMemStorage(0); // ��ʱ�洢��
	CvSeq* pContour = 0; // �洢������

	// �ԻҶ�ͼ�ķ�ʽ����ͼƬ
	pImgSrc = cvLoadImage("Fruits.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (pImgSrc == NULL)
		goto SAFE_EXIT;

	// ��ʾԭʼͼ
	cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
	cvShowImage("Source", pImgSrc);

	// Ϊ����ͼ����ռ�, 3ͨ��ͼ��
	pImgContour = cvCreateImage(cvGetSize(pImgSrc), IPL_DEPTH_8U, 3);

	// ����ͨ���Ҷ�ͼת��Ϊ3ͨ���Ҷ�ͼ
	cvCvtColor(pImgSrc, pImgContour, CV_GRAY2BGR);

	// ��������
	cvFindContours(pImgSrc, pMemStorage, &pContour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// ����������
	cvDrawContours(pImgContour, pContour, CV_RGB(0, 0, 255), CV_RGB(255, 0, 0), 2, 2, 8, cvPoint(0, 0));

	// ��ʾ����ͼ
	cvNamedWindow("Contour", CV_WINDOW_AUTOSIZE);
	cvShowImage("Contour", pImgContour);

	cvWaitKey(0);

SAFE_EXIT:
	cvDestroyWindow("Contour");
	cvDestroyWindow("Source");
	cvReleaseImage(&pImgSrc);
	cvReleaseImage(&pImgContour);
	cvReleaseMemStorage(&pMemStorage);
	return 0;
}