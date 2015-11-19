

#include <opencv2/opencv.hpp>


int main()
{
	IplImage* pImgSrc = NULL;
	IplImage* pImgRotate = NULL;

	float rotateMatData[6] = { 0 }; // ��ת��������
	CvMat rotateMat = cvMat(2, 3, CV_32F, rotateMatData); // ��ת����

	// ����ԭʼͼ
	pImgSrc = cvLoadImage("Lena.jpg", CV_LOAD_IMAGE_UNCHANGED);
	if (NULL == pImgSrc)
		goto SAFE_EXIT;

	// ����ԭʼͼ
	pImgRotate = cvCloneImage(pImgSrc);

	// ��������
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

	// ������ת����
	rotateMatData[2] = imgWidth * 0.5f;
	rotateMatData[5] = imgHeight * 0.5f;
	

	int angle = 0; // ��ת�Ƕ�
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