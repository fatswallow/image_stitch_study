#include <atlbase.h>
#include "tools.h"

string tools::filePathToProcess = "";
Mat tools::image = Mat();

void tools::DisplayImage(Mat img)
{

}

Mat tools::GetImage()
{
	if (image.data)
		return image;
}

bool tools::readImage()
{
	if (GetFilePathToProcess())
	{
		image = imread(tools::filePathToProcess, IMREAD_COLOR);
		
		if (!image.data)
		{
			Tip("Failed to read in a image.");
			Tip("");
			return false;
		}
		return true;
	}
	else
	{

		Tip("You should select an image to process in the dialog!");
		Tip("");
	}
	return false;
}
bool tools::GetFilePathToProcess()
{
	OPENFILENAMEA ofn;
	char szFile[MAX_PATH];

	char curPath[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, curPath);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "jpg\0*.jpg\0bmp\0*.bmp\0png\0*.png\0"; //��������ѡ�� jpg/bmp/pngͼƬ��ʽ
	ofn.nFilterIndex = 1;      //������ѡ������һ��
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrInitialDir = curPath;

	if (GetOpenFileNameA(&ofn))
	{	
		filePathToProcess = szFile;
#ifdef _DEBUG
		cout << "The path of image to be opened is:"<<endl
			<<"\t"<<filePathToProcess << endl;
#endif // _DEBUG	
		return true;
	}
	return false;
}

//�÷���ʾ
void tools::Tip(string message)
{
	if (message.empty())
	{
		cout << "Press \"Enter\" to exit ...";
		getchar();
		exit(0);
	}
	else
	{
		cout << "******* MESSAGE *******"<<endl
			<<message << endl 
			<< "***********************"<<endl;
	}
}