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
	Mat image1 = imread(argv[1]); /* front */
	Mat image2 = imread(argv[2]); /* left */
	Mat image3 = imread(argv[3]); /* back */
	Mat image4 = imread(argv[4]); /* right */

	float w = 24.0f; /* each piece */
	float iw = 1280.0f;
	float ih = 1280.0f;

	vector<Point2f> corners_image1(12);
	vector<Point2f> corners_image2(12);
	vector<Point2f> corners_image3(12);
	vector<Point2f> corners_image4(12);
	vector<Point2f> corners_trans1(12);
	vector<Point2f> corners_trans2(12);
	vector<Point2f> corners_trans3(12);
	vector<Point2f> corners_trans4(12);

	corners_image1[0] = Point2f(364.f,1154.f);
	corners_image1[1] = Point2f(415.f,1155.f);
	corners_image1[2] = Point2f(370.f,1221.f);
	corners_image1[3] = Point2f(415.f,1223.f);
	corners_image1[4] = Point2f(570.f,1164.f);
	corners_image1[5] = Point2f(690.f,1164.f);
	corners_image1[6] = Point2f(570.f,1236.f);
	corners_image1[7] = Point2f(690.f,1236.f);
	corners_image1[8] = Point2f(829.f,1171.f);
	corners_image1[9] = Point2f(878.f,1174.f);
	corners_image1[10] = Point2f(825.f,1239.f);
	corners_image1[11] = Point2f(870.f,1241.f);

	corners_image2[0] = Point2f(1123.f,239.f);
	corners_image2[1] = Point2f(1187.f,240.f);
	corners_image2[2] = Point2f(1108.f,360.f);
	corners_image2[3] = Point2f(1171.f,357.f);
	corners_image2[4] = Point2f(1063.f,594.f);
	corners_image2[5] = Point2f(1196.f,590.f);
	corners_image2[6] = Point2f(1064.f,665.f);
	corners_image2[7] = Point2f(1189.f,665.f);
	corners_image2[8] = Point2f(1085.f,860.f);
	corners_image2[9] = Point2f(1131.f,862.f);
	corners_image2[10] = Point2f(1087.f,920.f);
	corners_image2[11] = Point2f(1129.f,922.f);

	corners_image3[0] = Point2f(385.f,12.f);
	corners_image3[1] = Point2f(428.f,12.f);
	corners_image3[2] = Point2f(371.f,71.f);
	corners_image3[3] = Point2f(419.f,76.f);
	corners_image3[4] = Point2f(583.f,42.f);
	corners_image3[5] = Point2f(701.f,43.f);
	corners_image3[6] = Point2f(584.f,112.f);
	corners_image3[7] = Point2f(703.f,114.f);
	corners_image3[8] = Point2f(832.f,38.f);
	corners_image3[9] = Point2f(882.f,38.f);
	corners_image3[10] = Point2f(836.f,106.f);
	corners_image3[11] = Point2f(884.f,103.f);

	corners_image4[0] = Point2f(148.f,320.f);
	corners_image4[1] = Point2f(197.f,308.f);
	corners_image4[2] = Point2f(150.f,393.f);
	corners_image4[3] = Point2f(199.f,389.f);
	corners_image4[4] = Point2f(118.f,564.f);
	corners_image4[5] = Point2f(237.f,564.f);
	corners_image4[6] = Point2f(120.f,636.f);
	corners_image4[7] = Point2f(237.f,638.f);
	corners_image4[8] = Point2f(156.f,900.f);
	corners_image4[9] = Point2f(200.f,902.f);
	corners_image4[10] = Point2f(152.f,981.f);
	corners_image4[11] = Point2f(197.f,979.f);


	//---------------------
	//|    0    1     2   |
	//|    3          4   |
	//|    5    6     7   |
	//---------------------

