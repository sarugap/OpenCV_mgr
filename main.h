#ifndef MAIN_H
#define MAIN_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>

#define MSER_RED					0
#define MSER_BLUE					1
#define MSER_YELLOW					2
#define MSER_WHITE					3
#define MSER_BLACK					4

using namespace std;
using namespace cv;

struct _tMSER
{
	unsigned int DELTA;
	unsigned int MIN_AREA;
	unsigned int MAX_AREA;
	float VARIATION;
};

#endif