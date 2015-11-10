#include "FishEyeCorrect.h"


CFishEyeCorrect::~CFishEyeCorrect(){}

/*****************************
*����תΪ����
******************************/

void CFishEyeCorrect::ParamFix()
{
	phi0 = phi0 * piParam;
	sita0 = (90 - sita0) * piParam;
	gama0 = gama0 * piParam;

	phi1 = phi1 * piParam;
	sita1 = (90 - sita1) * piParam;
	gama1 = gama1 * piParam;

	phi2 = phi2 * piParam;
	sita2 = (90-sita2)*piParam;
	gama2 = gama2 * piParam;

	phi3 = phi3 * piParam;
	sita3 = (90-sita3)*piParam;
	gama3 = gama3 * piParam;
}

/*****************************
*���캯������ʼ��piParam
******************************/

CFishEyeCorrect::CFishEyeCorrect():piParam(0.01745329)
{
	
}

/*****************************
*���ܣ����Դͼ��������������
*�����������Ƶ����ͼ
*�����������
*����ֵ�������������ڵľ���
******************************/

Rect CFishEyeCorrect::GetArea(const Mat &inputImage)
{
	FindCorrectArea(inputImage);
	return correctArea;
}

/*****************************
*���ܣ����Դͼ��������������
*�����������Ƶ����ͼ
*�����������
*����ֵ����
******************************/

