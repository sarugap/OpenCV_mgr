#include "main.h"

bool bSuccess;

vector<vector<Point> > contours_B,
					   contours_R,
					   contours_Y;

vector<Vec4i> hierarchy_B,
			  hierarchy_R,
			  hierarchy_Y;

RNG rng(12345);

float stosunekHW = 0.0;


int main(int argc, char** argv)
{
	//Load image
/*	gOrginalImage = imread(argv[1]);

	if(gOrginalImage.empty())
	{
		cout <<"Invalid path to image!" <<endl;
		getchar();
		return -1;
	}
*/

	VideoCapture cap(argv[1]); //capture the video from web cam

	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	cap.read(gOrginalImage);

	CreateGUI();

	//StructuringElement - erode and dilate
	gStructElement3_3 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1,1));
	gStructElement5_5 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1,1));

	//Resize the original image
	H_res = gOrginalImage.rows;
	W_res = gOrginalImage.cols;
	//resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));
	
	while(1)
	{
		bSuccess = cap.read(gOrginalImage); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
		cout << "Cannot read a frame from video stream" << endl;
		break;
		}

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
		
		// Find contours
		findContours(gThresholdImage[0], contours_B, hierarchy_B, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//BLUE
		findContours(gThresholdImage[1], contours_R, hierarchy_R, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//RED
		findContours(gThresholdImage[2], contours_Y, hierarchy_Y, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//YELLOW
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		vector<vector<Point> > contours_poly_B(contours_B.size()),
							   contours_poly_R(contours_R.size()),
							   contours_poly_Y(contours_Y.size());

		vector<Rect> boundRect_B(contours_B.size()),
					 boundRect_R(contours_R.size()),
					 boundRect_Y(contours_Y.size());
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		for(int i = 0; i < contours_B.size(); i++)
		{ 
			approxPolyDP(Mat(contours_B[i]), contours_poly_B[i], 3, true);
			boundRect_B[i] = boundingRect( Mat(contours_poly_B[i]) );
			//minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
		}

		// Draw polygonal contour + bonding rects + circles
		Mat drawing = Mat::zeros(gThresholdImage[0].size(), CV_8UC3);
		
		//Show results of thresholding
		imshow("Thresholded image - BLUE SIGNS", gThresholdImage[0]); 
		imshow("Thresholded image - RED SIGNS", gThresholdImage[1]); 
		imshow("Thresholded image - YELLOW SIGNS", gThresholdImage[2]); 

		for( int i = 0; i< contours_B.size(); i++ )
		{
			stosunekHW = boundRect_B[i].width/boundRect_B[i].height;
			//if(contours_poly[i].size() == 3)
			if((boundRect_B[i].width > 20)&&(boundRect_B[i].height > 20)&&(boundRect_B[i].width < 100)&&(boundRect_B[i].height < 100)&&(0.95 <= stosunekHW)&&(stosunekHW <= 1.05)&&(contours_poly_B[i].size() >= 3))
			{
				Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
				drawContours( drawing, contours_poly_B, i, color, 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(drawing, boundRect_B[i].tl(), boundRect_B[i].br(), color, 2, 8, 0);
				//circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
			}
		}

		namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
		imshow( "Contours", drawing );


		if(waitKey(10) == 27) break;
	}
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
};