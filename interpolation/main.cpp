

#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat jinlin(Mat image, int dstwidth, int dstheight)
{
	CvSize s = image.size();
	int srcheight = s.height;
	int srcwidth = s.width;
	Mat dst(dstheight, dstwidth, CV_8UC1);
	for (int i = 0; i < dstheight; i++)
	for (int j = 0; j < dstwidth; j++)
	{
		double srcx = i*srcheight*1.0 / dstheight;
		double srcy = j*srcwidth*1.0 / dstwidth;
		dst.at<uchar>(i, j) = image.at<uchar>((int)srcx, (int)srcy);
	}
	return dst;
}
Mat xianxing(Mat image, int dstwidth, int dstheight)
{
	CvSize s = image.size();
	int srcheight = s.height;
	int srcwidth = s.width;
	Mat dst(dstheight, dstwidth, CV_8UC1);
	for (int i = 0; i < dstheight; i++)
	for (int j = 0; j < dstwidth; j++)
	{
		double srcx = i*srcheight*1.0 / dstheight;
		double srcy = j*srcwidth*1.0 / dstwidth;
		int x = (int)srcx;
		int y = (int)srcy;
		double u = srcx - x;
		double v = srcy - y;
		dst.at<uchar>(i, j) = (1 - u)*(1 - v)*image.at<uchar>(x, y) + (1 - u)*v*image.at<uchar>(x, y + 1) + u*(1 - v)*image.at<uchar>(x + 1, y) + u*v*image.at<uchar>(x + 1, y + 1);
	}
	return dst;
}
Mat pingyi(Mat image, int r, int d)
{
	CvSize s = image.size();
	int srcheight = s.height;
	int srcwidth = s.width;
	Mat dst = Mat::zeros(image.size(), CV_8UC1);
	//Mat dst(srcheight, srcwidth, CV_8UC1);
	for (int i = 0; i < srcheight; i++)
	for (int j = 0; j < srcwidth; j++)
	{
		//dst.at<uchar>(i, j) = 0;

		if (i + r < srcheight&&i + r >= 0 && j + d < srcwidth&&j + d >= 0)
		{
			dst.at<uchar>(i + r, j + d) = image.at<uchar>(i, j);
		}
		
	}
	return dst;
}
Mat diandao(Mat image, int flag)
{
	CvSize s = image.size();
	int srcheight = s.height;
	int srcwidth = s.width;
	Mat dst = Mat::zeros(image.size(), CV_8UC1);
	//Mat dst(srcheight, srcwidth, CV_8UC1);
	if (flag == 0)
	{
		for (int i = 0; i < srcheight; i++)
		for (int j = 0; j < srcwidth; j++)
		{
			//dst.at<uchar>(i, j) = 0;

			dst.at<uchar>(i, srcwidth - j - 1) = image.at<uchar>(i, j);

		}
	}
	else
	{
		for (int i = 0; i < srcheight; i++)
		for (int j = 0; j < srcwidth; j++)
		{
			//dst.at<uchar>(i, j) = 0;

			dst.at<uchar>(srcheight-i-1, j) = image.at<uchar>(i, j);

		}
	}
	
	return dst;
}
Mat xuanzhuan(Mat image, double theta)
{
	CvSize s = image.size();
	int srcheight = s.height;
	int srcwidth = s.width;
	int xcenter = srcwidth / 2;
	int ycenter = srcheight / 2;
	double a = sin(theta), b = cos(theta);
	int width_rotate = int(srcheight * fabs(a) + srcwidth * fabs(b));
	int height_rotate = int(srcwidth * fabs(a) + srcheight * fabs(b));
	Mat dst = Mat::zeros(cvSize(width_rotate,height_rotate), CV_8UC1);
	//Mat dst(srcheight, srcwidth, CV_8UC1);
	double dx = -0.5*width_rotate*cos(theta) - 0.5*height_rotate*sin(theta) + 0.5*srcwidth;
	double dy = 0.5*width_rotate*sin(theta) - 0.5*height_rotate*cos(theta) + 0.5*srcheight;
	for (int i = 0; i <height_rotate; i++)
	for (int j = 0; j < width_rotate; j++)
	{
		//dst.at<uchar>(i, j) = 0;
		//double srcx=cos(theta)*i+sin(theta)*j ;
		//double srcy =-sin(theta)*i+cos(theta)*j;
		double x1 = j * cos(theta) + i *sin(theta)+dx;// +xcenter;
		double y1 = (-j) *sin(theta) + i  *cos(theta)+dy;// +ycenter;
		int x = cvRound(x1);
		int y = cvRound(y1);
		if (y >= 0 && y < srcheight&&x >= 0 && x < srcwidth)
		{
			dst.at<uchar>(i, j) = image.at<uchar>(y, x);
		}
		
		
		
		

	}
	return dst;
}
int main()
{
	Mat image;
	image = imread("lena.jpg", 0);
	int dstwidth=800, dstheight=800;
	Mat dst = jinlin(image, dstwidth, dstheight);
	Mat dst1 = jinlin(image, 200, 200);
	Mat dst2 = pingyi(image, 50, 50);
	Mat dst3 = diandao(image, 1);
	Mat dst4 = diandao(image, 0);
	Mat dst5 = xuanzhuan(image, 3.14/4);
	imshow("origin", image);
	imshow("放大", dst);
	imshow("缩小", dst1);
	imshow("平移", dst2);
	imshow("垂直颠倒", dst3);
	imshow("水平颠倒", dst4);
	imshow("旋转", dst5);
	char key = cvWaitKey();
	
}
