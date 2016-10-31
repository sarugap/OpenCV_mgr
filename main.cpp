#include "main.h"

int main(int argc, char** argv)
{
	////Load image
	//gOrginalImage = imread(argv[1], CV_LOAD_IMAGE_COLOR); 

	//if(gOrginalImage.empty())
	//{
	//	cout <<"Invalid path to image!" <<endl;
	//	getchar();
	//	return -1;
	//}

	//CreateGUI();

	VideoCapture cap(0); //capture the video from web cam

	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	
	namedWindow("Test", WINDOW_AUTOSIZE); 

	bool bSuccess;

	while(1)
	{
		bSuccess = cap.read(gOrginalImage); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		split(gOrginalImage, gRGBImage);
		//Histogram equalization
		equalizeHist(gRGBImage[0], gEqualizationHist[0]);
		equalizeHist(gRGBImage[1], gEqualizationHist[1]);
		equalizeHist(gRGBImage[2], gEqualizationHist[2]);

		//Merge 3 matrix into 1 matrix
		merge(gEqualizationHist, 3, gRGBImage[0]);

		Convert2RB(gRGBImage[0], tempMat);
		imshow("Test", tempMat); 

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

void Convert2RB(Mat& BGR_Img,  Mat& RB_img)
{
	unsigned int i, j;
	float b, g, r, temp_float,
				   temp_float2;
	Mat layers[3];

	RB_img = Mat::zeros(BGR_Img.rows, BGR_Img.cols, CV_8UC1);

	split(BGR_Img, layers);

	for(i = 0; i < BGR_Img.rows; i++)
	{
		for(j = 0; j < BGR_Img.cols; j++)
		{

			b = layers[0].data[ i * layers[0].cols + j];
			g = layers[1].data[ i * layers[1].cols + j];
			r = layers[2].data[ i * layers[2].cols + j];

			temp_float2 = b+g+r;

			temp_float = max((r/temp_float2),(b/temp_float2));

			RB_img.data[ i * BGR_Img.cols + j] = (unsigned int)(temp_float*100);
		}
	}
}