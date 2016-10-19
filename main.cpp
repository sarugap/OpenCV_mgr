#include "main.h"

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
RNG rng(12345);

int main(int argc, char** argv)
{
	//Load image
	gOrginalImage = imread(*(argv+1));

	if(gOrginalImage.empty())
	{
		cout <<"Invalid path to image!" <<endl;
		getchar();
		return -1;
	}

	CreateGUI();

	//StructuringElement - erode and dilate
	gStructElement3_3 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1,1));
	gStructElement5_5 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1,1));

	//Resize the original image
	H_res = gOrginalImage.rows;
	W_res = gOrginalImage.cols;
	resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));
	imshow("Original image", gOrginalImage); 

	//Image preprocesing
	split(gOrginalImage, gRGBImage);

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

	//Blur H and S channel for HSV
	medianBlur(gHSVImage[0],gHSVImage[0],(MedFilter,MedFilter));
	medianBlur(gHSVImage[1],gHSVImage[1],(MedFilter,MedFilter));
	merge(gHSVImage, 3, hHSVImageBase);
	
//	while(1)
//	{
		//Threshold the HSV image - BLUE COLOR
		inRange(hHSVImageBase, Scalar(BlueLowH, BlueLowS, BlueLowV), Scalar(BlueHighH, BlueHighS, BlueHighV), gThresholdImage[0]); 

		dilate(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		dilate(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		
		erode(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);
		erode(gThresholdImage[0], gThresholdImage[0], gStructElement3_3);

		//Threshold the HSV image - RED COLOR
		inRange(hHSVImageBase, Scalar(RedLowH, RedLowS, RedLowV), Scalar(RedHighH, RedHighS, RedHighV), gThresholdImage[1]); 

		dilate(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		dilate(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		
		erode(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);
		erode(gThresholdImage[1], gThresholdImage[1], gStructElement3_3);

		//Threshold the HSV image - YELLOW COLOR
		inRange(hHSVImageBase, Scalar(YellowLowH, YellowLowS, YellowLowV), Scalar(YellowHighH, YellowHighS, YellowHighV), gThresholdImage[2]); 

		dilate(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		dilate(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		
		erode(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		erode(gThresholdImage[2], gThresholdImage[2], gStructElement3_3);
		
		//Show results of thresholding
		imshow("Thresholded image - BLUE SIGNS", gThresholdImage[0]); 
		imshow("Thresholded image - RED SIGNS", gThresholdImage[1]); 
		imshow("Thresholded image - YELLOW SIGNS", gThresholdImage[2]); 


		/// Canny detector
		//Canny(gThresholdImage[0], gThresholdImage[0], 0, 200, 3);
		/// Find contours
		findContours(gThresholdImage[0], contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		/// Approximate contours to polygons + get bounding rects and circles
		vector<vector<Point> > contours_poly( contours.size() );
		vector<Rect> boundRect( contours.size() );
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );

		for( int i = 0; i < contours.size(); i++ )
			{ approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
			}

		float stosunekHW = 0.0;

		/// Draw polygonal contour + bonding rects + circles
		Mat drawing = Mat::zeros( gThresholdImage[0].size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
			{
				stosunekHW = boundRect[i].width/boundRect[i].height;
				if((boundRect[i].width > 20)&&(boundRect[i].height > 20)&&(boundRect[i].width < 50)&&(boundRect[i].height < 50)&&(0.95 <= stosunekHW)&&(stosunekHW <= 1.05))
				//if((0.9 <= stosunekHW)&&(stosunekHW <= 1.0))
				{
					Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
					rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
					circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
				}
			}

		/// Show in a window
		namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
		imshow( "Contours", drawing );


		//imshow("Thresholded image - BLUE SIGNS - Edges", gThresholdImage[0]); 

//		if(waitKey(10) == 27) break;
//	}
		waitKey(10);
		getchar();
    return 0;
}

void CreateGUI(void)
{
	//Create windows 
	namedWindow("Original image", WINDOW_AUTOSIZE); 
	namedWindow("Histogram eqaulization", WINDOW_AUTOSIZE);
	namedWindow("Thresholded image - BLUE SIGNS", WINDOW_AUTOSIZE); 
	namedWindow("Thresholded image - RED SIGNS", WINDOW_AUTOSIZE); 
	namedWindow("Thresholded image - YELLOW SIGNS", WINDOW_AUTOSIZE); 
	namedWindow("Thresholded image - BLUE SIGNS - Edges", WINDOW_AUTOSIZE); 
};