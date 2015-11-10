#include <iostream> 			   // for standard I/O
#include <string>   			   // for strings
#include <iomanip>  			   // for controlling float print precision
#include <sstream>                         // string to number conversion
#include <pthread.h>
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include "string.h"
#include "FishEyeCorrect.h"
#include  "opencv2/core/core.hpp"           // Basic OpenCV structures (cv::Mat, Scalar)
#include  "opencv2/imgproc/imgproc.hpp"     // Gaussian Blur
#include  "opencv2/highgui/highgui.hpp"     // OpenCV window I/O

using namespace std;
using namespace cv;

int frameNumber = 0;
/*�߳�1  �����ĸ����У��*/
void  *thrd_func1(void *argv);                  
/*�߳�2   ������Ƶ����*/
void  *thrd_func2(void *argv);	

/*��ȡһ���ַ����е�����(��txt����ȡ���ֲ���)*/		        
void str2num(string& str,vector<int>& rint);    
/*�ַ�������ȡ����*/
int substr2num(string& str);	

void numinfile(string path,int &num,char& ch1,char& ch2,char& ch3);
void savechangetofile(string path,int num);
			
CFishEyeCorrect fishCorrect;
/*������Ƶ֡�ı���*/
Mat frame;	
VideoCapture cap;
Mat destImage;
Mat orgImage;
int main(int argc, char *argv[])
{
      if(argc <2)
      {
         return -1;
      }

	int nn;
	char c1,c2,c3;
	string pp="copyrights.txt";
	string pp1="in2.txt";
	numinfile(pp,nn,c1,c2,c3);
	if (nn > 100)
		{
			cout<<"ʹ�������ѵ�������ϵ�ṩ����"<<endl;
			return;
			}
	savechangetofile(pp,nn+1);

	cap.open(argv[1]);
	/*��ȡ��Ƶ֡��*/
	frameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);    
	if(!cap.isOpened())
	{
	   cout<<"can't open the video!"<<endl;
	   return 0;
	}

	int flag = 0;
	/*�ַ�����������*/
	char  buf[100];										
	ifstream in("param.txt");
	int totalnum = 0;

	while (in.getline(buf,100))
	{
		string str = buf;
		vector<int>   myint;
		/*��ȡһ���ַ����е�����*/
		str2num(str,myint);	
		/*��һ�У���ȡ��͸�*/							
		if(0==totalnum)
		{
			fishCorrect.dstWidth = myint[0]/2;
			fishCorrect.dstHeight = myint[1]/2;
			fishCorrect.wRatio = myint[2];
			fishCorrect.hRatio = myint[3];
		}
		/*��ȡ�ڶ��У���ȡ��һ��չ��ͼ�Ĳ�������λ�ǣ����ǣ��ӽ�*/
		else if(1==totalnum)
		{
			fishCorrect.phi0 = myint[0];
			fishCorrect.sita0 = myint[1];
			fishCorrect.gama0 = myint[2];
		}
		/*��ȡ�����У���ȡ�ڶ���չ��ͼ�Ĳ�������λ�ǣ����ǣ��ӽ�*/
		else if(2==totalnum)
		{
			fishCorrect.phi1 = myint[0];
			fishCorrect.sita1 = myint[1];
			fishCorrect.gama1 = myint[2];
		}
		/*��ȡ�����У���ȡ������չ��ͼ�Ĳ�������λ�ǣ����ǣ��ӽ�*/
		else if(3==totalnum)
		{
			fishCorrect.phi2 = myint[0];
			fishCorrect.sita2 = myint[1];
			fishCorrect.gama2 = myint[2];
		}
		/*��ȡ�����У���ȡ���ĸ�չ��ͼ�Ĳ�������λ�ǣ����ǣ��ӽ�*/
		else if(4==totalnum)
		{
			fishCorrect.phi3 = myint[0];
			fishCorrect.sita3 = myint[1];
			fishCorrect.gama3 = myint[2];
		}	
		totalnum++;
	}
	cout<<"playing...."<<endl;
	/*ѭ��������Ƶ*/
        int wflag = 0;
	int num =0;
	
	fishCorrect.dstImg = Mat::zeros(Size(fishCorrect.dstWidth*2,fishCorrect.dstHeight*2),CV_8UC3);
	fishCorrect.position =  (int*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*8*sizeof(int));
	fishCorrect.positionShift = (long*)malloc(fishCorrect.dstWidth*fishCorrect.dstHeight*4*sizeof(long));
	for(int i = 0;i < frameNumber;++i)               
	{
	   		
		pthread_t tid1,tid2;
         	void *tret;
		if(i <= 21)
		{
		    cap >> frame;			
		}
		if(i >20)
		{
		    if (flag == 0)
		    {
		    	/*��ȡ����ͼ������*/
				fishCorrect.GetArea(frame);
				flag =1;
		    }
		   if (i>21)
		    {
			
			/*�����߳�2*/
		      if (pthread_create(&tid2,NULL,thrd_func2,NULL)!=0)
			 {
         		    printf("Create thread 2 error!\n");
         		    exit(1);
     			}
	     		/*�ȴ��߳�һִ�����*/
			if (pthread_join(tid1,&tret)!=0)
			{ 
				printf("Join thread 1 error!\n");
			      exit(1);
                         }
		   // ��ʾ���
		    if (wflag == 0)
		    {
		     	namedWindow("Video0",1);
		     	namedWindow("Video1",1);
			    wflag =1;
			}
		    // clock_t t1 = clock();
		    imshow("Video0",fishCorrect.orgImg);
		    imshow("Video1",fishCorrect.dstImg);
		  //  clock_t t2 = clock();
		   // cout<<(t2-t1)/1000.0<<endl;
		     /*�ȴ��̶߳�ִ�����*/
		    if (pthread_join(tid2,&tret)!=0)
			{
		           printf("Join thread 2 error!\n");
		       	   exit(1);
		    }
		    waitKey(1);				
		   }
		   /*���û����Ƶ���ݣ�����ѭ��*/
	  		if (!frame.data)                                       
			{
			   break;
			}
			/*������������ȡ����������orgImg����*/
		  fishCorrect.orgImg  = frame(fishCorrect.correctArea);

		  if (i ==21)
		  {
		  	/*�����������*/
		    fishCorrect.ParamFix();
		    fishCorrect.CalculateParam(fishCorrect.orgImg.cols,fishCorrect.orgImg.rows);
			fishCorrect.CalcPositon(fishCorrect.orgImg);
			fishCorrect.FixPosition(fishCorrect.orgImg);
		  }
		if(i < frameNumber-1)
		{
		 /*�����߳�һ*/
     		 if (pthread_create(&tid1,NULL,thrd_func1,NULL)!=0) 
		     {
        	     printf("Create thread 1 error!\n");
                 exit(1);
         	  }
		}
	    }
	}
	      free(fishCorrect.position);
	      free(fishCorrect.positionShift);
	      cout<<"play over"<<endl;
	      return 0;
}
/*�̺߳���һ����Ƶ֡����У������*/
void  *thrd_func1(void *argv)
{
   fishCorrect.NewImageCorrect(fishCorrect.orgImg,fishCorrect.dstImg);
   pthread_exit(NULL); 
}
/*�̺߳���������Ƶ����*/
void  *thrd_func2(void *argv)
{
	cap >> frame;
	pthread_exit(NULL); 	
}

