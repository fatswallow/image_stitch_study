//球面坐标定位法校正鱼眼图像畸变
//作者：田粮                 合肥工业大学
 
 
#include "ScanningMethod.h"
//#include"cv.h"
//#include"cxcore.h"
//#include"highgui.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

 
#include<iostream>
using namespace std;
using namespace cv;
 
//根据校正图像 U坐标 计算鱼眼图像的X坐标
float CalculateFisheye_x( int u, int v, float x0, float y0, float r);
float Calculate_dx( int v, float y0, float r);

int main( int argc, char** argv )
{
 
	IplImage* pImg;
	const char *filename = argv[1];
	if( ( pImg = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR ) ) == 0)
	return 0;
	float x0;
	float y0;
	float r;
	 
	IplImage* imgTemp = ScaningMethod( pImg, &x0, &y0, &r);
	if( imgTemp == 0 )
	{		 
		cout << "标定鱼眼图像中心失败" <<endl;
		return 0;
	}
	IplImage* imgCalib;// = cvCreateImage( cvGetSize( imgTemp ), IPL_DEPTH_8U, 3);

	int key = 0;
	while (key != 27) {
		//cout << "key=" << key << endl;
		switch (key) {
			case 'a': x0 -= 1.0f; break;
			case 'd': x0 += 1.0f; break;
			case 'w': y0 += 1.0f; break;
			case 's': y0 -= 1.0f; break;
			case 'j': r -= 1.0f; break;
			case 'k': r += 1.0f; break;
			default: break;
		}
		cout << "x0=" << x0 << ",y0=" << y0 << ",r=" <<  r << endl;

		imgCalib = cvCreateImage( cvGetSize( imgTemp ), IPL_DEPTH_8U, 3);
		//imgCalib = cvCreateImage( cvSize(x0*2, y0*2), IPL_DEPTH_8U, 3);

		for( int j = 0; j < imgCalib->height; j++)
		{
			for( int i = 0; i < imgCalib->width; i++)
			{
				float x = CalculateFisheye_x( i, j, imgTemp->width/2, imgTemp->height/2, r );
				int ix = (int)x;
				int dx = Calculate_dx( j, imgTemp->height/2, r );
				 
				 
				if( abs(x - (int)x0) > dx+50 )// 有错，可以不要，该段的功能为：
				{
					//当计算得到的x值超出鱼眼图像的有效区域时，
					//x对应的校正后的图像像素点值设为0
					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3] = 0;
					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3 + 1] = 0;
					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3 + 2] = 0;
				}
				else
				{
					//   计算出鱼眼图像的（x，y）坐标后，使用双线性插值法填充校正后图像
					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3] =
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3] * (1-abs(x-ix))+
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3] * (1-abs(x-(ix+1)));

					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3+1] =
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3+1] * (1-abs(x-ix))+
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3+1] * (1-abs(x-(ix+1)));

					((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3+2] =
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3+2] * (1-abs(x-ix))+
					((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3+2] * (1-abs(x-(ix+1)));
				}
			}
		}
		cvNamedWindow("original", 1 );
		cvNamedWindow("circle", 1);
		cvNamedWindow("calibrate", 1 );
		cvShowImage("original", pImg );
		cvShowImage("circle", imgTemp );
		//cvSaveImage("circle.jpg", imgTemp);
		cvShowImage("calibrate", imgCalib);
		string out = "cali_";
		out += filename;
		cvSaveImage(out.c_str(), imgCalib);
		key = cvWaitKey(0);
	}
	 
	cvDestroyWindow( "original" );
	cvDestroyWindow( "circle" );
	cvDestroyWindow( "calibrate" );
	cvReleaseImage( &imgTemp );
	cvReleaseImage( &imgCalib );
	cvReleaseImage( &pImg );
	 
	return 0;
 
 
}

int main1( int argc, char** argv )
{
 
	IplImage* pImg;
	const char *filename = argv[1];
	if( ( pImg = cvLoadImage( filename, CV_LOAD_IMAGE_ANYCOLOR ) ) == 0)
	return 0;
	float x0;
	float y0;
	float r;
	 
	IplImage* imgTemp = ScaningMethod( pImg, &x0, &y0, &r);
	if( imgTemp == 0 )
	{
		 
		cout << "标定鱼眼图像中心失败" <<endl;
		return 0;
	 
	}
	//x0 = 360.f;
	//y0 = 220.f;
	//r = 365.f;
	//cam1_x
	//x0 = 200.f;
	//y0 = 480.f;
	//r = 230.f;

	//x0 = 225.f;
	//y0 = 250.f;
	//r = 300.f;
	//x0 = 200.f;
	//y0 = 480.f;
	r = 300.f;
	cout << x0 << endl << y0 << endl <<  r << endl;
	 
	
	IplImage* imgCalib = cvCreateImage( cvGetSize( imgTemp ), IPL_DEPTH_8U, 3);
	 
	for( int j = 0; j < imgCalib->height; j++)
		for( int i = 0; i < imgCalib->width; i++)
		{
		 
			float x = CalculateFisheye_x( i, j, imgTemp->width/2, imgTemp->height/2, r );
			int ix = (int)x;
			int dx = Calculate_dx( j, imgTemp->height/2, r );
			 
			 
			 if( abs(x - (int)x0) > dx+50 )// 有错，可以不要，该段的功能为：
			 {
			//当计算得到的x值超出鱼眼图像的有效区域时，
			//x对应的校正后的图像像素点值设为0
			 
			((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3] = 0;
			((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3 + 1] = 0;
			((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3 + 2] = 0;
			  
			}
			 else
			 {
				 
				//   计算出鱼眼图像的（x，y）坐标后，使用双线性插值法填充校正后图像
				((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3] =
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3] * (1-abs(x-ix))+
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3] * (1-abs(x-(ix+1)));
				 
				((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3+1] =
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3+1] * (1-abs(x-ix))+
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3+1] * (1-abs(x-(ix+1)));
				 
				((uchar*)(imgCalib->imageData + imgCalib->widthStep*j))[i*3+2] =
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[ix*3+2] * (1-abs(x-ix))+
				((uchar*)(imgTemp->imageData + imgTemp->widthStep*j))[(ix+1)*3+2] * (1-abs(x-(ix+1)));
			  
			}
		 
		}
	 
	cvNamedWindow("original", 1 );
	cvNamedWindow("circle", 1);
	cvNamedWindow("calibrate", 1 );

	cvShowImage("original", pImg );
	cvShowImage("circle", imgTemp );
	//cvSaveImage("circle.jpg", imgTemp);
	cvShowImage("calibrate", imgCalib);
	string out = "cali_";
	out += filename;
	cvSaveImage(out.c_str(), imgCalib);
	 
	cvWaitKey(0);
	 
	cvDestroyWindow( "original" );
	cvDestroyWindow( "circle" );
	cvDestroyWindow( "calibrate" );
	cvReleaseImage( &imgTemp );
	cvReleaseImage( &imgCalib );
	cvReleaseImage( &pImg );
	 
	return 0;
 
 
}
 
 
float CalculateFisheye_x( int u, int v, float x0, float y0, float r)
{
 
float xh = u - x0;
float yi = y0 - v;
float dx = sqrt( r*r - yi*yi );
int xk = ( x0 + xh * dx / r);
return xk;
 
}
 
 
float Calculate_dx( int v, float y0, float r )
{
 
float yi = y0 - v;
int dx = sqrt( r*r - yi*yi);
return dx;
 
}