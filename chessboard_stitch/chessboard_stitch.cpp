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
	Mat image1 = imread(argv[1]);
	Mat image2 = imread(argv[2]);
	int board_w = atoi(argv[3]);
	int board_h = atoi(argv[4]);
	int w = image1.cols;
	int h = image1.rows;

	cv::Size board_sz(board_w, board_h);
	std::vector<cv::Point2f> board_corners1;
	std::vector<cv::Point2f> board_corners2;

	int found1 = cv::findChessboardCorners(image1, board_sz, board_corners1);
	int found2 = cv::findChessboardCorners(image2, board_sz, board_corners2);
	cout << "found=" << found1 << "," << found2 << endl;

	if (found1 && found2) {
		cv::drawChessboardCorners(image1, board_sz, board_corners1, found1);
		cv::drawChessboardCorners(image2, board_sz, board_corners2, found2);

		vector<Point2f> corners_image1(4);
		vector<Point2f> corners_image2(4);
		vector<Point2f> corners_trans(4);
		corners_image1[0] = board_corners1.at(0);
		corners_image1[1] = board_corners1.at(board_w-1);
		corners_image1[2] = board_corners1.at(board_h * board_w - 1);
		corners_image1[3] = board_corners1.at((board_h-1) * board_w);

		corners_image2[0] = board_corners2.at(0);
		corners_image2[1] = board_corners2.at(board_w-1);
		corners_image2[2] = board_corners2.at(board_h * board_w - 1);
		corners_image2[3] = board_corners2.at((board_h-1) * board_w);

		/* 1/3 image
		int b = ((w > h) ? h : w)/3;
		corners_trans[0] = Point2f((w-b)/2,(h-b)/2);
		corners_trans[1] = Point2f((w-b)/2+b,(h-b)/2);
		corners_trans[2] = Point2f((w-b)/2+b,(h-b)/2+b);
		corners_trans[3] = Point2f((w-b)/2,(h-b)/2+b);*/
		
		//* ave wxh
		int center_x = (corners_image1[0].x + corners_image1[1].x + corners_image1[2].x + corners_image1[3].x) / 4;
		int center_y = (corners_image1[0].y + corners_image1[1].y + corners_image1[2].y + corners_image1[3].y) / 4;
		int ave_w = ((corners_image1[1].x - corners_image1[0].x) + (corners_image1[2].x - corners_image1[3].x)) / 2;
		int ave_h = ((corners_image1[3].y - corners_image1[0].y) + (corners_image1[2].y - corners_image1[1].y)) / 2;
		int r = (ave_w > ave_h) ? ave_h : ave_w;
		corners_trans[0] = Point2f(center_x-r,center_y-r);
		corners_trans[1] = Point2f(center_x+r,center_y-r);
		corners_trans[2] = Point2f(center_x+r,center_y+r);
		corners_trans[3] = Point2f(center_x-r,center_y+r);
		
		/* min wxh
		float center_x = (corners_image[0].x + corners_image[1].x + corners_image[2].x + corners_image[3].x) / 4;
		float center_y = (corners_image[0].y + corners_image[1].y + corners_image[2].y + corners_image[3].y) / 4;
		float min_w = min((corners_image[1].x - corners_image[0].x), (corners_image[2].x - corners_image[3].x));
		float min_h = min((corners_image[3].y - corners_image[0].y), (corners_image[2].y - corners_image[1].y));
		float r = min(min_w, min_h)/2;
		cout << corners_image << endl << r << endl;
		corners_trans[0] = Point2f(center_x-r,center_y-r);
		corners_trans[1] = Point2f(center_x+r,center_y-r);
		corners_trans[2] = Point2f(center_x+r,center_y+r);
		corners_trans[3] = Point2f(center_x-r,center_y+r);
		*/

		Mat transform1 = getPerspectiveTransform(corners_image1, corners_trans);
		Mat transform2 = getPerspectiveTransform(corners_image2, corners_trans);
		Mat img_trans1 = image1.clone();
		Mat img_trans2 = image2.clone();
		warpPerspective(image1, img_trans1, transform1, image1.size());
		warpPerspective(image2, img_trans2, transform2, image1.size());

		float alpha1 = 0.4f;
		float alpha2 = 0.4f;
		Mat blending;
		addWeighted(img_trans1, alpha1, img_trans2, alpha2, 0.0, blending);

		namedWindow("cali1",1);
		imshow("cali1", img_trans1);
		namedWindow("cali2",1);
		imshow("cali2", img_trans2);
		namedWindow("blending", 1);
		imshow("blending", blending);
		//imwrite("b.jpg", img_trans);
	}
	//namedWindow("ori",1);
	//imshow("ori", image);
	waitKey(0);

	return 0 ;
}