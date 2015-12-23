#include <cv.h>
#include <cxcore.h>
#include <opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "canny��Ե���";

void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));//�Ƚ����˲�ƽ��
	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);//canny������Ҫ�Ǽ�����1.ͼ����ǿ��������ͼ���ݶȼ��䷽��2. �Ǽ���ֵ���ƣ��ݶȷ����ֵ��3.ȡ˫��ֵ
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

int main()
{
	src = imread("lena.jpg");
	if (!src.data)
	{
		return -1;
	}
	dst.create(src.size(), src.type());
	cvtColor(src, src_gray, CV_BGR2GRAY);
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
	waitKey(0);
	return 0;
}