/**********************************************
*���ܣ���ȡ�ַ����е�����
*���������һ���ַ���
*�����������������
*����ֵ����
***********************************************/
void str2num(string& str,vector<int>& rint)
{
	/*ͨ���ַ�'/'���в��Ϊ��ͬ���ִ���Ȼ�����substr2num������ȡ�ִ��е�����*/
	string strSep = "/";
	int size_pos = 0;
	string strresult;
	int size_prev_pos = 0;
	int itemp = 0;

	while((size_pos=str.find_first_of(strSep,size_pos))!=string::npos)
	{
		strresult = str.substr(size_prev_pos,size_pos - size_prev_pos);
		/*���ִ���һ�������������*/
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
*���ܣ���ȡ�ִ��е�����
*���������һ���ַ���
*�����������
*����ֵ��һ������
***********************************************/
int substr2num(string& str)
{
	/*���ݵȺ���ȡ����*/
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


void numinfile(string path,int &num,char& ch1,char& ch2,char& ch3)
{
	char buf[300];
	char buf1[10];
	ifstream in(path.c_str());
	if (!in)
	{
		cout<<"the file is not exist!"<<endl;
	}

	int acc=0;
	char zf1,zf2,zf3;

	for(acc=0;acc<13;acc++)
	{
		in.getline(buf,300);	
		if (acc==0)
		{
			zf1 = buf[0];
		}
		if (acc==1)
		{
			zf2 = buf[0];
		}
		if (acc==2)
		{
			zf3 = buf[0];
		}
		//cout<<buf<<endl;
	}
	int sizenum1,sizenum2=0;
	sizenum1 = strlen(buf);
	memset(buf1,0,10);
	for (int i=0;i<sizenum1;i++)
	{
		char tem;
		tem = buf[i];
		if (tem != ' ')
		{
			buf1[i]=buf[i];
			sizenum2++;
		}
		else
		{
			break;
		}
	}
	int numtotal = 0;
	switch (sizenum2)
	{
	case 1:
		numtotal = (buf[0]-65);
		break;
	case 2:
		numtotal = (buf[0]-65)*10+(buf[1]-65);
		break;
	case 3:
		numtotal = (buf[0]-65)*100+(buf[1]-65)*10+(buf[2]-65);
		break;
	}
	num=numtotal;
	ch1=zf1;
	ch2=zf2;
	ch3=zf3;
}
void savechangetofile(string path,int num)
{
	char buf[300];
	char buf1[10];
	ifstream in(path.c_str());
	fstream outfile("in2.txt",ios::out);

	int acc;
	for(acc=0;acc<13;acc++)
	{
		in.getline(buf,300);	
		if (acc!=12)
		{
			outfile<<buf<<endl;
		}
	}

	int numtotal = num;
	char savechar[10];
	if(numtotal>=0&&numtotal<=9)
	{
		savechar[0]=numtotal+65;
		savechar[2]='\0';
	}
	else if(numtotal>=10&&numtotal<=99)
	{
		int shi,ge;
		shi=numtotal/10;
		ge=numtotal%10;
		shi+=65;
		ge+=65;
		savechar[0]=shi;
		savechar[1]=ge;
		savechar[2]='\0';
	}
	else if (numtotal>=100&&numtotal<=999)
	{
		int bai,shi,ge;
		int tem;
		bai = numtotal/100;
		tem = numtotal%100;
		shi = tem/10;
		ge = tem%10;
		bai+=65;
		shi+=65;
		ge+=65;
		savechar[0]=bai;
		savechar[1]=shi;
		savechar[2]=ge;
		savechar[3]='\0';
	}
	string str=savechar;
	str += " is writen by xarkit.";
	outfile<<str<<endl;
	in.close();
	outfile.close();

	string line;
	ofstream outfile1(path.c_str(),ios::out|ios::trunc);
	fstream file1("in2.txt");    
	while(!file1.eof())
	{          
		getline(file1,line);
		outfile1<<line<<endl;
	} 
	outfile1.close();
	file1.close();
	system("rm in2.txt");//ɾ���м��ļ�
}
