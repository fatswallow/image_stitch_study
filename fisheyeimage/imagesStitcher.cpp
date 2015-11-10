#include "imagesStitcher.h"

imagesStitcher::imagesStitcher()
{
	panorama_winname = "panorama";
}

imagesStitcher::~imagesStitcher()
{
}

Mat imagesStitcher::stitchImages(vector<Mat>& images)
{
	Stitcher s=Stitcher::createDefault(false);
	Stitcher::Status status = s.stitch(images, panorama);
	if (Stitcher::OK != status)
	{
		cout << "Can't stitch images, error code = " << status << endl;
		return Mat();
	}
	return panorama;
}

void imagesStitcher::showPanorama()
{
	if (panorama.empty()) return;
	Size s = panorama.size();
	float ratio = 512.0 / s.height;
	s.width = s.width*ratio;
	s.height = s.height*ratio;
	Mat tmp;
	resize(panorama, tmp, s);
	imshow(panorama_winname, tmp);
	setMouseCallback(panorama_winname, onMouse, this);

	waitKey();
}

void imagesStitcher::onMouse(int event, int x, int y, int, void * args)
{
	imagesStitcher *s = (imagesStitcher*)args;
	static Point pt1, pt2;
	static bool flag = false;
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		pt1.x = x;
		pt1.y = y;
		flag = true;
		break;
	case EVENT_MOUSEMOVE:
		if (flag)
		{

		}
	default:
		break;
	}
}
