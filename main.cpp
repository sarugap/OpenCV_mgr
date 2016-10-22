//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* );

int main( int, char** argv )
{
  src = imread(argv[1]);

  if (src.empty())
  {
    cerr << "No image supplied ..." << endl;
    return -1;
  }

  //resize(src, src, Size(src.cols/4, src.rows/4));

  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  const char* source_window = "Source";

  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );
  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );
  waitKey(0);
  return(0);
}
void thresh_callback(int, void* )
{
	Mat canny_output, drawing, approx_edges; 
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(src_gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point>> contours_poly(contours.size());

	drawing = Mat::zeros( src_gray.size(), CV_8UC3 );
	approx_edges = Mat::zeros( src_gray.size(), CV_8UC3 );

	for(size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	for(size_t i = 0; i< contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 5, true);
	}

	for(size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		drawContours(approx_edges, contours_poly, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	namedWindow( "Contours", WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );

	namedWindow( "Approx", WINDOW_AUTOSIZE );
	imshow( "Approx", approx_edges );
}