#define DX(img, pt) ((corners_image##img[pt*4].x+corners_image##img[pt*4+1].x+corners_image##img[pt*4+2].x+corners_image##img[pt*4+3].x)/4)
#define DY(img, pt) ((corners_image##img[pt*4].y+corners_image##img[pt*4+1].y+corners_image##img[pt*4+2].y+corners_image##img[pt*4+3].y)/4)

	vector<Point2f> chess_center(8);
	chess_center[0].y = (corners_image1[4].y + corners_image1[5].y + corners_image1[6].y + corners_image1[7].y) / 4;
	chess_center[1].y = chess_center[0].y;
	chess_center[2].y = chess_center[0].y;
	chess_center[0].x = (corners_image2[4].x + corners_image2[5].x + corners_image2[6].x + corners_image2[7].x) / 4;
	chess_center[3].x = chess_center[0].x;
	chess_center[5].x = chess_center[0].x;

	float dx10 = (DX(1,1) - DX(1,0));
	float dx21 = (DX(1,2) - DX(1,1));
	corners_trans1[0].x = chess_center[0].x - w;
	corners_trans1[0].y = chess_center[0].y - 1.5 * w;
	corners_trans1[1].x = chess_center[0].x + w;
	corners_trans1[1].y = chess_center[0].y - 1.5 * w;
	corners_trans1[2].x = chess_center[0].x - w;
	corners_trans1[2].y = chess_center[0].y + 1.5 * w;
	corners_trans1[3].x = chess_center[0].x + w;
	corners_trans1[3].y = chess_center[0].y + 1.5 * w;
	corners_trans1[4].x = corners_trans1[0].x + dx10 - 1.5 * w;
	corners_trans1[4].y = corners_trans1[0].y;
	corners_trans1[5].x = corners_trans1[1].x + dx10 + 1.5 * w;
	corners_trans1[5].y = corners_trans1[1].y;
	corners_trans1[6].x = corners_trans1[2].x + dx10 - 1.5 * w;
	corners_trans1[6].y = corners_trans1[2].y;
	corners_trans1[7].x = corners_trans1[3].x + dx10 + 1.5 * w;
	corners_trans1[7].y = corners_trans1[3].y;
	corners_trans1[8].x = corners_trans1[0].x + dx21 + dx10;
	corners_trans1[8].y = corners_trans1[0].y;
	corners_trans1[9].x = corners_trans1[1].x + dx21 + dx10;
	corners_trans1[9].y = corners_trans1[1].y;
	corners_trans1[10].x = corners_trans1[2].x + dx21 + dx10;
	corners_trans1[10].y = corners_trans1[2].y;
	corners_trans1[11].x = corners_trans1[3].x + dx21 + dx10;
	corners_trans1[11].y = corners_trans1[3].y;

	float dy30 = DY(2, 1) - DY(2, 0);
	float dy53 = DY(2, 2) - DY(2, 1);
	corners_trans2[0] = corners_trans1[0];
	corners_trans2[1] = corners_trans1[1];
	corners_trans2[2] = corners_trans1[2];
	corners_trans2[3] = corners_trans1[3];
	corners_trans2[4].x = corners_trans2[0].x - 1.5 * w;
	corners_trans2[4].y = corners_trans2[0].y + dy30;
	corners_trans2[5].x = corners_trans2[1].x + 1.5 * w;
	corners_trans2[5].y = corners_trans2[1].y + dy30;
	corners_trans2[6].x = corners_trans2[2].x - 1.5 * w;
	corners_trans2[6].y = corners_trans2[2].y + dy30;
	corners_trans2[7].x = corners_trans2[3].x + 1.5 * w;
	corners_trans2[7].y = corners_trans2[3].y + dy30;
	corners_trans2[8].x = corners_trans2[0].x;
	corners_trans2[8].y = corners_trans2[0].y + dy53 + dy30;
	corners_trans2[9].x = corners_trans2[1].x;
	corners_trans2[9].y = corners_trans2[1].y + dy53 + dy30;
	corners_trans2[10].x = corners_trans2[2].x;
	corners_trans2[10].y = corners_trans2[2].y + dy53 + dy30;
	corners_trans2[11].x = corners_trans2[3].x;
	corners_trans2[11].y = corners_trans2[3].y + dy53 + dy30;

	float dx65 = (DX(3,1) - DX(3,0));
	float dx76 = (DX(3,2) - DX(3,1));
	corners_trans3[0] = corners_trans2[8];
	corners_trans3[1] = corners_trans2[9];
	corners_trans3[2] = corners_trans2[10];
	corners_trans3[3] = corners_trans2[11];
	corners_trans3[4].x = corners_trans3[0].x + dx65 - 1.5 * w;
	corners_trans3[4].y = corners_trans3[0].y;
	corners_trans3[5].x = corners_trans3[1].x + dx65 + 1.5 * w;
	corners_trans3[5].y = corners_trans3[1].y;
	corners_trans3[6].x = corners_trans3[2].x + dx65 - 1.5 * w;
	corners_trans3[6].y = corners_trans3[2].y;
	corners_trans3[7].x = corners_trans3[3].x + dx65 + 1.5 * w;
	corners_trans3[7].y = corners_trans3[3].y;
	corners_trans3[8].x = corners_trans3[0].x + dx76 + dx65;
	corners_trans3[8].y = corners_trans3[0].y;
	corners_trans3[9].x = corners_trans3[1].x + dx76 + dx65;
	corners_trans3[9].y = corners_trans3[1].y;
	corners_trans3[10].x = corners_trans3[2].x + dx76 + dx65;
	corners_trans3[10].y = corners_trans3[2].y;
	corners_trans3[11].x = corners_trans3[3].x + dx76 + dx65;
	corners_trans3[11].y = corners_trans3[3].y;

	float dy42 = DY(4, 1) - DY(4, 0);
	float dy74 = DY(4, 2) - DY(4, 1);
	corners_trans4[0] = corners_trans1[8];
	corners_trans4[1] = corners_trans1[9];
	corners_trans4[2] = corners_trans1[10];
	corners_trans4[3] = corners_trans1[11];
	corners_trans4[4].x = corners_trans4[0].x - 1.5 * w;
	corners_trans4[4].y = corners_trans4[0].y + dy42;
	corners_trans4[5].x = corners_trans4[1].x + 1.5 * w;
	corners_trans4[5].y = corners_trans4[1].y + dy42;
	corners_trans4[6].x = corners_trans4[2].x - 1.5 * w;
	corners_trans4[6].y = corners_trans4[2].y + dy42;
	corners_trans4[7].x = corners_trans4[3].x + 1.5 * w;
	corners_trans4[7].y = corners_trans4[3].y + dy42;
	corners_trans4[8] = corners_trans3[8];
	corners_trans4[9] = corners_trans3[9];
	corners_trans4[10] = corners_trans3[10];
	corners_trans4[11] = corners_trans3[11];

