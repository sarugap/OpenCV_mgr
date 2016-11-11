#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
<<<<<<< HEAD
#include <fstream>
=======
>>>>>>> refs/remotes/origin/master

#define ImageResizeCoef 4

using namespace cv;
using namespace std;

void CreateGUI(void);
<<<<<<< HEAD
void Convert2RB(Mat& Input,  Mat& Output);
void EqualizeHist(Mat& Input,  Mat& Output);
=======
>>>>>>> refs/remotes/origin/master

Mat gOrginalImage,
	gGreyImage,
	gGaussBlurImage,
	gCannyEdgeImage,
	gColor_dst,
	hHSVImageBase,
<<<<<<< HEAD
	gBGRImage[3],
	gR_channel,
	gG_channel,
	gB_channel,
	gC_channel,
	gM_channel,
	gY_channel,
	gREDv_channel,
	gBLUEv_channel,
	gYELLOWv_channel,
	gWHITEv_channel,
	gHSVImage[3],
	gEqualizationHist[3],
	gThresholdImage[4],
	gStructElement3_3,
	gStructElement5_5,
	tempMat;
//-----------------------BLUE COLOR-----------------------\\		
///* H range [0-179]*/
//int BlueLowH = 86; //94
//int BlueHighH = 122;
//
///* S range [0-255]*/
//int BlueLowS = 140; 
//int BlueHighS = 255;
//
///* V range [0-255]*/
//int BlueLowV = 92; 
//int BlueHighV = 232;

/* H range [0-179]*/
int BlueLowH = 70;
int BlueHighH = 130;

/* S range [0-255]*/
int BlueLowS = 0;//120 
=======
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
>>>>>>> refs/remotes/origin/master
int BlueHighS = 255;

/* V range [0-255]*/
int BlueLowV = 92; 
int BlueHighV = 232;
<<<<<<< HEAD

//-----------------------RED COLOR-----------------------\\		
/* H range [0-179]*/
int RedLowH = 120;//120
=======
//-----------------------RED COLOR-----------------------\\		
/* H range [0-179]*/
int RedLowH = 120;
>>>>>>> refs/remotes/origin/master
int RedHighH = 179;

/* S range [0-255]*/
int RedLowS = 15; 
int RedHighS = 255;

/* V range [0-255]*/
int RedLowV = 170; //124 
int RedHighV = 255;	
//-----------------------YELLOW COLOR-----------------------\\		
/* H range [0-179]*/
<<<<<<< HEAD
int YellowLowH = 10;//14
int YellowHighH = 70;//60

/* S range [0-255]*/
int YellowLowS = 0;//70 
=======
int YellowLowH = 14;
int YellowHighH = 60;

/* S range [0-255]*/
int YellowLowS = 70; 
>>>>>>> refs/remotes/origin/master
int YellowHighS = 255;

/* V range [0-255]*/
int YellowLowV = 131;
int YellowHighV = 255;
<<<<<<< HEAD
//-----------------------WHITE COLOR-----------------------\\		
/* H range [0-179]*/
int WhiteLowH = 0;
int WhiteHighH = 180;

/* S range [0-255]*/
int WhiteLowS = 0; 
int WhiteHighS = 70;

/* V range [0-255]*/
int WhiteLowV = 220;
int WhiteHighV = 255;
//---------------------------------------------------------\\	

=======
//---------------------------------------------------------\\	
>>>>>>> refs/remotes/origin/master
/* Image resolution */
int H_res;
int W_res;

/* Median filter */
unsigned int MedFilter = 5;