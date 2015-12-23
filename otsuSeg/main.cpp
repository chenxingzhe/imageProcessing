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
		avgValue += i * histogram[i];  //����ͼ���ƽ���Ҷ�
	}

	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷȵ�����(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���
		u += i * histogram[i];  // �Ҷ�i ֮ǰ������(0~i)��ƽ���Ҷ�ֵ�� ǰ�����ص�ƽ���Ҷ�ֵ

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