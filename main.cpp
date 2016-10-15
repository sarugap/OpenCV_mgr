#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>

#define ImageResizeCoef 4

using namespace cv;
using namespace std;

void CreateGUI(void);

Mat	element;

Mat gOrginalImage,
	gGreyImage,
	gGaussBlurImage,
	gCannyEdgeImage,
	gColor_dst,
	hHSVImageBase,
	gRGBImage[3],
	gHSVImage[3],
	gEqualizationHist[3],
	gThresholdImage[3];
		
/* H range [0-179]*/
int iLowH = 98;
int iHighH = 122;

/* S range [0-255]*/
int iLowS = 140; 
int iHighS = 255;

/* V range [0-255]*/
int iLowV = 115; 
int iHighV = 232;

/* Image resolution */
int H_res;
int W_res;

/* Median filter */
unsigned int MedFilter = 3;
unsigned int StartStop = 0;
unsigned int temp_pause  = 0;

int main( int argc, char** argv )
{
	CreateGUI();

	//Load image
	gOrginalImage = imread("D:/Rozpoznawanie znakow drogowych/TSR-zdjecia/IMG_20160914_175427.jpg");

	if(gOrginalImage.empty())
	{
		cout <<"Invalid path to image!" <<endl;
		getchar();
		return -1;
	}

	//Resize the original image
	H_res = gOrginalImage.rows;
	W_res = gOrginalImage.cols;
	resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));
	imshow("Original image", gOrginalImage); 

	//Image preprocesing
	split(gOrginalImage,gRGBImage);

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

	while(1)
	{
		//Threshold the HSV image
		inRange(hHSVImageBase, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), gThresholdImage[0]); 
		imshow("Thresholded image", gThresholdImage[0]); 

		if(waitKey(10) == 27) break;
	}
    return 0;
}

void CreateGUI(void)
{
	//Create windows 
	namedWindow("Original image", WINDOW_AUTOSIZE); 
	namedWindow("Histogram eqaulization", WINDOW_AUTOSIZE);
	namedWindow("Thresholded image", WINDOW_AUTOSIZE); 
	//namedWindow("Median filter image", WINDOW_AUTOSIZE); 
	namedWindow("Control", CV_WINDOW_AUTOSIZE);

	//Create trackbars
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
};