//利用扫描线逼近算法标定鱼眼图像的中心和半径
//作者：田粮    合肥工业大学
 
//#include"cv.h"
//#include"cxcore.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
 
 
//极限亮度差阈值
#define GRAY_DIFF_THR 40
  
IplImage* ScaningMethod( IplImage* img, float* x0, float* y0, float* r);
void ExtremeRow( IplImage* img, float* maxRow, float* minRow, int row);
void ExtremeCol(IplImage* img, float* maxCol, float* minCol, int col );
float RGB_to_gray( IplImage* img, int x, int y );
IplImage* Standard_circle(IplImage* img);