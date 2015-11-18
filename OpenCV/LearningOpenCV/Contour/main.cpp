
#include "opencv2\\opencv.hpp"


int main()
{
	IplImage* pImgSrc = NULL; // 原始图
	IplImage* pImgContour = NULL; // 轮廓图

	CvMemStorage* pMemStorage = cvCreateMemStorage(0); // 临时存储区
	CvSeq* pContour = 0; // 存储轮廓点

	// 以灰度图的方式加载图片
	pImgSrc = cvLoadImage("Fruits.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (pImgSrc == NULL)
		goto SAFE_EXIT;

	// 显示原始图
	cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
	cvShowImage("Source", pImgSrc);

	// 为轮廓图申请空间, 3通道图像
	pImgContour = cvCreateImage(cvGetSize(pImgSrc), IPL_DEPTH_8U, 3);

	// 将单通道灰度图转化为3通道灰度图
	cvCvtColor(pImgSrc, pImgContour, CV_GRAY2BGR);

	// 查找轮廓
	cvFindContours(pImgSrc, pMemStorage, &pContour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// 将轮廓画出
	cvDrawContours(pImgContour, pContour, CV_RGB(0, 0, 255), CV_RGB(255, 0, 0), 2, 2, 8, cvPoint(0, 0));

	// 显示轮廓图
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