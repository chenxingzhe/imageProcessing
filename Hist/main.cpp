#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
const int colvl = 256;
cv::Mat ycMatchHist(cv::Mat srcImg, cv::Mat dstImg, int flag=1)
{
	// ****** ����� RGB ͼƬ��תΪ�Ҷ�ͼƬ���� ******
	Mat out(srcImg);
	if (flag == 1)
	{
		cvtColor(srcImg, out, CV_BGR2GRAY);
	}
	else if (flag == 0)
	{
	}
	int grayLevel[colvl];
	for (int i = 0; i < colvl; i++)	grayLevel[i] = i;

	int grayArr[colvl];
	int srcRow = srcImg.rows;
	int srcCol = srcImg.cols;
	int dstRow = dstImg.rows;
	int dstCol = dstImg.cols;
	float srcCdfArr[colvl] = { 0.f };
	float dstCdfArr[colvl] = { 0.f };
	float tmp;

	// *** ���ԴͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ� *** 
	memset(grayArr, 0, sizeof(grayArr));
	for (size_t nrow = 0; nrow < srcRow; nrow++)
	for (size_t ncol = 0; ncol < srcCol; ncol++)
	{
		int tag = srcImg.at<uchar>(nrow, ncol);
		grayArr[tag]++;
	}

	tmp = 0;
	for (int i = 0; i < colvl; i++)
	{
		tmp += grayArr[i];
		srcCdfArr[i] = tmp / (srcRow * srcCol);
		// std::cout<<srcCdfArr[i]<<std::endl;
	}

	// *** ���Ŀ��ͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ� *** 
	memset(grayArr, 0, sizeof(grayArr));
	for (size_t nrow = 0; nrow < dstRow; nrow++)
	for (size_t ncol = 0; ncol < dstCol; ncol++)
	{
		int tag = dstImg.at<uchar>(nrow, ncol);
		grayArr[tag]++;
	}

	tmp = 0;
	for (int i = 0; i < colvl; i++)
	{
		tmp += grayArr[i];
		dstCdfArr[i] = tmp / (dstRow * dstCol);
	}

	// *** ֱ��ͼƥ���㷨 ***
	int histMap[colvl];
	int minTag;
	for (int i = 0; i < colvl; i++)
	{
		float minMap = 10.f;
		for (int j = 0; j<colvl; j++)
		{
			if (minMap > abs(srcCdfArr[i] - dstCdfArr[j]))
			{
				minMap = abs(srcCdfArr[i] - dstCdfArr[j]);
				minTag = j;
			}
		}
		histMap[i] = minTag;
	}

	for (size_t nrow = 0; nrow < out.rows; nrow++)
	for (size_t ncol = 0; ncol < out.cols; ncol++)
	{
		int tag = out.at<uchar>(nrow, ncol);
		out.at<uchar>(nrow, ncol) = histMap[tag];
	}

	return out;
}
Mat drawhist(Mat src,int flag=1)
{

	Mat gray;
	if (flag == 1)
		cvtColor(src, gray, CV_RGB2GRAY);
	else
		gray = src;
	Mat hist = Mat::zeros(1, 256, CV_64FC1);
	CvSize a = gray.size();
	for (int i = 0; i < a.height; i++)
	for (int j = 0; j < a.width; j++)
	{
		int num = gray.at<uchar>(i, j);
		//cout << num << endl;
		hist.at<double>(0, num) = hist.at<double>(0, num) + 1;

	}
	double max_val;
	minMaxLoc(hist, 0, &max_val, 0, 0);
	cout << max_val << endl;
	int scale = 2;
	int hist_height = 256;
	int bins = 256;
	//ԭʼͼƬֱ��ͳ�ƣ�ʹ���Լ�д��ͳ�Ʒ�ʽ
	Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3);
	//Mat hist_img1 = Mat::zeros(hist_height, bins*scale, CV_8UC3);
	for (int i = 0; i < bins; i++)
	{
		float bin_val = hist.at<double>(0, i);
		int intensity = cvRound(bin_val*hist_height / max_val);  //Ҫ���Ƶĸ߶�
		rectangle(hist_img, Point(i*scale, hist_height - 1),
			Point((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	return hist_img;
}
int main()
{
	Mat src, gray;
	src = imread("fengjing.jpg");
	//CvSize a = src.size();
	//Mat t = drawhist(src);
	cvtColor(src, gray, CV_RGB2GRAY);
	CvSize a = gray.size();
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	Mat hist = Mat::zeros(1, 256, CV_64FC1);
	Mat hist1 = Mat::zeros(1, 256, CV_64FC1);
	int HistGram[256];
	double probpixel[256], cumupixel[256];
	//int channels[] = { 0 };

	//calcHist(&gray, 1, channels, Mat(), // do not use mask
	//	hist, 1, hist_size, ranges,
	//	true, // the histogram is uniform
	//	false);
	//ԭͼֱ��ͳ��
	for (int i = 0; i < a.height; i++)
	for (int j = 0; j < a.width; j++)
	{
		int num = gray.at<uchar>(i, j);
		//cout << num << endl;
		hist.at<double>(0, num) = hist.at<double>(0, num)+1;
		
	}
	for (int i = 0; i < 256; i++)
	{
		probpixel[i] = hist.at<double>(0, i) / (a.height*a.width);
		if (i == 0)
			cumupixel[i] = probpixel[i];
		else
			cumupixel[i] = cumupixel[i - 1] + probpixel[i];
	}
	Mat junheng(a.height, a.width, CV_8UC1);
	for (int i = 0; i < a.height; i++)
	for (int j = 0; j < a.width; j++)
	{
		int num = gray.at<uchar>(i, j);
		//cout << num << endl;
		junheng.at<uchar>(i, j) = (uchar)(cumupixel[num] * 255+0.5);

	}
	//����ͼֱ��ͳ��
	for (int i = 0; i < a.height; i++)
	for (int j = 0; j < a.width; j++)
	{
		int num = junheng.at<uchar>(i, j);
		//cout << num << endl;
		hist1.at<double>(0, num) = hist1.at<double>(0, num) + 1;

	}
	imshow("���⻯���", junheng);
	Mat hist_img = drawhist(gray, 0);
	Mat hist_img1 = drawhist(junheng, 0);
	imshow("ԭͼ", gray);
	imshow("ԭͼͼֱ��ͳ��", hist_img);
	imshow("����ͼֱ��ͳ��", hist_img1);
	Mat match_re = ycMatchHist(gray, junheng, 0);
	Mat hist_img2 = drawhist(match_re, 0);
	imshow("�涨�����", match_re);
	imshow("�涨��ֱ��ͳ��", hist_img2);
	char c = cvWaitKey();
	return 0;
}