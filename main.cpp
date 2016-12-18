#include "main.h"
#include <algorithm>

char file_path[512];

fstream fOriginalImg, fB_channel, fG_channel, fR_channel;

void imadjust(Mat& SRC, Mat& DST, float x1, float x2, float y1, float y2);

int main(int argc, char** argv)
{
	cout << "--------------------------START--------------------------" << endl;
	fB_channel.open("fB_channel.txt", std::ios::out);
	fG_channel.open("fG_channel.txt", std::ios::out);
	fR_channel.open("fR_channel.txt", std::ios::out);
	fOriginalImg.open("gOriginal.txt", std::ios::out);

	gOrginalImage = imread(argv[1]);

	if(gOrginalImage.empty())
	{
		cout <<"Invalid path to image!" <<endl;
		getchar();
		return -1;
	}

	//Resize the original image
	H_res = gOrginalImage.rows;
	W_res = gOrginalImage.cols;
	//resize(gOrginalImage, gOrginalImage, Size(W_res/ImageResizeCoef, H_res/ImageResizeCoef));

	//Covert to float image
	gOrginalImage.convertTo(gOrginalImage, CV_32FC3); 
	gOrginalImage = gOrginalImage/255;

	//Image preprocesing
	split(gOrginalImage, gBGRImage);

	
	imadjust(gBGRImage[2], gR_ch_main, 0.5, 1, 0, 1);
	imadjust(gBGRImage[1], gG_ch_main, 0.5, 1, 0, 1);
	imadjust(gBGRImage[0], gB_ch_main, 0.5, 1, 0, 1);
	
	//RED TRAFFIC SIGNS		[r_h.*(1-g).*(1-b)]
	//gR_channel = gR_channel*(1-gBGRImage[0])*(1-gB_channel)
/*
	temp[0] = 1-gBGRImage[1];
	gR_channel = gR_ch_main.mul(temp[0]);
	temp[0] = 1-gBGRImage[0];
	gR_channel = gR_channel.mul(temp[0]);
*/

	//BLUE TRAFFIC SIGNS	[b_h.*(1-r).*(1-g)]
/*
	temp[0] = 1-gBGRImage[1];
	temp[1] = 1-gBGRImage[2];
	temp[2] = gB_ch_main.mul(temp[1]);
	gB_channel = temp[1].mul(temp[0]);
*/	

	//YELLOW TRAFFIC SIGNS		[r_h.*g.*(1-b)]
	//gY_channel = gR_ch_main*gBGRImage[1]*(1-gBGRImage[0])
	
	//temp[0] = gR_ch_main.mul(gBGRImage[1]);
	//temp[1] = 1-gBGRImage[0];
	////gY_channel = gR_ch_main.mul(gBGRImage[1]);
	//gY_channel = gBGRImage[1].mul(temp[1]);
	
	/*
	temp[0] = 1-gBGRImage[0];
	gY_channel = gR_ch_main.mul(gBGRImage[1].mul(temp[0]));
	*/
	//WHITE TRAFFIC SIGNS
	

	//BLACK TRAFFIC SIGNS


	cout << "--------------------------WRITE TO FILE------------------" << endl;
	fB_channel << gBGRImage[0];
	fG_channel << gBGRImage[1];
	fR_channel << gBGRImage[2];
	//fR_channel << gR_ch_main.mul(gBGRImage[1]); 
	fOriginalImg << gR_ch_main;

	cout << "--------------------------WRITE IMAGE--------------------" << endl;

	//Convert to unsigned int
	gR_channel.convertTo(gR_channel, CV_8UC1, 255);
	gY_channel.convertTo(gY_channel, CV_8UC1, 255);
	gB_channel.convertTo(gB_channel, CV_8UC1, 255);
	gG_channel.convertTo(gG_channel, CV_8UC1, 255);
	
	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Red.jpg");
	//imwrite(file_path, gR_ch_main*255);
	imwrite(file_path, gR_ch_main*255);
/*
	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Yellow.jpg");
	imwrite(file_path, gY_channel);
*/
	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Blue.jpg");
	imwrite(file_path, gB_ch_main*255);

	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Green.jpg");
	imwrite(file_path, gG_ch_main*255);
/*
	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Black.jpg");
	imwrite(file_path, gK_channel);
*/
	cout << "--------------------------END----------------------------" << endl;
	
	fB_channel.close();
	fG_channel.close();
	fR_channel.close();
	fOriginalImg.close();

    return 0;
}

void imadjust(Mat& SRC, Mat& DST, float x1, float x2, float y1, float y2) 
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