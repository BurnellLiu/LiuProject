
#include "opencv2/opencv.hpp"

int main()  
{  
    const char* szImageName = "splash.png";  
    const char* szWindowsTitle = "HelloOpenCV";  

    //从文件中读取图像  
    IplImage *pImage = cvLoadImage(szImageName, CV_LOAD_IMAGE_UNCHANGED);  

    //创建窗口  
    cvNamedWindow(szWindowsTitle, CV_WINDOW_AUTOSIZE);  

    //在指定窗口中显示图像  
    cvShowImage(szWindowsTitle, pImage);  

    //等待按键事件  
    cvWaitKey();  

    cvDestroyWindow(szWindowsTitle);  
    cvReleaseImage(&pImage); 

    return 0;  
}  