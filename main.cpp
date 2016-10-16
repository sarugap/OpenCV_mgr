#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

#define ImageResizeCoef 4

using namespace cv;
using namespace std;

void CreateGUI(void);

Mat gOrginalImage,
	gGreyImage,
	gGaussBlurImage,
	gCannyEdgeImage,
	gColor_dst,
	hHSVImageBase,
	gRGBImage[3],
	gHSVImage[3],
	gEqualizationHist[3],
	gThresholdImage[3],
	gStructElement3_3,
	gStructElement5_5;
//-----------------------BLUE COLOR-----------------------\\		
/* H range [0-179]*/
int BlueLowH = 86; //94
int BlueHighH = 122;

/* S range [0-255]*/
int BlueLowS = 140; 
int BlueHighS = 255;

/* V range [0-255]*/
int BlueLowV = 92; 
int BlueHighV = 232;
//-----------------------RED COLOR-----------------------\\		
/* H range [0-179]*/
int RedLowH = 120;
int RedHighH = 179;

/* S range [0-255]*/
int RedLowS = 15; 
int RedHighS = 255;

/* V range [0-255]*/
int RedLowV = 170; //124 
int RedHighV = 255;	
//-----------------------YELLOW COLOR-----------------------\\		
/* H range [0-179]*/
int YellowLowH = 14;
int YellowHighH = 60;

/* S range [0-255]*/
int YellowLowS = 70; 
int YellowHighS = 255;

/* V range [0-255]*/
int YellowLowV = 131;
int YellowHighV = 255;
//---------------------------------------------------------\\	
/* Image resolution */
int H_res;
int W_res;

/* Median filter */
unsigned int MedFilter = 5;

int main(int argc, char** argv)
{
	//Load image
	gOrginalImage = imread(*(argv+1));

	if(gOrginalImage.empty())
	{
		cout <<"Invalid path to image!" <<endl;
		getchar();
		return -1;
	}

	CreateGUI();

	//StructuringElement - erode and dilate
	gStructElement3_3 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1,1));
	gStructElement5_5 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1,1));

	//Resize the original image
	H_res = gOrginalImage.rows;
	W_res = gOrginalImage.cols;
	resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));
	imshow("Original image", gOrginalImage); 

	//Image preprocesing
	split(gOrginalImage, gRGBImage);

	//Histogram equalization
	equalizeHist(gRGBImage[0], gEqualizationHist[0]);
	equalizeHist(gRGBImage[1], gEqualizationHist[1]);
	equalizeHist(gRGBImage[2], gEqualizationHist[2]);

	//Merge 3 matrix into 1 matrix
	merge(gEqualizationHist, 3, gRGBImage[0]);
	imshow("Histogram eqaulization", gRGBImage[0]); 

	//Convert image from BGR to HSV
	cvtColor(gRGBImage[0], hHSVImageBase, CV_BGR2HSV);

	//Split each channel
	split(hHSVImageBase, gHSVImage);

	//Blur H and S channel for HSV
	medianBlur(gHSVImage[0],gHSVImage[0],(MedFilter,MedFilter));
	medianBlur(gHSVImage[1],gHSVImage[1],(MedFilter,MedFilter));
	merge(gHSVImage, 3, hHSVImageBase);
	
	while(1)
	{
		//Threshold the HSV image - BLUE COLOR
		inRange(hHSVImageBase, Scalar(BlueLowH, BlueLowS, BlueLowV), Scalar(BlueHighH, BlueHighS, BlueHighV), gThresholdImage[0]); 

		dilate(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		dilate(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		
		erode(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		erode(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);

		//Threshold the HSV image - RED COLOR
		inRange(hHSVImageBase, Scalar(RedLowH, RedLowS, RedLowV), Scalar(RedHighH, RedHighS, RedHighV), gThresholdImage[1]); 

		dilate(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		dilate(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		
		erode(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		erode(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);

		//Threshold the HSV image - YELLOW COLOR
		inRange(hHSVImageBase, Scalar(YellowLowH, YellowLowS, YellowLowV), Scalar(YellowHighH, YellowHighS, YellowHighV), gThresholdImage[2]); 

		dilate(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		dilate(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		
		erode(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		erode(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		
		//Show results of thresholding
		imshow("Thresholded image - BLUE SIGNS", gThresholdImage[0]); 
		imshow("Thresholded image - RED SIGNS", gThresholdImage[1]); 
		imshow("Thresholded image - YELLOW SIGNS", gThresholdImage[2]); 


		if(waitKey(10) == 27) break;
	}
    return 0;
}

void CreateGUI(void)
{
	//Create windows 
	namedWindow("Original image", WINDOW_AUTOSIZE); 
	namedWindow("Histogram eqaulization", WINDOW_AUTOSIZE);
	namedWindow("Thresholded image - BLUE SIGNS", WINDOW_AUTOSIZE); 
	namedWindow("Thresholded image - RED SIGNS", WINDOW_AUTOSIZE); 
	namedWindow("Thresholded image - YELLOW SIGNS", WINDOW_AUTOSIZE); 
};