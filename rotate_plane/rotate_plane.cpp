#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

using namespace std;
using namespace cv;

int rotate(const char *filename, int degree)
{
	IplImage *src = cvLoadImage(filename);
	IplImage *dst;
	CvSize sz;
	double scale = (double)cos((double)(90-degree)/180*3.1415);
	if(src)
	{
		sz.width = src->width;
		sz.height = src->height*scale;
		dst = cvCreateImage(sz,src->depth,src->nChannels);
		cvResize(src,dst,CV_INTER_CUBIC);
		string out = "plane_";
		out += filename;
		cvSaveImage(out.c_str(), dst);
		return 0;
	}
	return -1;
}

int main(int argc, char **argv)
{
	for (int i = 1; i+1 < argc; i+=2)
	{
		rotate(argv[i], atoi(argv[i+1]));
	}
	return 0;
}