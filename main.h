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