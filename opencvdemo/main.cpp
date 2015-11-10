
#include <windows.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"

using namespace std;
using namespace cv;

int demo()
{
	//char cwd[1024];
	//getcwd(cwd, sizeof(cwd));
	TCHAR cwd[1024];
	GetCurrentDirectory(sizeof(cwd), cwd);
	wprintf(L"cwd=%s\n", cwd);
	char* imagename = "1.jpg";
	Mat img = imread("1.jpg");
	if (img.empty())
	{
		printf("can not load image %s\n", imagename);
		waitKey();
		return -1;
	}
	imshow("image", img);
	waitKey();
	return 0;
}

int main()
{
	return demo();
}
