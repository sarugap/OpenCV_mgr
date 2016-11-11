#include "main.h"

#define __Image 0

bool bSuccess;
fstream plik;

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
	
		//VideoCapture cap(0); //capture the video from web cam
		VideoCapture cap(argv[1]); 

		if ( !cap.isOpened() )  // if not success, exit program
		{
			cout << "Cannot open the web cam" << endl;
			return -1;
		}

		cap.read(gOrginalImage);
	
	#endif

	//CreateGUI();

	#if __Image

		//Resize the original image
		H_res = gOrginalImage.rows;
		W_res = gOrginalImage.cols;
		resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));

	#endif

	/**/
		plik.open("dane.txt", std::ios::out);

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

		gB_channel = gBGRImage[0];
		gG_channel = gBGRImage[1];
		gR_channel = gBGRImage[2];

		//RGB -> CMYK
		gC_channel = 255 - gR_channel;
		gM_channel = 255 - gG_channel;
		gY_channel = 255 - gB_channel;
		
		//Virtual channels
		gREDv_channel = (gR_channel + gM_channel)/2;
		gBLUEv_channel = (gB_channel + gC_channel)/2;
		gYELLOWv_channel = (gR_channel + gM_channel + gY_channel)/3;
		gWHITEv_channel = (gR_channel + gG_channel + gB_channel)/3;


	/*	imshow("R", gR_channel);
		imshow("G", gG_channel); 
		imshow("B", gB_channel); 
		imshow("C", gC_channel); 
		imshow("M", gM_channel); 
		imshow("Y", gY_channel); */
		imshow("RED", gREDv_channel); 
		imshow("BLUE", gBLUEv_channel); 
		imshow("YELLOW", gYELLOWv_channel); 
		imshow("WHITE", gWHITEv_channel); 



		if(waitKey(10) == 27)
			{
				//plik << gR_channel;
				plik.close();
				break;
			}
	}
    return 0;
}

void CreateGUI(void)
{
	//Create windows 
	namedWindow("Original image", WINDOW_AUTOSIZE); 
	//namedWindow("Histogram eqaulization", WINDOW_AUTOSIZE);
	//namedWindow("Thresholded image - BLUE SIGNS", WINDOW_AUTOSIZE); 
	//namedWindow("Thresholded image - RED SIGNS", WINDOW_AUTOSIZE); 
	//namedWindow("Thresholded image - YELLOW SIGNS", WINDOW_AUTOSIZE); 
	//namedWindow("Thresholded image - WHITE SIGNS", WINDOW_AUTOSIZE); 
/*	namedWindow("R", WINDOW_AUTOSIZE);
	namedWindow("G", WINDOW_AUTOSIZE); 
	namedWindow("B", WINDOW_AUTOSIZE); 
	namedWindow("C", WINDOW_AUTOSIZE); 
	namedWindow("M", WINDOW_AUTOSIZE); 
	namedWindow("Y", WINDOW_AUTOSIZE); */
	namedWindow("RED", WINDOW_AUTOSIZE); 
	namedWindow("BLUE", WINDOW_AUTOSIZE); 
	namedWindow("YELLOW", WINDOW_AUTOSIZE); 
	namedWindow("WHITE", WINDOW_AUTOSIZE); 
};