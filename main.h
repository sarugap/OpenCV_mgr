#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

#define ImageResizeCoef 4

using namespace cv;
using namespace std;

Mat gOrginalImage,
	gGrayImage,
	gGaussBlurImage,
	gCannyEdgeImage,
	gColor_dst,
	hHSVImageBase,
	hHSLImageBase,
	gBGRImage[3],
	gR_ch_main,
	gG_ch_main,
	gB_ch_main,
	gR_channel,
	gG_channel,
	gB_channel,
	gW_channel,
	gK_channel, //black
	gY_channel,
	temp[3];

/* Image resolution */
int H_res;
int W_res;