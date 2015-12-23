//#include "stdafx.h"
#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "Math.h"

int Otsu(IplImage* src);

int main(int argc, char * argv[])
{
	IplImage* img = cvLoadImage("lena.jpg", 0);
	IplImage* dst = cvCreateImage(cvGetSize(img), 8, 1);
	int threshold = Otsu(img);
	printf("threshold = %d\n", threshold);
	cvThreshold(img, dst, threshold, 255, CV_THRESH_BINARY);

	cvNamedWindow("img", 1);
	cvShowImage("img", dst);


	cvWaitKey(-1);

	cvReleaseImage(&img);
	cvReleaseImage(&dst);

	cvDestroyWindow("dst");
	return 0;
}

int Otsu(IplImage* src)
{
	int height = src->height;
	int width = src->width;

	//histogram  
	float histogram[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		unsigned char* p = (unsigned char*)src->imageData + src->widthStep * i;
		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}
	//normalize histogram  
	int size = height * width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	//average pixel value  
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];  //整幅图像的平均灰度
	}

	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例
		u += i * histogram[i];  // 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值

		float t = avgValue * w - u;
		float variance = t * t / (w * (1 - w));
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}

	return threshold;
}