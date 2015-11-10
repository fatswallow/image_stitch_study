#include <iostream> 			   // for standard I/O
#include <string>   			   // for strings
#include <iomanip>  			   // for controlling float print precision
#include <sstream>                         // string to number conversion
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include "string.h"
#include "opencv2/core/core.hpp"           // Basic OpenCV structures (cv::Mat, Scalar)
#include "opencv2/imgproc/imgproc.hpp"     // Gaussian Blur
#include "opencv2/highgui/highgui.hpp"     // OpenCV window I/O
#include "FishEyeCorrect.h"

using namespace std;
using namespace cv;

/*提取一个字符串中的数字(从txt中提取数字参数)*/		        
void str2num(string& str,vector<int>& rint);    
/*字符串中提取数字*/
int substr2num(string& str);

CFishEyeCorrect fishCorrect;

int main(int argc, char **argv)
{
	char  buf[100];										
	ifstream in("param.txt");
	int totalnum = 0;

	CFishEyeCorrect bak;

	while (in.getline(buf,100))
	{
		string str = buf;
		vector<int>   myint;
		/*提取一行字符串中的数字*/
		str2num(str,myint);	
		/*第一行，获取宽和高*/							
		if(0==totalnum)
		{
			fishCorrect.dstWidth = myint[0]/2;
			fishCorrect.dstHeight = myint[1]/2;
			fishCorrect.wRatio = myint[2];
			fishCorrect.hRatio = myint[3];
		}
		/*提取第二行，获取第一个展开图的参数：方位角，仰角，视角*/
		else if(1==totalnum)
		{
			fishCorrect.phi0 = myint[0];
			fishCorrect.sita0 = myint[1];
			fishCorrect.gama0 = myint[2];
		}
		/*提取第三行，获取第二个展开图的参数：方位角，仰角，视角*/
		else if(2==totalnum)
		{
			fishCorrect.phi1 = myint[0];
			fishCorrect.sita1 = myint[1];
			fishCorrect.gama1 = myint[2];
		}
		/*提取第四行，获取第三个展开图的参数：方位角，仰角，视角*/
		else if(3==totalnum)
		{
			fishCorrect.phi2 = myint[0];
			fishCorrect.sita2 = myint[1];
			fishCorrect.gama2 = myint[2];
		}
		/*提取第五行，获取第四个展开图的参数：方位角，仰角，视角*/
		else if(4==totalnum)
		{
			fishCorrect.phi3 = myint[0];
			fishCorrect.sita3 = myint[1];
			fishCorrect.gama3 = myint[2];
		}	
		totalnum++;
	}
	memcpy(&bak, &fishCorrect, sizeof(CFishEyeCorrect));
	printf("totalnum=%d.\n", totalnum);

	namedWindow("Video0",1);
	namedWindow("Video1",1);

	int key = 0;
	while (key != 27) {

		memcpy(&fishCorrect, &bak, sizeof(CFishEyeCorrect));

		switch (key) {
			case 'a': fishCorrect.wRatio -= .1f; break;
			case 'd': fishCorrect.wRatio += .1f; break;
			case 'w': fishCorrect.hRatio += .1f; break;
			case 's': fishCorrect.hRatio -= .1f; break;
			case '1': fishCorrect.phi0 -= 1.0f; break;
			case '3': fishCorrect.phi0 += 1.0f; break;
			case '4': fishCorrect.sita0 -= 1.0f; break;
			case '6': fishCorrect.sita0 += 1.0f; break;
			case '7': fishCorrect.gama0 -= 1.0f; break;
			case '9': fishCorrect.gama0 += 1.0f; break;
			default: break;
		}

		memcpy(&bak, &fishCorrect, sizeof(CFishEyeCorrect));

		cout << "wRatio=" << fishCorrect.wRatio << ",hRatio=" << fishCorrect.hRatio << ",phi=" << fishCorrect.phi0 << ",sita=" << fishCorrect.sita0 << ",gama=" << fishCorrect.gama0 << endl;

		Mat img = imread(argv[1]);
		fishCorrect.dstImg = Mat::zeros(Size(fishCorrect.dstWidth*2,fishCorrect.dstHeight*2),CV_8UC3);
		fishCorrect.position =  (int*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*8*sizeof(int));
		fishCorrect.positionShift = (long*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*4*sizeof(long));

		fishCorrect.GetArea(img);
		fishCorrect.orgImg  = img(fishCorrect.correctArea);
		fishCorrect.ParamFix();
		fishCorrect.CalculateParam(fishCorrect.orgImg.cols,fishCorrect.orgImg.rows);
		fishCorrect.CalcPositon(fishCorrect.orgImg);
		fishCorrect.FixPosition(fishCorrect.orgImg);
		fishCorrect.NewImageCorrect(fishCorrect.orgImg,fishCorrect.dstImg);

		imshow("Video0",fishCorrect.orgImg);
		imshow("Video1",fishCorrect.dstImg);

		free(fishCorrect.position);
		free(fishCorrect.positionShift);
		key = waitKey(0);
	}

	return 0;
}