void CFishEyeCorrect::FindCorrectArea(const Mat &inputImage)
{
	Mat grayImage;
	cvtColor(inputImage,grayImage,CV_BGR2GRAY,1);
	/*��ֵ��*/
	threshold(grayImage,grayImage,40,255,THRESH_BINARY);
	vector<vector<Point > > contours;
	/*������*/
	findContours(grayImage,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	Rect minAreaTemp;
	int areaSize(0); 
	int areaSizeTemp(0);
	int areaIter(0);
	/*�ҵ��������,��������Բ������*/
	for (int contoursIter = 0;contoursIter != contours.size();++contoursIter)
	{
	//*�������İ�Χ����*/
		minAreaTemp=boundingRect(contours[contoursIter]);
		areaSizeTemp=minAreaTemp.width*minAreaTemp.height;
		if (areaSize<areaSizeTemp)
		{
			correctArea=minAreaTemp;
			areaSize=areaSizeTemp;
		}
	}
	
}

/*****************************
*���ܣ������������
*�����������������Ŀ��
*�����������
*����ֵ����
******************************/
void CFishEyeCorrect::CalculateParam(int width, int height)
{
	r=0.5*(width<height?width:height);
	f= r*2/3.14159265;
	cx=(width-1)/2;  
	cy=(height-1)/2;   
}


/*****************************
*���ܣ�չ��ͼ��������������
*���������չ��ͼ�����꣬չ��ͼ�ķ�λ�ǣ����ǣ��ӽ�
*������������������еĶ�Ӧ����
*����ֵ����
******************************/

void CFishEyeCorrect::Fix(int &u,int &v,double &x,double &y,const double &phi,const double &sita,const double &gama)
{
	/*չ��ƽ����У��ͼ������ƽ����ռ������ϵ*/
	double wp,hp,dux,duy,duz,dvx,dvy,dvz,xp,yp,zp,SQRT,sit,c_p,s_p;
	//1
	/*չ��ͼ���ϵ�һ�����ش�С��Ӧ͸��ͶӰƽ���ϵĿ�ߴ�С*/
	wp=r*tan(gama/2)/(cx+0.5);     
	hp=r*tan(gama/2)/(cy+0.5);     
	//2
	/*չ��ͼU����������λ���ض�Ӧ�������ϵx,y,z�������ϵı仯��*/
	dux=wp*sin(phi),duy=-wp*cos(phi),duz=0;
	/*չ��ͼV����������λ���ض�Ӧ�������ϵx,y,z�������ϵı仯��*/
	dvx=-hp*cos(sita)*cos(phi),dvy=-hp*cos(sita)*sin(phi);
	dvz=hp*sin(sita);
	//3
	/*չ��ͼ��һ����������������ϵx,y,z������ֵ*/
	xp=r*sin(sita)*cos(phi)+(cx-u)*dux+(v-cy)*dvx;
	yp=r*sin(sita)*sin(phi)+(cx-u)*duy+(v-cy)*dvy;
	zp=r*cos(sita)+(v-cy)*dvz;
	//4
	/*���б�߳���*/
	SQRT=sqrt(xp*xp+yp*yp);
	/*�������*/
	sit=atan(SQRT/zp);
	/*��cosֵ*/
	c_p=xp/SQRT;   
	/*��sinֵ*/          
	s_p=yp/SQRT;
	//5
	/*��Ӧ����ͼ������*/
	x=cx+f*sit*c_p;
	y=cy+f*sit*s_p;
}

/*****************************
*���ܣ������ķ�չ��ͼ��Ӧ������ͼ��������
*�������������ͼ
*�����������
*����ֵ����
******************************/

void CFishEyeCorrect::CalcPositon(Mat &fishImage )
{
	double x1 = 0 ;
	double y1 = 0;
	int tempPos(0);
	for (int row = 0;row < dstHeight ; ++row)
	{
		for (int col = 0;col < dstWidth; ++col)
		{
			int temprow = row * hRatio ;
			int tempcol = col * wRatio;
			tempPos = row*dstWidth*4 + col*2;
			Fix(temprow,tempcol, x1,y1,phi0,sita0,gama0);
			position[tempPos] = (int)x1;
			position[tempPos + 1] = (int)y1;
			Fix(temprow,tempcol, x1,y1,phi1,sita1,gama1);
			position[tempPos += dstWidth*2] = (int)x1;
			position[tempPos + 1] = (int)y1;
			tempPos = (row+dstHeight)*4*dstWidth + col*2;
			Fix(temprow,tempcol, x1,y1,phi2,sita2,gama2);
			position[tempPos] = (int)x1;
			position[tempPos + 1] = (int)y1;
			Fix(temprow,tempcol, x1,y1,phi3,sita3,gama3);
			position[tempPos += dstWidth*2] = (int)x1;
			position[tempPos + 1] = (int)y1;
		}
	}

}
/*****************************
*���ܣ�����ͼУ��
*�������������ͼ��У��ͼ��
*�����������
*����ֵ����
******************************/

void CFishEyeCorrect::NewImageCorrect(const Mat& orgImg,Mat& dstImg)
{
	//clock_t start = clock();

	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];

	uchar* dstImgData = dstImg.data; 
	uchar* orgImgData = orgImg.data;

	unsigned int tempPos(0);
	unsigned int temOrgPos(0);
	unsigned int temDstPos(0);

	for (int row = 0;row < dstHeight*2; ++row)
	{
		for (int col = 0;col <dstWidth*2; ++col)
		{
		
			tempPos = ((row * dstWidth)<<2) + (col << 1);
			temOrgPos = orgImgLineWidht*position[tempPos] + ((position[tempPos + 1])<<1) + position[tempPos + 1];
			memcpy(dstImgData + temDstPos,orgImgData+temOrgPos,3);
			temDstPos +=3;
		}
	}
	//clock_t end = clock();
	//cout<<((end - start)/1000)<<endl;
}

void CFishEyeCorrect::FixPosition(const Mat& orgImg)
{
	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];

	long tempPos(0);
	for (int row = 0;row < dstHeight*2; ++row)
	{
		for (int col = 0;col <dstWidth*2; ++col)
		{
			tempPos = ((row * dstWidth)<<2) + (col << 1);
			positionShift[dstWidth*row + col] = (double)orgImgLineWidht*(double)position[tempPos] + (double)((position[tempPos + 1])<<1) + (double)position[tempPos + 1];
		}
	}
}