#define SET4(d,s,start) do {d[0]=s[0+start];d[1]=s[1+start];d[2]=s[2+start];d[3]=s[3+start];} while(0)
#define MINUSX4(d, n) do {d[0].x-=n;d[1].x-=n;d[2].x-=n;d[3].x-=n;} while(0)
#define MINUSY4(d, n) do {d[0].y-=n;d[1].y-=n;d[2].y-=n;d[3].y-=n;} while(0)
	float alpha = 0.5f;
	float alpha2 = 0.8f;
	Mat blending;
	Mat img_trans;
	resize(image1, img_trans, Size(2560, 2560), 0, 0, CV_INTER_LINEAR);
	Mat roi_img;
	Rect r;
	Mat transform;
	vector<Point2f> orig(4);
	vector<Point2f> trans(4);

	/* image1 part2 */
	r.x = corners_image1[3].x + 1.5 * w;
	r.y = 0;
	r.width = (corners_image1[10].x - 1.5 * w) - r.x;
	r.height = ih;
	//roi_img.release();
	image1(r).copyTo(roi_img);
	SET4(orig, corners_image1, 4);
	MINUSX4(orig, r.x);
	SET4(trans, corners_trans1, 4);
	trans[0].x -= 2;
	trans[0].y += 20;
	trans[1].y += 18;
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(img_trans, alpha2, img_trans, 0.0, 0.0, blending);

	/* image2 part2 */
	r.x = 0;
	r.y = corners_image2[2].y + 1.5 * w;
	r.width = iw;
	r.height = (corners_image2[8].y - 1.5 * w) - r.y;
	roi_img.release();
	image2(r).copyTo(roi_img);
	SET4(orig, corners_image2, 4);
	MINUSY4(orig, r.y);
	SET4(trans, corners_trans2, 4);
	trans[0].y -= 25;
	trans[1].y -= 25;
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(img_trans, alpha2, blending, 1.0, 0.0, blending);

	/* image3 part2 */
	r.x = corners_image3[3].x + 1.5 * w;
	r.y = 0;
	r.width = (corners_image3[10].x - 1.5 * w) - r.x;
	r.height = ih;
	roi_img.release();
	image3(r).copyTo(roi_img);
	SET4(orig, corners_image3, 4);
	MINUSX4(orig, r.x);
	SET4(trans, corners_trans3, 4);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(img_trans, alpha2, blending, 1.0, 0.0, blending);

	/* image4 part2 */
	r.x = 0;
	r.y = corners_image4[2].y + 1.5 * w;
	r.width = iw;
	r.height = (corners_image4[8].y - 1.5 * w) - r.y;
	roi_img.release();
	image4(r).copyTo(roi_img);
	SET4(orig, corners_image4, 4);
	MINUSY4(orig, r.y);
	SET4(trans, corners_trans4, 4);
	trans[0].x -= 2;
	trans[0].y -= 6;
	trans[1].x -= 6;
	trans[1].y -= 8;
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(img_trans, alpha2, blending, 1.0, 0.0, blending);

	/* image1 part1 */
	r.x = 0;
	r.y = 0;
	r.width = corners_image1[3].x + 2 * w;
	r.height = ih;
	roi_img.release();
	image1(r).copyTo(roi_img);
	SET4(orig, corners_image1, 0);
	SET4(trans, corners_trans1, 0);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image1 part3 */
	r.x = corners_image1[8].x - 2 * w;
	r.y = 0;
	r.width = iw - r.x;
	r.height = ih;
	roi_img.release();
	image1(r).copyTo(roi_img);
	SET4(orig, corners_image1, 8);
	MINUSX4(orig, r.x);
	SET4(trans, corners_trans1, 8);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	
	/* image2 part1 */
	r.x = 0;
	r.y = 0;
	r.width = iw;
	r.height = corners_image2[2].y + 2 * w;
	roi_img.release();
	image2(r).copyTo(roi_img);
	SET4(orig, corners_image2, 0);
	SET4(trans, corners_trans2, 0);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image2 part3 */
	r.x = 0;
	r.y = corners_image2[8].y - 2 * w;
	r.width = iw;
	r.height = ih-r.y;
	roi_img.release();
	image2(r).copyTo(roi_img);
	SET4(orig, corners_image2, 8);
	MINUSY4(orig, r.y);
	SET4(trans, corners_trans2, 8);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image3 part1 */
	r.x = 0;
	r.y = 0;
	r.width = corners_image3[3].x + 2 * w;
	r.height = ih;
	roi_img.release();
	image3(r).copyTo(roi_img);
	SET4(orig, corners_image3, 0);
	MINUSX4(orig, r.x);
	SET4(trans, corners_trans3, 0);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image3 part3 */
	r.x = corners_image3[8].x - 2 * w;
	r.y = 0;
	r.width = iw - r.x;
	r.height = ih;
	roi_img.release();
	image3(r).copyTo(roi_img);
	SET4(orig, corners_image3, 8);
	MINUSX4(orig, r.x);
	SET4(trans, corners_trans3, 8);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image4 part1 */
	r.x = 0;
	r.y = 0;
	r.width = iw;
	r.height = corners_image4[2].y + 2 * w;
	roi_img.release();
	image4(r).copyTo(roi_img);
	SET4(orig, corners_image4, 0);
	MINUSY4(orig, r.y);
	SET4(trans, corners_trans4, 0);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	/* image4 part3 */
	r.x = 0;
	r.y = corners_image4[8].y - 2 * w;
	r.width = iw;
	r.height = ih-r.y;
	roi_img.release();
	image4(r).copyTo(roi_img);
	SET4(orig, corners_image4, 8);
	MINUSY4(orig, r.y);
	SET4(trans, corners_trans4, 8);
	transform = getPerspectiveTransform(orig, trans);
	warpPerspective(roi_img, img_trans, transform, img_trans.size());
	addWeighted(blending, 1.0, img_trans, alpha, 0.0, blending);

	imshow("blending", blending);
	imwrite("blending.jpg", blending);
	waitKey(0);

	return 0 ;
}