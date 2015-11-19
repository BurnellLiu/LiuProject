
#include "opencv2\\opencv.hpp"

void DrawImage(IplImage* pImg)
{
	if (pImg == NULL)
		return;

	cvZero(pImg);

	for (int i = 0; i < 6; i++)
	{
		int dx = (i % 2) * 250 - 30;
		int dy = (i / 2) * 150;
		CvScalar white = cvRealScalar(255);
		CvScalar black = cvRealScalar(0);

		if (i == 0)
		{
			for (int j = 0; j <= 10; j++)
			{
				double angle = (j + 5)*CV_PI / 21;
				cvLine(pImg, cvPoint(cvRound(dx + 100 + j * 10 - 80 * cos(angle)),
					cvRound(dy + 100 - 90 * sin(angle))),
					cvPoint(cvRound(dx + 100 + j * 10 - 30 * cos(angle)),
						cvRound(dy + 100 - 30 * sin(angle))), white, 1, 8, 0);
			}
		}

		cvEllipse(pImg, cvPoint(dx + 150, dy + 100), cvSize(100, 70), 0, 0, 360, white, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 115, dy + 70), cvSize(30, 20), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 185, dy + 70), cvSize(30, 20), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 115, dy + 70), cvSize(15, 15), 0, 0, 360, white, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 185, dy + 70), cvSize(15, 15), 0, 0, 360, white, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 115, dy + 70), cvSize(5, 5), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 185, dy + 70), cvSize(5, 5), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 150, dy + 100), cvSize(10, 5), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 150, dy + 150), cvSize(40, 10), 0, 0, 360, black, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 27, dy + 100), cvSize(20, 35), 0, 0, 360, white, -1, 8, 0);
		cvEllipse(pImg, cvPoint(dx + 273, dy + 100), cvSize(20, 35), 0, 0, 360, white, -1, 8, 0);
	}

}


int main()
{
	const int imgHeight = 500;
	const int imgWidth = 500;

	IplImage* pImgSrc = cvCreateImage(cvSize(imgWidth, imgHeight), IPL_DEPTH_8U, 1); // 原始图
	IplImage* pImgContour = NULL; // 轮廓图

	CvMemStorage* pMemStorage = cvCreateMemStorage(0); // 临时存储区
	CvSeq* pContour = 0; // 存储轮廓点

	// 绘制原始图片
	DrawImage(pImgSrc);

	// 显示原始图
	cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
	cvShowImage("Source", pImgSrc);

	// 为轮廓图申请空间, 3通道图像
	pImgContour = cvCreateImage(cvGetSize(pImgSrc), IPL_DEPTH_8U, 3);

	// 将单通道灰度图转化为3通道灰度图
	//cvCvtColor(pImgSrc, pImgContour, CV_GRAY2BGR);
	cvZero(pImgContour);

	// 查找轮廓
	cvFindContours(pImgSrc, pMemStorage, &pContour, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// 将轮廓画出
	cvDrawContours(pImgContour, pContour, CV_RGB(0, 0, 255), CV_RGB(255, 0, 0), 2, 2, 8, cvPoint(0, 0));

	// 显示轮廓图
	cvNamedWindow("Contour", CV_WINDOW_AUTOSIZE);
	cvShowImage("Contour", pImgContour);

	cvWaitKey(0);

	cvDestroyWindow("Contour");
	cvDestroyWindow("Source");
	cvReleaseImage(&pImgSrc);
	cvReleaseImage(&pImgContour);
	cvReleaseMemStorage(&pMemStorage);
	return 0;
}