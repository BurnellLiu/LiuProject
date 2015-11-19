

#include <opencv2/opencv.hpp>

int main()
{
	const char* szVideoWndName = "Video";
	const char* szForceWndName = "Forceground";
	const char* szBackWndName = "Background";


	IplImage* pImgFrame = NULL; // ֡ͼ
	IplImage* pImgForce = NULL; // ǰ��ͼ
	IplImage* pImgBack = NULL; // ����ͼ

	CvMat* pMatFrame = NULL; // ֡����
	CvMat* pMatForce = NULL; // ǰ������
	CvMat* pMatBack = NULL; // ��������

	CvCapture* pCameraCapture = NULL; // ����ͷ�ɼ���

	// ��������
	cvNamedWindow(szVideoWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(szForceWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(szBackWndName, CV_WINDOW_AUTOSIZE);

	cvMoveWindow(szVideoWndName, 30, 0);
	cvMoveWindow(szForceWndName, 360, 0);
	cvMoveWindow(szBackWndName, 690, 0);

	// ������ͷ
	pCameraCapture = cvCreateCameraCapture(0);
	if (NULL == pCameraCapture)
		goto SAFE_EXIT;

	// ��һ֡��ʼ���ṹ
	pImgFrame = cvQueryFrame(pCameraCapture);
	if (NULL == pImgFrame)
		goto SAFE_EXIT;

	pImgForce = cvCreateImage(cvGetSize(pImgFrame), IPL_DEPTH_8U, 1);
	pImgBack = cvCreateImage(cvGetSize(pImgFrame), IPL_DEPTH_8U, 1);

	pMatFrame = cvCreateMat(pImgFrame->height, pImgFrame->width, CV_32FC1);
	pMatForce = cvCreateMat(pImgFrame->height, pImgFrame->width, CV_32FC1);
	pMatBack = cvCreateMat(pImgFrame->height, pImgFrame->width, CV_32FC1);

	

	while (pImgFrame = cvQueryFrame(pCameraCapture))
	{
		//cvCvtColor(pImgFrame, pImgForce, CV_BGR2GRAY);
		cvShowImage(szVideoWndName, pImgFrame);
		//cvShowImage(szForceWndName, pImgForce);
		//cvShowImage(szBackWndName, 0);

		if (cvWaitKey(33) == 27)
			break;
	}



	
	

SAFE_EXIT:
	cvReleaseImage(&pImgForce);
	cvReleaseImage(&pImgBack);

	cvReleaseMat(&pMatFrame);
	cvReleaseMat(&pMatForce);
	cvReleaseMat(&pMatBack);

	cvReleaseCapture(&pCameraCapture);

	cvDestroyWindow(szVideoWndName);
	cvDestroyWindow(szForceWndName);
	cvDestroyWindow(szBackWndName);

	return 0;
}