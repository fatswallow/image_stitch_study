#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat ori = imread(argv[1]);

	float rotx = 0.0f;
	float roty = 0.0f;
	float rotz = 0.0f;
	int f = 2; // this is also configurable, f=2 should be about 50mm focal length

	int key = 0;
	while (key != 27) {

		switch (key) {
			case 'a': f -= 1; break;
			case 'd': f += 1; break;
			case 'u': rotx -= .1f; break;
			case 'i': rotx += .1f; break;
			case 'o': roty -= .1f; break;
			case 'p': roty += .1f; break;
			case 'k': rotz -= .1f; break;
			case 'l': rotz += .1f; break;
			default: break;
		}

		cout << "x,y,z=" << rotx << "," << roty << "," << rotz << ",f=" << f << endl;

		Mat img = ori.clone();

		int h = img.rows;
		int w = img.cols;

		float cx = cosf(rotx), sx = sinf(rotx);
		float cy = cosf(roty), sy = sinf(roty);
		float cz = cosf(rotz), sz = sinf(rotz);

		float roto[3][2] = { // last column not needed, our vector has z=0
			{ cz * cy, cz * sy * sx - sz * cx },
			{ sz * cy, sz * sy * sx + cz * cx },
			{ -sy, cy * sx }
		};

		float pt[4][2] = {{ -w / 2, -h / 2 }, { w / 2, -h / 2 }, { w / 2, h / 2 }, { -w / 2, h / 2 }};
		float ptt[4][2];
		for (int i = 0; i < 4; i++) {
			float pz = pt[i][0] * roto[2][0] + pt[i][1] * roto[2][1];
			ptt[i][0] = w / 2 + (pt[i][0] * roto[0][0] + pt[i][1] * roto[0][1]) * f * h / (f * h + pz);
			ptt[i][1] = h / 2 + (pt[i][0] * roto[1][0] + pt[i][1] * roto[1][1]) * f * h / (f * h + pz);
		}

		cv::Mat in_pt = (cv::Mat_<float>(4, 2) << 0, 0, w, 0, w, h, 0, h);
		cv::Mat out_pt = (cv::Mat_<float>(4, 2) << ptt[0][0], ptt[0][1],
			ptt[1][0], ptt[1][1], ptt[2][0], ptt[2][1], ptt[3][0], ptt[3][1]);

		cv::Mat transform = cv::getPerspectiveTransform(in_pt, out_pt);

		cv::Mat img_in = img.clone();
		cv::warpPerspective(img_in, img, transform, img_in.size());

		namedWindow("ori",1);
		namedWindow("rotate",1);
		imshow("ori", ori);
		imshow("rotate", img);
		key = cv::waitKey(0);
	}
	return 0;
}
