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
		cvtColor(gOrginalImage, gGrayImage, CV_BGR2GRAY);
		cvtColor(gOrginalImage, hHSLImageBase, CV_RGB2HLS);
		
		split(hHSVImageBase, gHSVImage);//HSV
		split(hHSLImageBase, gHSLImage);//HSL

		BlueImg.BGR_channel[0] = gBGRImage[0];
		BlueImg.BGR_channel[1] = gBGRImage[1];
		BlueImg.BGR_channel[2] = gBGRImage[2];

		//BLUE TRAFFIC SIGNS
		inRange(gHSVImage[0], BlueLowH, BlueHighH, BlueImg.HSV_thr[0]); 
		inRange(gHSVImage[1], BlueLowS, BlueHighS, BlueImg.HSV_thr[1]); 
		//inRange(gHSVImage[2], BlueLowV, BlueHighV, BlueImg.HSV_thr[2]); 

		BlueImg.temp[0] = (BlueImg.HSV_thr[0] + BlueImg.HSV_thr[1])/2;

		//YELLOW TRAFFIC SIGNS
		inRange(gHSVImage[0], YellowLowH, YellowHighH, YellowImg.HSV_thr[0]); 
		inRange(gHSVImage[1], YellowLowS, YellowHighS, YellowImg.HSV_thr[1]); 
		//inRange(gHSVImage[2], YellowLowV, YellowHighV, YellowImg.HSV_thr[2]); 

		YellowImg.temp[0] = (YellowImg.HSV_thr[0] + YellowImg.HSV_thr[1])/2;

		//RED TRAFFIC SIGNS
		inRange(gHSVImage[0], RedLowH1, RedHighH1, RedImg.HSV_thr[0]); 
		inRange(gHSVImage[0], RedLowH2, RedHighH2, RedImg.HSV_thr[2]); 
		inRange(gHSVImage[1], RedLowS, RedHighS, RedImg.HSV_thr[1]); 

		RedImg.HSV_thr[0] = RedImg.HSV_thr[0] + RedImg.HSV_thr[2];

		RedImg.temp[0] = (RedImg.HSV_thr[0] + RedImg.HSV_thr[1])/2;


		//WHITE TRAFFIC SIGNS
		//inRange(gHSVImage[0], WhiteLowH, WhiteHighH, WhiteImg.HSV_thr[0]); 
		//inRange(gHSVImage[1], WhiteLowS, WhiteHighS, WhiteImg.HSV_thr[1]); 
		//inRange(gHSVImage[2], WhiteLowV, WhiteHighV, WhiteImg.HSV_thr[2]); 
		
		inRange(gHSLImage[0], WhiteLowH, WhiteHighH, WhiteImg.HSV_thr[0]); 
		inRange(gHSLImage[1], WhiteLowS, WhiteHighS, WhiteImg.HSV_thr[1]); 
		inRange(gHSLImage[2], WhiteLowV, WhiteHighV, WhiteImg.HSV_thr[2]); 

		WhiteImg.temp[0] = (WhiteImg.HSV_thr[1] + WhiteImg.HSV_thr[2])/2; 
		//WhiteImg.temp[1] = gBGRImage[0] & gBGRImage[1] & gBGRImage[2];

		//imshow("BLUE - Hue threshold", BlueImg.HSV_thr[0]); 
		//imshow("BLUE - Saturation threshold", BlueImg.HSV_thr[1]); 
		//imshow("BLUE - Value threshold", BlueImg.HSV_thr[2]); 
		//imshow("BLUE - (H+S)/2", BlueImg.temp[0]); 

		//imshow("YELLOW - Hue threshold", YellowImg.HSV_thr[0]); 
		//imshow("YELLOW - Saturation threshold", YellowImg.HSV_thr[1]); 
		//imshow("YELLOW - Value threshold", YellowImg.HSV_thr[2]); 
		//imshow("YELLOW - (H+S)/2", YellowImg.temp[0]); 

		//imshow("RED - Hue threshold", RedImg.HSV_thr[0]); 
		//imshow("RED - Saturation threshold", RedImg.HSV_thr[1]); 
		//imshow("RED - Value threshold", RedImg.HSV_thr[2]); 
		//imshow("RED - (H+S)/2", RedImg.temp[0]); 

		imshow("White - Saturation threshold", WhiteImg.HSV_thr[1]); 
		imshow("White - Value threshold", WhiteImg.HSV_thr[2]); 
		imshow("White - (S+V)/2", WhiteImg.temp[0]); 
		//imshow("White - (S+V)/2", gGrayImage); 


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
	//namedWindow("BLUE - (H+S)/2", WINDOW_AUTOSIZE); 
	
	//namedWindow("YELLOW - Hue threshold", WINDOW_AUTOSIZE); 
	//namedWindow("YELLOW - Saturation threshold", WINDOW_AUTOSIZE); 
	//namedWindow("YELLOW - Value threshold", WINDOW_AUTOSIZE); 
	//namedWindow("YELLOW - (H+S)/2", WINDOW_AUTOSIZE); 

	//namedWindow("RED - Hue threshold", WINDOW_AUTOSIZE); 
	//namedWindow("RED - Saturation threshold", WINDOW_AUTOSIZE); 
	//namedWindow("RED - Value threshold", WINDOW_AUTOSIZE); 
	//namedWindow("RED - (H+S)/2", WINDOW_AUTOSIZE); 

	namedWindow("White - Saturation threshold", WINDOW_AUTOSIZE); 
	namedWindow("White - Value threshold", WINDOW_AUTOSIZE); 
	namedWindow("White - (S+V)/2", WINDOW_AUTOSIZE); 

	namedWindow("Control", WINDOW_AUTOSIZE); 
	
	//cvCreateTrackbar("LowH", "Control", &RedLowH1, 179); //Hue (0 - 179)
	//cvCreateTrackbar("HighH", "Control", &RedHighH1, 179);
	//cvCreateTrackbar("LowH2", "Control", &RedLowH2, 179); //Hue (0 - 179)
	//cvCreateTrackbar("HighH2", "Control", &RedHighH2, 179);
	//cvCreateTrackbar("LowS", "Control", &RedLowS, 255); //Saturation (0 - 255)
	//cvCreateTrackbar("HighS", "Control", &RedHighS, 255);
	//cvCreateTrackbar("LowV", "Control", &RedLowV, 255); //Value (0 - 255)
	//cvCreateTrackbar("HighV", "Control", &RedHighV, 255);

	cvCreateTrackbar("LowS", "Control", &WhiteLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &WhiteHighS, 255);
	cvCreateTrackbar("LowV", "Control", &WhiteLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &WhiteHighV, 255);
};