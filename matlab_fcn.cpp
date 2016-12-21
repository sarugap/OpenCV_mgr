#include "matlab_fcn.h"
#include <cstdlib>

void ImAdjust(Mat& SRC, Mat& DST, float x1, float x2, float y1, float y2) 
{
	float temp_a, temp_b;

	temp_a = (y2-y1)/(x2-x1);
	temp_b = (y1*x2-y2*x1)/(x2-x1);

	DST = temp_a * SRC + temp_b;
	
	DST = DST*255;

	DST.convertTo(DST, CV_8U); 
	DST.convertTo(DST, CV_32F);

	DST = DST/255;
}

void ImCropp(Mat& SRC, Mat& DST, unsigned int TopX, unsigned int TopY, unsigned int BottomX, unsigned int BottomY)
{
	DST = SRC(Rect(TopX, TopY, labs(BottomX - TopX), labs(BottomY - TopY))).clone();
}