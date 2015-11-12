//包含的头文件
#include "tools.h"
#include "findCircleParameter.h"
#include "corrector.h"
#include "viewer.h"
#include "imagesStitcher.h"


void help()
{

}

//主程度入口点
int main1(int argc, char** argv)
{
	//*
	//读图片到内存中
	if (tools::readImage())
	{
		Mat source_image = tools::GetImage();

		if (findCircleParameter::init(source_image))
		{
			findCircleParameter::findCircle();

			if (IDOK == MessageBox(NULL, L"Would you want to check the FOV of fish-eye lens is valid?", L"Check FOV", IDOK))
			{
				findCircleParameter::checkVarify();
			}
			else
				cout << "You have not choose to check the FOV" << endl;

			if (IDOK == MessageBox(NULL, L"Is this Image a HeaveAndEarth circular fish-eye image?", L"Answer a question", IDOK))
			{
				corrector::dispHeaveAndEarthCorrectImage(source_image);
			}

			if (IDOK == MessageBox(NULL, L"Do you want to correct the fish-eye image using the LCCP method?", L"Correction Select",IDOK))
			{
#pragma region 校正参数设定区
				correctParameters params;
				params.imgOrg = source_image;

				findCircleParameter::getCircleParatemer(params.center, params.radius);

				params.w_longtitude = PI / 2;
				params.w_latitude = PI / 2;
				params.distMap = LATITUDE_LONGTITUDE;
				params.theta_left = 0;
				params.phi_up = 0;
				params.camerFieldAngle = findCircleParameter::FOV;
				params.camProjMode = EQUIDISTANCE;
				params.typeOfCorrect = Reverse;
#pragma endregion

				corrector adjuster;
				adjuster.correctImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_HALF_PI);
				//adjuster.correctImage(params, corrector::correctMethod::LONG_LAT_MAP_REVERSE_FORWARD);
				//adjuster.correctImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_FORWORD_W_VARIABLE);
			}
		}
	}
	//*/

#if 0
#pragma region 图像接接部分
	vector<Mat> images1,images2;
	for (int i = 1;i < argc;i++)
	{
		if (i < argc - 1)
			images1.push_back(imread(argv[i]));
		else
			images2.push_back(imread(argv[i]));
	}
	imagesStitcher composer;
	images2.insert(images2.begin(),composer.stitchImages(images1));	
	composer.showPanorama();
#pragma endregion
#endif
	return 0;
}

int main(int argc, char **argv)
{
	Mat image = imread(argv[1]);

	correctParameters params;

	int x = 450;
	int y = 360;
	int r = 360;

	namedWindow("ori",1);
	imshow("ori", image);
	/*namedWindow("img0",1);
	namedWindow("img1",1);
	namedWindow("img2",1);
	namedWindow("img3",1);
	namedWindow("img4",1);*/

	params.imgOrg = image;
	params.center = Point2i(x, y);
	params.radius = r;
	params.w_longtitude = PI / 2;
	params.w_latitude = PI / 2;
	params.distMap = LATITUDE_LONGTITUDE;
	params.theta_left = 0;
	params.phi_up = 0;
	params.camerFieldAngle = findCircleParameter::FOV;
	params.camProjMode = EQUIDISTANCE;
	params.typeOfCorrect = Reverse;

	camMode c = EQUIDISTANCE;
	distMapMode m = PERSPECTIVE;
	CorrectType t = Reverse;

	int key = 0;
	while (key != 27) {

		switch (key) {
			case 'a': params.w_longtitude -= .1f; break;
			case 'd': params.w_longtitude += .1f; break;
			case 'w': params.w_latitude += .1f; break;
			case 's': params.w_latitude -= .1f; break;
			case 'c': if (c==STEREOGRAPHIC) c=EQUIDISTANCE; else if(c==EQUIDISTANCE) c=EQUISOLID; else if (c==EQUISOLID) c=ORTHOGONAL; else if (c==ORTHOGONAL) c=STEREOGRAPHIC; break;
			case 'm': if (m==PERSPECTIVE) m=LATITUDE_LONGTITUDE; else if (m==LATITUDE_LONGTITUDE) m=PERSPECTIVE; break;
			case 't': if (t==Reverse) t=Forward; else if (t==Forward) t=Reverse; break;
			case 'u': params.theta_left -= .1f; break;
			case 'i': params.theta_left += .1f; break;
			case 'o': params.phi_up -= .1f; break;
			case 'p': params.phi_up += .1f; break;
			case 'k': params.camerFieldAngle -= .1f; break;
			case 'l': params.camerFieldAngle += .1f; break;
			case '1': x -= 1; break;
			case '3': x += 1; break;
			case '4': y -= 1; break;
			case '6': y += 1; break;
			case '7': r -= 1; break;
			case '9': r += 1; break;
			default: break;
		}
		cout << "xyr=" << x << "," << y << "," << r << ",w_l=(" << params.w_longtitude << "," << params.w_latitude << "),cm=" << c << ",mm=" << m << ",t=" << t << ",theta,phi=" << params.theta_left << "," << params.phi_up << ",cangle=" << params.camerFieldAngle << endl;
		params.center = Point2i(x, y);
		params.radius = r;
		params.distMap = m;
		params.camProjMode = c;

		corrector adjuster;
#if 0
		Mat img0 = adjuster.getCorrectImage(params, corrector::correctMethod::LONG_LAT_MAP_REVERSE_FORWARD);
		namedWindow("img0",1);
		imshow("img0", img0);
#endif
#if 1
		Mat img1 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL);
		namedWindow("img1",1);
		imshow("img1", img1);
		imwrite("defish1.jpg", img1);
#endif
#if 0
		Mat img2 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_HALF_PI);
		namedWindow("img2",1);
		imshow("img2", img2);
		//imwrite("defish2.jpg", img2);
#endif
#if 0
		Mat img3 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_VARIABLE);
		namedWindow("img3",1);
		imshow("img3", img3);
		Mat img4 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_FORWORD_W_VARIABLE);
		namedWindow("img4",1);
		imshow("img4", img4);
#endif

		key = cvWaitKey(0);
	}
	return 0;
}

int main_x(int argc, char **argv)
{
	vector<Mat> images1,images2;
	for (int i = 1;i < argc;i++)
	{
		if (i < argc - 1)
			images1.push_back(imread(argv[i]));
		else
			images2.push_back(imread(argv[i]));
	}
	imagesStitcher composer;
	images2.insert(images2.begin(),composer.stitchImages(images1));	
	composer.showPanorama();
	return 0;
}