#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
using namespace std;
using namespace cv;
int threshold_value1 = 125;
int threshold_value2 = 255;
char* trackbar_value1 = "H_low Value";
char* trackbar_value2 = "H_high Value";
void Threshold_Demo(int, void*)
{}
void rgb2gray(Mat rgb, Mat &gray)
{
	CV_Assert(rgb.depth() != sizeof(uchar));
	const int channels = rgb.channels();
	CvSize a = rgb.size();
	int width = a.width;
	int height = a.height;
	switch (channels)
	{

	case 3:
	{
			  MatIterator_<Vec3b> it, end;
			  MatIterator_<uchar> it1, end1;
			  it = rgb.begin<Vec3b>();//bgr
			  for (it = rgb.begin<Vec3b>(), end = rgb.end<Vec3b>(),it1=gray.begin<uchar>(); it != end; ++it,++it1)
			  {
				  *it1 = 0.11*(*it)[0] + 0.59*(*it)[1] + 0.3*(*it)[2];
			  }
			
	}
	}

}
bool IsDimodal(int* HistGram)       // 检测直方图是否为双峰的
{
	// 对直方图的峰进行计数，只有峰数位2才为双峰 
	int Count = 0;
	for (int Y = 1; Y < 255; Y++)
	{
		if (HistGram[Y - 1] < HistGram[Y] && HistGram[Y + 1] < HistGram[Y])
		{
			//cout << Y << endl;
			Count++;
			//if (Count > 2) return false;
		}
	}
	cout << Count << endl;
	if (Count == 2)
		return true;
	else
		return false;
}
int GetMinimumThreshold(int* HistGram)
{
	int Y, Iter = 0;
	int x1, x2;//第一第二峰值坐标
	int y1=0 ,y2=0;  //峰值    
	int HistGram2[256];
	int stepnum = 5;
	while (IsDimodal(HistGram) == false)                                        // 判断是否已经是双峰的图像了      
	{
		
		for (int i = 0; i < 256; i++)
		{
			int count = 0;
			double sum = 0;
			for (int j = -stepnum / 2; j <= stepnum / 2; j++)
			{
				if (i + j >= 0 && i + j < 256)
				{
					sum += HistGram[i + j];
					count++;
				}
			}
			HistGram2[i] = sum / count;

		}
		for (int i = 0; i < 256; i++)
		{
			HistGram[i]=HistGram2[i] ;

		}
		Iter++;
		if (Iter >= 1000) return -1;                                                   // 直方图无法平滑为双峰的，返回错误代码
	}
	if (!IsDimodal(HistGram))
		return -1;
	int kk = 0;
	for (int i = 0; i < 255; i++)
	{
		if (HistGram[i - 1] < HistGram[i] && HistGram[i + 1] < HistGram[i])
		{
		
				if (kk == 0)
				{
					x1 = i;
					kk++;
				}
				if (kk == 1)
				{
					x2 = i;
				}
			
		}
		
	}
	cout << x1 << endl << x2 << endl;
	int s, e;
	if (x1 > x2)
	{
		s = x2;
		e = x1;
	}
	else
	{
		s = x1;
		e = x2;
	}
	int miny=999999;
	int xx;
	for (int i = s; i < e; i++)
	{
		if (HistGram[i] < miny)
		{
			miny = HistGram[i];
			xx = i;
		}
	}
		
	// 阈值极为两峰之间的最小值 
	return xx;
}

int main()
{
	Mat rgb;
	cv::namedWindow("BarValueThres");
	rgb=imread("lena.jpg");
	char c=0;
	int  HistGram[256];
		createTrackbar(trackbar_value1,
			"BarValueThres", &threshold_value1,
			255, Threshold_Demo);

		createTrackbar(trackbar_value2,
			"BarValueThres", &threshold_value2,
			255, Threshold_Demo);
		imshow("rgb", rgb);
		CvSize a = rgb.size();
		Mat gray(a.width, a.height, CV_8UC1);

		rgb2gray(rgb, gray);
		//GaussianBlur(gray, gray, Size(19, 19), 0);
		//medianBlur(gray, gray, 9);
		memset(HistGram, 0, sizeof(HistGram));
		double jun=0;
		for (int i = 0; i < 300; i++)
		for (int j = 0; j< 300; j++)
		{
			int num = gray.at<uchar>(i, j);
			//cout << num << endl;
			jun += num;
			HistGram[num]++;
		}
		jun /= 300 * 300;
		//cout << jun << endl;
		static ofstream outfile;
		if (!outfile.is_open()) {
			cout << "not open" << endl;
			outfile.open("point.txt", ios::out);//文件名改成自己的
		}
		for (int i = 0; i < 256; i++)
		{
			outfile << HistGram[i] << '\t';
		}
		/*for (int i = 0; i < 256; i++)
		{
		cout << HistGram[i] << endl;
		}*/

		int thre = GetMinimumThreshold(HistGram);
		cout << thre << endl;
		imshow("gray", gray);
		Mat shuangfeng,junzhi;
		//cout << threshold_value1 << endl;
		inRange(gray, thre, 255, shuangfeng);
		imshow("shuangfeng", shuangfeng);
		inRange(gray, jun, 255, junzhi);
		imshow("junzhi", junzhi);
		waitKey();

		

	

}