#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
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
void RGB2Lab(double R, double G, double B, double &L, double &a, double &b)
{
	double X, Y, Z, fX, fY, fZ;

	X = 0.412453*R + 0.357580*G + 0.180423*B;
	Y = 0.212671*R + 0.715160*G + 0.072169*B;
	Z = 0.019334*R + 0.119193*G + 0.950227*B;

	X /= (255 * 0.950456);
	Y /= 255;
	Z /= (255 * 1.088754);

	if (Y > 0.008856)
	{
		fY = pow(Y, 1.0 / 3.0);
		L = 116.0*fY - 16.0;
	}
	else
	{
		fY = 7.787*Y + 16.0 / 116.0;
		L = 903.3*Y;
	}

	if (X > 0.008856)
		fX = pow(X, 1.0 / 3.0);
	else
		fX = 7.787*X + 16.0 / 116.0;

	if (Z > 0.008856)
		fZ = pow(Z, 1.0 / 3.0);
	else
		fZ = 7.787*Z + 16.0 / 116.0;

	a = 500.0*(fX - fY);
	b = 200.0*(fY - fZ);

}
double k[9] = { 0.412453, 0.357580, 0.180423, 0.212671, 0.715160, 0.072169, 0.019334, 0.119193, 0.950227 };
double f(double x)
{
	if (x > 0.008856)
		return pow(x, 1.0/3);
	else
		return (7.787*x + 16 / 116);
}
void rgb2lab(Mat image, Mat &dst)
{
	CvSize s = image.size();
	int height = s.height;
	int width = s.width;
	double Xn = 0.950456;
	double Zn = 1.088754;
	Mat K(3, 3, CV_64FC1, k);
	dst = Mat(height, width, CV_8UC3);
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	{
		//cout << (int)image.at<cv::Vec3b>(i, j)[0] <<" "<< (int)image.at<cv::Vec3b>(i, j)[1] <<" "<< (int)image.at<cv::Vec3b>(i, j)[2] << endl;
		double m1 = (double)image.at<cv::Vec3b>(i, j)[0] / 255, m2 = (double)image.at<cv::Vec3b>(i, j)[1] / 255, m3 = (double)image.at<cv::Vec3b>(i, j)[2] / 255;
		double l, a, b;
		
		double ss[3] = { m3, m2, m1 };
		//cout << s[0]<<s[1]<<s[2] << endl;
		
		Mat rgb(3, 1, CV_64FC1, ss);
		//cout << rgb << endl;
		Mat xyz = K*rgb;
		//cout << K << endl;
		//cout << rgb << endl;
		double x = xyz.at<double>(0, 0);
		double y = xyz.at<double>(0, 1);
		double z = xyz.at<double>(0, 2);
		x = x / Xn;
		z = z / Zn;
		if (y > 0.008856)
			l = 116 * pow(y, 1.0 / 3) - 16;
		else
			l = 903.3*y;
		double delta = 0;
		a = 500 * (f(x) - f(y)) + delta;
		b = 200 * (f(y) - f(z)) + delta;
	/*	l = l * 255.0 / 100;
		a = a + 128;
		b = b + 128;*/

		dst.at<Vec3b>(i, j)[0] = (uchar)l;
		dst.at<Vec3b>(i, j)[1] = (uchar)a;
		dst.at<Vec3b>(i, j)[2] = (uchar)b;
		//cout << xyz << endl;
		


	}
}

int main()
{
	Mat image,dst,dst1,dst2,dst3;
	image = imread("lena.jpg");
	cvtColor(image, dst, CV_BGR2HSV);
	cvtColor(image, dst3, CV_BGR2Lab);
	rgb2hsv(image, dst1);
	rgb2lab(image, dst2);
	imshow("RGB", image);
	imshow("HSVopencv", dst);
	imshow("HSV", dst1);
	imshow("lab", dst2);
	imshow("labopencv", dst3);
	double L, A, B;
	double r = (double)image.at<cv::Vec3b>(0, 0)[2], g = (double)image.at<cv::Vec3b>(0, 0)[1], b = (double)image.at<cv::Vec3b>(0, 0)[0];
	RGB2Lab(r, g, b, L, A, B);
	cout << pow(27, 1.0 / 3) << endl;
	//cout << (int)image.at<cv::Vec3b>(0, 0)[0] << " " << (int)image.at<cv::Vec3b>(0, 0)[1] << " " << (int)image.at<cv::Vec3b>(0, 0)[2] << endl;
	cout << (int)dst.at<cv::Vec3b>(0, 0)[0] << " "<<(int)dst.at<cv::Vec3b>(0, 0)[1] << " "<<(int)dst.at<cv::Vec3b>(0, 0)[2] << endl;
	cout << (int)dst1.at<cv::Vec3b>(0, 0)[0] << " " << (int)dst1.at<cv::Vec3b>(0, 0)[1] << " " << (int)dst1.at<cv::Vec3b>(0, 0)[2] << endl;
	cout << (int)dst2.at<cv::Vec3b>(0, 0)[0] << " " << (int)dst2.at<cv::Vec3b>(0, 0)[1] << " " << (int)dst2.at<cv::Vec3b>(0, 0)[2] << endl;
	cout << L << " hi" << A << " " << B << endl;
	cout << (int)dst3.at<cv::Vec3b>(0, 0)[0] << " " << (int)dst3.at<cv::Vec3b>(0, 0)[1] << " " << (int)dst3.at<cv::Vec3b>(0, 0)[2] << endl;
	char c = cvWaitKey();
}