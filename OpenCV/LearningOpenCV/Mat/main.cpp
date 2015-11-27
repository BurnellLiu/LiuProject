
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;




int main()
{
	Mat mat(3, 2, CV_8UC3, Scalar(0, 0, 255));
	cout << "M=" << endl << mat << endl;

	Mat zero = Mat::zeros(2, 3, CV_8UC1);
	cout << "Zero=" << endl << zero << endl;

	Mat one = Mat::ones(2, 3, CV_32F);
	cout << "One=" << endl << one << endl;

	Mat eye = Mat::eye(3, 3, CV_64F);
	cout << "Eye=" << endl << eye << endl;

	Vec3b color;
	color[0] = 255;
	color[1] = 0;
	color[2] = 0;


	Mat grayImg(600, 800, CV_8UC1);
	Mat colorImg(600, 800, CV_8UC3);

	imshow("Gray", grayImg);

	waitKey(0);

	return 0;
}