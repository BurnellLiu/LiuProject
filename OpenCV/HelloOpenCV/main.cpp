
#include "opencv2/opencv.hpp"

int main()  
{  
    const char* szImageName = "splash.png";  
    const char* szWindowsTitle = "HelloOpenCV";  

    //���ļ��ж�ȡͼ��  
    IplImage *pImage = cvLoadImage(szImageName, CV_LOAD_IMAGE_UNCHANGED);  

    //��������  
    cvNamedWindow(szWindowsTitle, CV_WINDOW_AUTOSIZE);  

    //��ָ����������ʾͼ��  
    cvShowImage(szWindowsTitle, pImage);  

    //�ȴ������¼�  
    cvWaitKey();  

    cvDestroyWindow(szWindowsTitle);  
    cvReleaseImage(&pImage); 

    return 0;  
}  