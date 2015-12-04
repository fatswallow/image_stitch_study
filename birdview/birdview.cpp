#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>

using namespace std;
using namespace cv ;

#define min(x,y) (x>y?y:x)

int main(int argc, char* argv[])
{
	Mat image = imread(argv[1]);
	int board_w = atoi(argv[2]);
	int board_h = atoi(argv[3]);
	int trans_x = atoi(argv[4]);
	int trans_y = atoi(argv[5]);
	int trans_w = atoi(argv[6]);
	int trans_h = atoi(argv[7]);
	int drawcorner = atoi(argv[8]);
	//int w = image.cols;
	//int h = image.rows;


	cv::Size board_sz(board_w, board_h);
	std::vector<cv::Point2f> board_corners;
	int found = cv::findChessboardCorners(image, board_sz, board_corners);
	cout << "found=" << found << endl;

	if (found) {

		if (drawcorner != 0)
			cv::drawChessboardCorners(image, board_sz, board_corners, found);

		vector<Point2f> corners_image(4);
		vector<Point2f> corners_trans(4);
		corners_image[0] = board_corners.at(0);
		corners_image[1] = board_corners.at(board_w-1);
		corners_image[2] = board_corners.at(board_h * board_w - 1);
		corners_image[3] = board_corners.at((board_h-1) * board_w);

		/* 1/3 image
		int b = ((w > h) ? h : w)/3;
		corners_trans[0] = Point2f((w-b)/2,(h-b)/2);
		corners_trans[1] = Point2f((w-b)/2+b,(h-b)/2);
		corners_trans[2] = Point2f((w-b)/2+b,(h-b)/2+b);
		corners_trans[3] = Point2f((w-b)/2,(h-b)/2+b);
		*/
		/* ave wxh
		int center_x = (corners_image[0].x + corners_image[1].x + corners_image[2].x + corners_image[3].x) / 4;
		int center_y = (corners_image[0].y + corners_image[1].y + corners_image[2].y + corners_image[3].y) / 4;
		int ave_w = ((corners_image[1].x - corners_image[0].x) + (corners_image[2].x - corners_image[3].x)) / 2;
		int ave_h = ((corners_image[3].y - corners_image[0].y) + (corners_image[2].y - corners_image[1].y)) / 2;
		int r = (ave_w > ave_h) ? ave_h : ave_w;
		corners_trans[0] = Point2f(center_x-r,center_y-r);
		corners_trans[1] = Point2f(center_x+r,center_y-r);
		corners_trans[2] = Point2f(center_x+r,center_y+r);
		corners_trans[3] = Point2f(center_x-r,center_y+r);
		*/
		float center_x = (corners_image[0].x + corners_image[1].x + corners_image[2].x + corners_image[3].x) / 4;
		float center_y = (corners_image[0].y + corners_image[1].y + corners_image[2].y + corners_image[3].y) / 4;
		float min_w = min((corners_image[1].x - corners_image[0].x), (corners_image[2].x - corners_image[3].x));
		float min_h = min((corners_image[3].y - corners_image[0].y), (corners_image[2].y - corners_image[1].y));
		float r = min(min_w, min_h)/2;
		cout << corners_image << endl << "x=" << center_x << ",y=" << center_y << ",r=" << r << endl;
		float w = r;
		float h = r;
		if (trans_x != 0 && trans_y != 0) {
			center_x = trans_x;
			center_y = trans_y;
		}
		if (trans_w != 0 && trans_h != 0) {
			w = trans_w;
			h = trans_h;
		}
		cout << "x=" << center_x << ",y=" << center_y << ",w=" << w << ",h=" << h << endl;
		corners_trans[0] = Point2f(center_x-w,center_y-h);
		corners_trans[1] = Point2f(center_x+w,center_y-h);
		corners_trans[2] = Point2f(center_x+w,center_y+h);
		corners_trans[3] = Point2f(center_x-w,center_y+h);

		Mat transform = getPerspectiveTransform(corners_image, corners_trans);
		Mat img_trans = image.clone();
		warpPerspective(image, img_trans, transform, image.size());

		namedWindow("cali",1);
		imshow("cali", img_trans);
		string f = "bird_";
		f.append(argv[1]);
		imwrite(f, img_trans);
	}
	namedWindow("ori",1);
	imshow("ori", image);
	waitKey(0);

	return 0 ;
}