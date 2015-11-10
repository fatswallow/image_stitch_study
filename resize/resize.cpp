#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int resize_smaller(const char *filename)
{
	IplImage *src = cvLoadImage(filename);
	IplImage *dst;
	CvSize sz;
	double scale = (double)src->height/src->width;
	if(src)
	{
		sz.width = src->width*scale;
		sz.height = src->height*scale;
		dst = cvCreateImage(sz,src->depth,src->nChannels);
		cvResize(src,dst,CV_INTER_CUBIC);

		/*cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
		cvShowImage("dst",dst);
		cvWaitKey(0);

		cvReleaseImage(&src);
		cvReleaseImage(&dst);
		cvDestroyWindow("src");
		cvDestroyWindow("dst");
		*/
		cvZero(src);
		Rect r((src->width-dst->width)/2, (src->height-dst->height)/2, dst->width, dst->height);
		cvSetImageROI(src, r);
		cvAdd(src, dst, src, NULL);
		cvResetImageROI(src);
		string out = "resize_";
		out += filename;
		cvSaveImage(out.c_str(), src);
		/*cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
		cvShowImage("dst",dst);
		cvWaitKey(0);

		cvReleaseImage(&src);
		cvReleaseImage(&dst);
		cvDestroyWindow("src");
		cvDestroyWindow("dst");*/
		return 0;
	}
	return -1;
}

int resize_larger(const char *filename)
{
	IplImage *src = cvLoadImage(filename);
	IplImage *dst;
	CvSize sz;
	double scale = (double)src->width/src->height;
	if(src)
	{
		sz.width = src->width*scale;
		sz.height = src->height*scale;
		dst = cvCreateImage(sz,src->depth,src->nChannels);
		cvResize(src,dst,CV_INTER_CUBIC);

		cvZero(dst);
		Rect r((dst->width-src->width)/2, (dst->height-src->height)/2, src->width, src->height);
		cvSetImageROI(dst, r);
		cvAdd(dst, src, dst, NULL);
		cvResetImageROI(dst);

		string out = "resize_";
		out += filename;
		cvSaveImage(out.c_str(), dst);

		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("dst",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
		cvShowImage("dst",dst);
		cvWaitKey(0);

		cvReleaseImage(&src);
		cvReleaseImage(&dst);
		cvDestroyWindow("src");
		cvDestroyWindow("dst");
		return 0;
	}
	return -1;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s in.jpg\n", argv[0]);
		return -1;
	}
	const char *filename = argv[1];
	//resize_smaller(filename);
	resize_larger(filename);
	return 0;
}