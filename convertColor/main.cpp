#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void rgb2hsv(Mat image, Mat &dst)
{
	CvSize s = image.size();
	int height = s.height;
	int width = s.width;
	dst = Mat(height, width, CV_8UC3);
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	{
		//cout << (int)image.at<cv::Vec3b>(i, j)[0] <<" "<< (int)image.at<cv::Vec3b>(i, j)[1] <<" "<< (int)image.at<cv::Vec3b>(i, j)[2] << endl;
		double m1 = (double)image.at<cv::Vec3b>(i, j)[0]/255, m2 = (double)image.at<cv::Vec3b>(i, j)[1]/255, m3 = (double)image.at<cv::Vec3b>(i, j)[2]/255;
		double h, s, v;
		if (m1 >= m2&&m1 >= m3)
		{
			v = m1;
			if (v == 0)
				s = 0;
			else
				s = (v - min(m2, m3))/v;
			h = 240 + 60 * (m3 - m2) / (v - min(m2, m3));
			if (h < 0)h = h + 360;
		}
		else if (m2 >= m1&&m2 >= m3)
		{
			v = m2;
			if (v == 0)
				s = 0;
			else
				s = (v - min(m1, m3))/v;
			h = 120 + 60 * (m1 - m3) / (v - min(m1, m3));
			if (h < 0)h = h + 360;
		}
		else if (m3 >= m1&&m3 >= m2)
		{
			v = m3;
			if (v == 0)
				s = 0;
			else
				s = (v - min(m1, m2))/v;
			h = 60 * (m2 - m1) / (v - min(m1, m2));
			if (h < 0)h = h + 360;
		}
		v = v * 255;
		s = s * 255;
		h = h / 2;
		dst.at<Vec3b>(i,j)[0] = (uchar)h;
		dst.at<Vec3b>(i, j)[1] = (uchar)s;
		dst.at<Vec3b>(i, j)[2] = (uchar)v;
             

	}
}
double k[9] = { 0.412453, 0.357580, 0.180423, 0.212671, 0.715160, 0.072169, 0.019334, 0.119193, 0.950227 };
void rgb2lab(Mat image, Mat &dst)
{
	CvSize s = image.size();
	int height = s.height;
	int width = s.width;
	dst = Mat(height, width, CV_8UC3);
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	{
		//cout << (int)image.at<cv::Vec3b>(i, j)[0] <<" "<< (int)image.at<cv::Vec3b>(i, j)[1] <<" "<< (int)image.at<cv::Vec3b>(i, j)[2] << endl;
		double m1 = (double)image.at<cv::Vec3b>(i, j)[0] / 255, m2 = (double)image.at<cv::Vec3b>(i, j)[1] / 255, m3 = (double)image.at<cv::Vec3b>(i, j)[2] / 255;
		double l, a, b;
		Mat K(3, 3, CV_64FC1, k);
		double s[3] = { m1, m2, m3 };
		Mat rgb(3, 1, CV_64FC1, s);
		Mat xyz = K*rgb;
		cout << xyz << endl;
		


	}
}

int main()
{
	Mat image,dst,dst1,dst2;
	image = imread("lena.jpg");
	cvtColor(image, dst, CV_BGR2HSV);
	rgb2hsv(image, dst1);
	rgb2lab(image, dst2);
	imshow("RGB", image);
	imshow("HSVopencv", dst);
	imshow("HSV", dst1);
	cout << (int)dst.at<cv::Vec3b>(0, 0)[0] << " "<<(int)dst.at<cv::Vec3b>(0, 0)[1] << " "<<(int)dst.at<cv::Vec3b>(0, 0)[2] << endl;
	cout << (int)dst1.at<cv::Vec3b>(0, 0)[0] << " " << (int)dst1.at<cv::Vec3b>(0, 0)[1] << " " << (int)dst1.at<cv::Vec3b>(0, 0)[2] << endl;
	char c = cvWaitKey();
}