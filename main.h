#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

#define ImageResizeCoef 4

using namespace cv;
using namespace std;

void CreateGUI(void);
void Convert2RB(Mat& Input,  Mat& Output);
void EqualizeHist(Mat& Input,  Mat& Output);

struct ImgStruct
{
	Mat HSV_channel[3];
	Mat BGR_channel[3];
	Mat CMY_channel[3];
	Mat H_channel;
	Mat S_channel;
	Mat V_channel;
	Mat HSV_thr[3];
	Mat temp[3];
};

ImgStruct RedImg, BlueImg, YellowImg, WhiteImg;

Mat gOrginalImage,
	gGrayImage,
	gGaussBlurImage,
	gCannyEdgeImage,
	gColor_dst,
	hHSVImageBase,
	hHSLImageBase,
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
	gHSLImage[3],
	gEqualizationHist[3],
	gThresholdImage[4],
	gStructElement3_3,
	gStructElement5_5,
	tempMat;
//-----------------------BLUE COLOR-----------------------\\		
/* H range [0-179]*/
int BlueLowH = 90;//100
int BlueHighH = 140;

/* S range [0-255]*/
int BlueLowS = 110;//120 //65 //90
int BlueHighS = 255;

/* V range [0-255]*/
int BlueLowV = 40; 
int BlueHighV = 250; //230

//-----------------------RED COLOR-----------------------\\		
/* H range [0-179]*/
int RedLowH1 = 0;//120
int RedHighH1 = 15;

/* H range [0-179]*/
int RedLowH2 = 170;//120
int RedHighH2 = 179;

/* S range [0-255]*/
int RedLowS = 120; 
int RedHighS = 255;

/* V range [0-255]*/
int RedLowV = 65; //124 
int RedHighV = 250;	

//-----------------------YELLOW COLOR-----------------------\\		
/* H range [0-179]*/
int YellowLowH = 5;//14
int YellowHighH = 30;//60//25

/* S range [0-255]*/
int YellowLowS = 60;//70 
int YellowHighS = 255;

/* V range [0-255]*/
int YellowLowV = 70;//110
int YellowHighV = 255;
//-----------------------WHITE COLOR-----------------------\\		
/* H range [0-179]*/
int WhiteLowH = 0;
int WhiteHighH = 180;

/* S range [0-255]*/
int WhiteLowS = 80; 
int WhiteHighS = 255;

/* V range [0-255]*/
int WhiteLowV = 0;
int WhiteHighV = 50;
//---------------------------------------------------------\\	

/* Image resolution */
int H_res;
int W_res;

/* Median filter */
unsigned int MedFilter = 5;