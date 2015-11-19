

#include <opencv2/opencv.hpp>

int main()
{
	const char* szVideoWndName = "Video";
	const char* szForceWndName = "Forceground";
	const char* szBackWndName = "Background";


	IplImage* pImgFrame = NULL; // 帧图
	IplImage* pImgForce = NULL; // 前景图
	IplImage* pImgBack = NULL; // 背景图

	CvMat* pMatFrame = NULL; // 帧矩阵
	CvMat* pMatForce = NULL; // 前景矩阵
	CvMat* pMatBack = NULL; // 背景矩阵

	CvCapture* pCameraCapture = NULL; // 摄像头采集器

	// 创建窗口
	cvNamedWindow(szVideoWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(szForceWndName, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(szBackWndName, CV_WINDOW_AUTOSIZE);

	cvMoveWindow(szVideoWndName, 30, 0);
	cvMoveWindow(szForceWndName, 360, 0);
	cvMoveWindow(szBackWndName, 690, 0);

	// 打开摄像头
	pCameraCapture = cvCreateCameraCapture(0);
	if (NULL == pCameraCapture)
		goto SAFE_EXIT;

	// 第一帧初始化结构
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