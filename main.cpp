#include "main.h"

#define __Image 1
#define __Camera 0

bool bSuccess;


int main(int argc, char** argv)
{
	#if __Image
	
		gOrginalImage = imread(argv[1]);

		if(gOrginalImage.empty())
		{
			cout <<"Invalid path to image!" <<endl;
			getchar();
			return -1;
		}
	
	#else
		#if __Camera
			VideoCapture cap(0); //capture the video from web cam
		#else
			VideoCapture cap(argv[1]); 
		#endif

		if ( !cap.isOpened() )  // if not success, exit program
		{
			cout << "Cannot open the web cam" << endl;
			return -1;
		}

		cap.read(gOrginalImage);
	
	#endif

	CreateGUI();

	#if __Image

		//Resize the original image
		H_res = gOrginalImage.rows;
		W_res = gOrginalImage.cols;
		resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));

	#endif

	while(1)
	{
		#if (!__Image)

			bSuccess = cap.read(gOrginalImage); // read a new frame from video

			if (!bSuccess) //if not success, break loop
			{
			cout << "Cannot read a frame from video stream" << endl;
			break;
			}

		#endif

		imshow("Original image", gOrginalImage); 

		//Image preprocesing
		split(gOrginalImage, gBGRImage);

		cvtColor(gOrginalImage, hHSVImageBase, CV_BGR2HSV);
		split(hHSVImageBase, gHSVImage);

		BlueImg.BGR_channel[0] = gBGRImage[0];
		BlueImg.BGR_channel[1] = gBGRImage[1];
		BlueImg.BGR_channel[2] = gBGRImage[2];

		//RGB -> CMYK
		//BlueImg.CMY_channel[0] = 255 - BlueImg.BGR_channel[2];
		//BlueImg.CMY_channel[1] = 255 - BlueImg.BGR_channel[1];
		//BlueImg.CMY_channel[2] = 255 - BlueImg.BGR_channel[0];

		//BLUE TRAFFIC SIGNS
		inRange(gHSVImage[0], BlueLowH, BlueHighH, BlueImg.HSV_thr[0]); 
		inRange(gHSVImage[1], BlueLowS, BlueHighS, BlueImg.HSV_thr[1]); 
		inRange(gHSVImage[2], BlueLowV, BlueHighV, BlueImg.HSV_thr[2]); 

		//adaptiveThreshold(gHSVImage[0], BlueImg.temp[0], 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 5);

		BlueImg.temp[0] = ((BlueImg.HSV_thr[0] + BlueImg.HSV_thr[1])/2 + BlueImg.HSV_thr[2])/2;

		//YELLOW TRAFFIC SIGNS
		inRange(gHSVImage[0], YellowLowH, YellowHighH, YellowImg.HSV_thr[0]); 
		inRange(gHSVImage[1], YellowLowS, YellowHighS, YellowImg.HSV_thr[1]); 
		inRange(gHSVImage[2], YellowLowV, YellowHighV, YellowImg.HSV_thr[2]); 

		YellowImg.temp[0] = ((YellowImg.HSV_thr[0] + YellowImg.HSV_thr[1])/2 + YellowImg.HSV_thr[2])/2;

		//imshow("BLUE - Hue threshold", BlueImg.HSV_thr[0]); 
		//imshow("BLUE - Saturation threshold", BlueImg.HSV_thr[1]); 
		//imshow("BLUE - Value threshold", BlueImg.HSV_thr[2]); 
		////imshow("BLUE - BGR, B channel", BlueImg.BGR_channel[0]); 
		////imshow("BLUE - CMY, C channel", BlueImg.CMY_channel[0]); 
		//imshow("BLUE - H+S+V/3", BlueImg.temp[0]); 


		imshow("YELLOW - Hue threshold", YellowImg.HSV_thr[0]); 
		imshow("YELLOW - Saturation threshold", YellowImg.HSV_thr[1]); 
		imshow("YELLOW - Value threshold", YellowImg.HSV_thr[2]); 
		imshow("YELLOW - H+S+V/3", YellowImg.temp[0]); 


		if(waitKey(100) == 27)
			{
				break;
			}
	}
    return 0;
}

void CreateGUI(void)
{
	//Create windows 
	namedWindow("Original image", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - Hue threshold", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - Saturation threshold", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - Value threshold", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - BGR, B channel", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - CMY, C channel", WINDOW_AUTOSIZE); 
	//namedWindow("BLUE - H+S+V/3", WINDOW_AUTOSIZE); 
	
	namedWindow("YELLOW - Hue threshold", WINDOW_AUTOSIZE); 
	namedWindow("YELLOW - Saturation threshold", WINDOW_AUTOSIZE); 
	namedWindow("YELLOW - Value threshold", WINDOW_AUTOSIZE); 
	namedWindow("YELLOW - H+S+V/3", WINDOW_AUTOSIZE); 


	namedWindow("Control", WINDOW_AUTOSIZE); 
	
	cvCreateTrackbar("LowH", "Control", &YellowLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &YellowHighH, 179);
	cvCreateTrackbar("LowS", "Control", &YellowLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &YellowHighS, 255);
	cvCreateTrackbar("LowV", "Control", &YellowLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &YellowHighV, 255);

};