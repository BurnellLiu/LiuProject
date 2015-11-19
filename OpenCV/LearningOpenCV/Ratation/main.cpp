

#include <opencv2/opencv.hpp>


int main()
{
	IplImage* pImgSrc = NULL;
	IplImage* pImgRotate = NULL;

	float rotateMatData[6] = { 0 }; // 旋转矩阵数据
	CvMat rotateMat = cvMat(2, 3, CV_32F, rotateMatData); // 旋转矩阵

	// 加载原始图
	pImgSrc = cvLoadImage("Lena.jpg", CV_LOAD_IMAGE_UNCHANGED);
	if (NULL == pImgSrc)
		goto SAFE_EXIT;

	// 复制原始图
	pImgRotate = cvCloneImage(pImgSrc);

	// 创建窗口
	cvNamedWindow("Source");
	cvShowImage("Source", pImgSrc);
	cvNamedWindow("Rotate");


	
	// Matrix m looks like:
	//
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ]       [ A21  A22   b2 ]
	//
	int imgWidth = pImgSrc->width;
	int imgHeight = pImgSrc->height;

	// 设置旋转中心
	rotateMatData[2] = imgWidth * 0.5f;
	rotateMatData[5] = imgHeight * 0.5f;
	

	int angle = 0; // 旋转角度
	while (true)
	{
		rotateMatData[0] = (float)cos(-angle * 2 * CV_PI / 180.0);
		rotateMatData[1] = (float)sin(-angle * 2 * CV_PI / 180.0);
		rotateMatData[3] = -rotateMatData[1];
		rotateMatData[4] = rotateMatData[0];

		cvZero(pImgRotate);

		cvGetQuadrangleSubPix(pImgSrc, pImgRotate, &rotateMat);

		cvShowImage("Rotate", pImgRotate);

		if (cvWaitKey(100) == 27) // Wait ESC
			break;

		angle = (angle + 1) % 360;
	}
	

SAFE_EXIT:
	cvDestroyWindow("Rotate");
	cvDestroyWindow("Source");
	cvReleaseImage(&pImgSrc);
	cvReleaseImage(&pImgRotate);
	return 0;
}