#ifndef NMATLAB_FCN_H
#define NMATLAB_FCN_H

#include <opencv2/imgproc/imgproc.hpp>
#include "main.h"

#define Top_X			1021//320
#define Top_Y			617//170
#define Bottom_X		5285//1315
#define Bottom_Y		1990//470

void ImAdjust(Mat& SRC, Mat& DST, float x1, float x2, float y1, float y2);
void ImCropp(Mat& SRC, Mat& DST, unsigned int TopX, unsigned int TopY, unsigned int BottomX, unsigned int BottomY);
void fcMSER(_tMSER MSERx, Mat& SRC, Mat& DST);

#endif