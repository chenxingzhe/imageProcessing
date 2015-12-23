#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <iostream>
#define sobel 0
#define prewitt 1
using namespace cv;
using namespace std;
int threshold_value1 = 125;
int threshold_value2 = 255;
char* trackbar_value1 = "H_low Value";
char* trackbar_value2 = "H_high Value";

void Threshold_Demo(int, void*)
{}
Mat edgeDetect(Mat image, int thre,int suanzi)
{
	int ssize = 3;
	int Gx[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };;
	int Gy[3][3] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	if (suanzi == 1)
	{
		
		Gx[1][0] = -1;
		Gx[1][2] = 1;
		Gy[0][1] = 1;
		Gy[2][1] = -1;


	}
	CvSize s = image.size();
	int height = s.height;
	int width = s.width;
	Mat dst;
	Mat dst1(height, width, CV_64FC1);
	for (int i = 0; i < height;i++)
	for (int j = 0; j < width; j++)
	{
		double xsum = 0, ysum = 0;
		//[f(x + 1, y - 1) + 2 * f(x + 1, y) + f(x + 1, y + 1)] - [f(x - 1, y - 1) + 2 * f(x - 1, y) + f(x - 1, y + 1)]
		// [f(x-1,y-1) + 2f(x,y-1) + f(x+1,y-1)]-[f(x-1, y+1) + 2*f(x,y+1)+f(x+1,y+1)]
		for (int ii = -ssize / 2; ii <= ssize / 2; ii++)
		for (int jj = -ssize / 2; jj <= ssize / 2; jj++)
		{
			int x = i + ii;
			int y = j + jj;
			if (x >= 0 & x < height&&y >= 0 && y < width)
			{
				xsum += image.at<uchar>(x, y)*Gx[jj + 1][ii + 1];
				ysum += image.at<uchar>(x, y)*Gy[jj + 1][ii + 1];
			}
		}
		double sum = fabs(xsum) + fabs(ysum);
		dst1.at<double>(i, j) = sum;

	}
	double mmin, mmax;
	minMaxIdx(dst1, &mmin, &mmax);
	cout << mmin << " " << mmax << endl;
	inRange(dst1, 0, thre, dst);
	return dst;

	

}
int main()
{
	Mat image;
	image=imread("lena.jpg", 0);
	cv::namedWindow("BarValueThres");
	createTrackbar(trackbar_value1,
		"BarValueThres", &threshold_value1,
		255, Threshold_Demo);
	createTrackbar(trackbar_value2,
		"BarValueThres", &threshold_value2,
		255, Threshold_Demo);
	char key;

	cout << (int)1.7 << endl;
	while (1)
	{
		Mat dst1=edgeDetect(image, threshold_value1,sobel);
		Mat dst2 = edgeDetect(image, threshold_value2, prewitt);
		imshow("sobel±ßÔµ", dst1);
		imshow("prewitt±ßÔµ", dst2);
		key = cvWaitKey(1);
		if (key == 27)
			break;
	}
	


}