int main1(int argc, char **argv)
{
	char  buf[100];										
	ifstream in("param.txt");
	int totalnum = 0;

	while (in.getline(buf,100))
	{
		string str = buf;
		vector<int>   myint;
		/*提取一行字符串中的数字*/
		str2num(str,myint);	
		/*第一行，获取宽和高*/							
		if(0==totalnum)
		{
			fishCorrect.dstWidth = myint[0]/2;
			fishCorrect.dstHeight = myint[1]/2;
			fishCorrect.wRatio = myint[2];
			fishCorrect.hRatio = myint[3];
		}
		/*提取第二行，获取第一个展开图的参数：方位角，仰角，视角*/
		else if(1==totalnum)
		{
			fishCorrect.phi0 = myint[0];
			fishCorrect.sita0 = myint[1];
			fishCorrect.gama0 = myint[2];
		}
		/*提取第三行，获取第二个展开图的参数：方位角，仰角，视角*/
		else if(2==totalnum)
		{
			fishCorrect.phi1 = myint[0];
			fishCorrect.sita1 = myint[1];
			fishCorrect.gama1 = myint[2];
		}
		/*提取第四行，获取第三个展开图的参数：方位角，仰角，视角*/
		else if(3==totalnum)
		{
			fishCorrect.phi2 = myint[0];
			fishCorrect.sita2 = myint[1];
			fishCorrect.gama2 = myint[2];
		}
		/*提取第五行，获取第四个展开图的参数：方位角，仰角，视角*/
		else if(4==totalnum)
		{
			fishCorrect.phi3 = myint[0];
			fishCorrect.sita3 = myint[1];
			fishCorrect.gama3 = myint[2];
		}	
		totalnum++;
	}
	printf("totalnum=%d.\n", totalnum);

	Mat img = imread(argv[1]);
	fishCorrect.dstImg = Mat::zeros(Size(fishCorrect.dstWidth*2,fishCorrect.dstHeight*2),CV_8UC3);
	fishCorrect.position =  (int*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*8*sizeof(int));
	fishCorrect.positionShift = (long*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*4*sizeof(long));

	fishCorrect.GetArea(img);
	fishCorrect.orgImg  = img(fishCorrect.correctArea);
	fishCorrect.ParamFix();
	fishCorrect.CalculateParam(fishCorrect.orgImg.cols,fishCorrect.orgImg.rows);
	fishCorrect.CalcPositon(fishCorrect.orgImg);
	fishCorrect.FixPosition(fishCorrect.orgImg);
	fishCorrect.NewImageCorrect(fishCorrect.orgImg,fishCorrect.dstImg);

	namedWindow("Video0",1);
	namedWindow("Video1",1);
	imshow("Video0",fishCorrect.orgImg);
	imshow("Video1",fishCorrect.dstImg);

	waitKey();

	free(fishCorrect.position);
	free(fishCorrect.positionShift);

	return 0;
}

/**********************************************
*功能：提取字符串中的数字
*输入参数：一个字符串
*输出参数：数字向量
*返回值：空
***********************************************/
void str2num(string& str,vector<int>& rint)
{
	/*通过字符'/'进行拆分为不同的字串，然后调用substr2num函数获取字串中的数字*/
	string strSep = "/";
	int size_pos = 0;
	string strresult;
	int size_prev_pos = 0;
	int itemp = 0;

	while((size_pos=str.find_first_of(strSep,size_pos))!=string::npos)
	{
		strresult = str.substr(size_prev_pos,size_pos - size_prev_pos);
		/*对字串进一步处理，获得数字*/
		itemp = substr2num(strresult);
		rint.push_back(itemp);
		size_prev_pos=++size_pos;
	}
	if (size_prev_pos!=str.size())
	{
		strresult = str.substr(size_prev_pos,size_pos - size_prev_pos);
		itemp = substr2num(strresult);
		rint.push_back(itemp);
	}
}
/**********************************************
*功能：提取字串中的数字
*输入参数：一个字符串
*输出参数：空
*返回值：一个整数
***********************************************/
int substr2num(string& str)
{
	/*依据等号提取数字*/
	int strlength = str.size();
	stringstream os;
	string strSep = "=";
	int size_pos=0;
	string tem;
	int result = 0;
	size_pos = str.find_first_of(strSep,size_pos);
	tem = str.substr(size_pos+1,strlength - size_pos);
	os<<tem;
	os>>result;
	return result;
}