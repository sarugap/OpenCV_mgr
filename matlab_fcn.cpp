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
};

void ImCropp(Mat& SRC, Mat& DST, unsigned int TopX, unsigned int TopY, unsigned int BottomX, unsigned int BottomY)
{
	DST = SRC(Rect(TopX, TopY, labs(BottomX - TopX), labs(BottomY - TopY))).clone();
};

void fcMSER(_tMSER MSERx, Mat& SRC, Mat& DST)
{
	RNG rng(12345);
	Scalar color;
	char tempString[100];
	int w(SRC.cols);
	int h(SRC.rows);

	vector<vector<Point>> regions_x;
	vector<Point> temp_vect;


	cv::MSER ms_x(MSERx.DELTA, MSERx.MIN_AREA, MSERx.MAX_AREA, MSERx.VARIATION);
	ms_x(SRC, regions_x, Mat());

	cvtColor(SRC, SRC, COLOR_GRAY2BGR);

	for (unsigned int i = 0; i < regions_x.size(); i++)
	{
		color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
		temp_vect = regions_x[i];

		for(unsigned int j =0; j < regions_x[i].size() ; j++)
		{
			circle(SRC, temp_vect[j], 1,  color, -1, 8);
		}
	}

	SRC.copyTo(DST(Rect(0, h, w, h)));
	ImCropp(DST, DST, 0, 0, w, 2*h);

	putText(DST, "Original Image", Point2f(50,50), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
	sprintf(tempString, "MSER(delta= %d, min_area= %d, max_area= %d, max_variation= %0.2f)", MSERx.DELTA, MSERx.MIN_AREA, MSERx.MAX_AREA, MSERx.VARIATION);
	putText(DST, tempString, Point2f(50,50+h), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
};