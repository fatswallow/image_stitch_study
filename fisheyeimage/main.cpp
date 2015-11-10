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

	int x = 342;
	int y = 257;
	int r = 332;

	namedWindow("ori",1);
	imshow("ori", image);
	namedWindow("img0",1);
	namedWindow("img1",1);
	namedWindow("img2",1);
	namedWindow("img3",1);
	namedWindow("img4",1);


	int key = 0;
	while (key != 27) {

		switch (key) {
			case 'a': break;
			case 'd': break;
			case 'w': break;
			case 's': break;
			case '1': break;
			case '3': break;
			case '4': break;
			case '6': break;
			case '7': break;
			case '9': break;
			default: break;
		}

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

		corrector adjuster;
		Mat img0 = adjuster.getCorrectImage(params, corrector::correctMethod::LONG_LAT_MAP_REVERSE_FORWARD);
		imshow("img0", img0);
		Mat img1 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL);
		imshow("img1", img1);
		Mat img2 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_HALF_PI);
		imshow("img2", img2);
		Mat img3 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_REVERSE_W_VARIABLE);
		imshow("img3", img3);
		Mat img4 = adjuster.getCorrectImage(params, corrector::correctMethod::PERSPECTIVE_LONG_LAT_MAP_CAMERA_LEN_MODEL_FORWORD_W_VARIABLE);
		imshow("img4", img4);

		key = cvWaitKey(0);
	}
	return 0;
}