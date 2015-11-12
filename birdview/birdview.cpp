#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>

using namespace std;
using namespace cv ;

int main(int argc, char* argv[])
{
	Mat image = imread(argv[1]);
	int board_w = atoi(argv[2]);
	int board_h = atoi(argv[3]);
	int w = image.cols;
	int h = image.rows;

	cv::Size board_sz(board_w, board_h);
	std::vector<cv::Point2f> board_corners;
	int found = cv::findChessboardCorners(image, board_sz, board_corners);
	cout << "found=" << found << endl;

	if (found) {
		cv::drawChessboardCorners(image, board_sz, board_corners, found);

		vector<Point2f> corners_image(4);
		vector<Point2f> corners_trans(4);
		corners_image[0] = board_corners.at(0);
		corners_image[1] = board_corners.at(board_w-1);
		corners_image[2] = board_corners.at(board_h * board_w - 1);
		corners_image[3] = board_corners.at((board_h-1) * board_w);

		int b = ((w > h) ? h : w)/3;
		corners_trans[0] = Point2f((w-b)/2,(h-b)/2);
		corners_trans[1] = Point2f((w-b)/2+b,(h-b)/2);
		corners_trans[2] = Point2f((w-b)/2+b,(h-b)/2+b);
		corners_trans[3] = Point2f((w-b)/2,(h-b)/2+b);
		Mat transform = getPerspectiveTransform(corners_image, corners_trans);
		Mat img_trans = image.clone();
		warpPerspective(image, img_trans, transform, image.size());

		namedWindow("cali",1);
		imshow("cali", img_trans);
	}
	namedWindow("ori",1);
	imshow("ori", image);
	waitKey(0);

	return 0 ;
}