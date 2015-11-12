#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int sett1(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, int &x3, int &y3)
{
	x0 = 384;
	y0 = 461;
	x1 = 591;
	y1 = 459;
	x2 = 899;
	y2 = 553;
	x3 = 0;
	y3 = 523;
	return 0;
}
int sett2(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2, int &x3, int &y3)
{
	x0 = 230;
	y0 = 165;
	x1 = 572;
	y1 = 165;
	x2 = 900;
	y2 = 470;
	x3 = 0;
	y3 = 470;
	return 0;
}
int main(int argc, char **argv)
{
	Mat img = imread(argv[1]);
	int x0 = 260;
	int y0 = 466;
	int x1 = 530;
	int y1 = 450;
	int x2 = 900;
	int y2 = 500;
	int x3 = 0;
	int y3 = 500;
	sett2(x0, y0, x1, y1, x2, y2, x3, y3);
	int h = img.rows;
	int w = img.cols;
	int key = 0;
	while (key != 27) {

		switch (key) {
			case 'q': x0 -= 1; break;
			case 'w': x0 += 1; break;
			case 'e': y0 -= 1; break;
			case 'r': y0 += 1; break;
			case 't': x1 -= 1; break;
			case 'y': x1 += 1; break;
			case 'u': y1 -= 1; break;
			case 'i': y1 += 1; break;
			case 'a': x2 -= 1; break;
			case 's': x2 += 1; break;
			case 'd': y2 -= 1; break;
			case 'f': y2 += 1; break;
			case 'g': x3 -= 1; break;
			case 'h': x3 += 1; break;
			case 'j': y3 -= 1; break;
			case 'k': y3 += 1; break;
			default: break;
		}
		cout << "(" << x0 << "," << y0 << "),(" << x1 << "," << y1 << "),(" << x2 << "," << y2 << "),(" << x3 << "," << y3 << ")" << endl;
		vector<Point2f> corners(4);
		vector<Point2f> corners_trans(4);
		corners[0] = Point2f(x0,y0);
		corners[1] = Point2f(x1,y1);
		corners[2] = Point2f(w+(w-x1)*(h-y2)/(y2-y1),h);
		corners[3] = Point2f(-(x0*(h-y3)/(y3-y0)),h);
		corners_trans[0] = Point2f(0,0);
		corners_trans[1] = Point2f(w,0);
		corners_trans[2] = Point2f(w,h);
		corners_trans[3] = Point2f(0,h);
		Mat transform = getPerspectiveTransform(corners, corners_trans);
		Mat img_trans = img.clone();
		warpPerspective(img, img_trans, transform, img.size());

		namedWindow("ori",1);
		namedWindow("rotate",1);
		imshow("ori", img);
		imshow("rotate", img_trans);
		imwrite("t.jpg", img_trans);
		key = cvWaitKey(0);
	}
	return 0;
}
