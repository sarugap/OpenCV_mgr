#include "main.h"
#include "matlab_fcn.h"
#include "gui.h"
#include <algorithm>
#include <Windows.h>

#define SIMPLE_IMG					0

fstream fOriginalImg, fB_channel, fG_channel, fR_channel;

unsigned int flMserColor			= 0;

_tMSER MSERred, MSERblue, MSERyellow, MSERwhite, MSERblack;

char file_path[256], tempString[100], mser_param[50];
char file_out_name[10], file_name[10], file_path_in[256], file_path_out[256], string_temp[256];

Mat  gOrginalImage,
		gGrayImage,
		gColor_dst,
		gBGRImage[3],
		gR_ch_main,
		gG_ch_main,
		gB_ch_main,
		gR_channel,
		gG_channel,
		gB_channel,
		gW_channel,
		gK_channel, //czarny kanal
		gY_channel,
		temp[3];

//Praca na wielu obrazach

#if (!SIMPLE_IMG)

unsigned int file_number;

int main(int argc, char** argv)
{
	MSERred.DELTA				= 12;
	MSERred.MIN_AREA			= 1000;
	MSERred.MAX_AREA			= 15000;
	MSERred.VARIATION			= 0.3;

	MSERblue.DELTA				= 7;
	MSERblue.MIN_AREA			= 1000;
	MSERblue.MAX_AREA			= 15000;
	MSERblue.VARIATION			= 0.3;

	MSERyellow.DELTA			= 12;
	MSERyellow.MIN_AREA			= 1000;
	MSERyellow.MAX_AREA			= 15000;
	MSERyellow.VARIATION		= 0.3;

	MSERwhite.DELTA				= 7;	
	MSERwhite.MIN_AREA			= 1000;
	MSERwhite.MAX_AREA			= 15000;
	MSERwhite.VARIATION			= 0.3;

	MSERblack.DELTA				= 7;
	MSERblack.MIN_AREA			= 1000;
	MSERblack.MAX_AREA			= 15000;
	MSERblack.VARIATION			= 0.3;

	CreateGui();
		
	memcpy(string_temp, file_path_in, sizeof(file_path_in));

	file_number = 1;

	while(!SIMPLE_IMG)
	{
		memset(file_path_in, 0, sizeof(file_path_in));
		sprintf(file_path_in, "%s%c%s(%d).jpg", string_temp, 92, file_name, file_number);
//		sprintf(file_path_in, "%s%c%s (%d).jpg", string_temp, 92, file_name, file_number);

		cout << file_path_in << endl;

		//Przycinanie obrazu
		//ImCropp(gOrginalImage, gOrginalImage, Top_X, Top_Y, Bottom_X, Bottom_Y);

		gOrginalImage = imread(file_path_in);

		if(gOrginalImage.empty())
		{
			cout <<"Invalid path to image!" <<endl;
			return -1;	
		}

		//Konwersja obrazu
		gOrginalImage.convertTo(gOrginalImage, CV_32FC3); 
		gOrginalImage = gOrginalImage/255;

		//Rozdzielenie kanalow B, G, R
		split(gOrginalImage, gBGRImage);
	
		ImAdjust(gBGRImage[2], gR_ch_main, 0.3, 1, 0, 1);
		ImAdjust(gBGRImage[1], gG_ch_main, 0.5, 1, 0, 1);
		ImAdjust(gBGRImage[0], gB_ch_main, 0.3, 1, 0, 1);

		//CZERWONE ZNAKI	[img_red = r_h.*(1-g).*(1-b)]
		gR_channel = gR_ch_main.mul(1-gBGRImage[1]);
		gR_channel = gR_channel.mul(1-gBGRImage[0]);

		//NIEBIESKIE ZNAKI	[img_blue = b_h.*(1-r).*(1-g)]
		gB_channel = gB_ch_main.mul(1-gBGRImage[2]);
		gB_channel = gB_channel.mul(1-gBGRImage[1]);

		//ZOLTE ZNAKI		[img_yellow = r_h.*g.*(1-b)]
		gY_channel = gR_ch_main.mul(gBGRImage[1]);
		gY_channel = gY_channel.mul(1-gBGRImage[0]);

		//BIALE ZNAKI		[img_white = r_h.*b_h.*g_h]
		gW_channel = gR_ch_main.mul(gB_ch_main);
		gW_channel = gW_channel.mul(gG_ch_main);

		//CZARNE ZNAKI		[img_black = (1-r).*(1-g).*(1-b)]
		gK_channel = 1 - gBGRImage[2];
		gK_channel = gK_channel.mul(1 - gBGRImage[1]);
		gK_channel = gK_channel.mul(1 - gBGRImage[0]);

		cout << "--------------------------MSER---------------------------" << endl;
	
		//Konwersja obrazow
		gR_channel.convertTo(gR_channel, CV_8UC1, 255);
		gY_channel.convertTo(gY_channel, CV_8UC1, 255);
		gB_channel.convertTo(gB_channel, CV_8UC1, 255);
		gW_channel.convertTo(gW_channel, CV_8UC1, 255);
		gK_channel.convertTo(gK_channel, CV_8UC1, 255);
	
		temp[0].zeros(gR_channel.rows, gR_channel.cols, CV_8UC1);
		temp[0] = gR_channel;

		temp[1].zeros(gB_channel.rows, gB_channel.cols, CV_8UC1);
		temp[1] = gB_channel;

		gOrginalImage = gOrginalImage * 255;
		gOrginalImage.convertTo(gOrginalImage, CV_8UC3); 

#if 1
		//Obraz pomocniczy do wycinania
		int w(gR_channel.cols);
		int h(gR_channel.rows);
		Mat dst(5*w, 5*h, CV_8UC3);
		
		gOrginalImage.copyTo(dst(Rect(0, 0, w, h)));

		switch(flMserColor)
		{
			case MSER_RED:
			
				fcMSER(MSERred, gR_channel, dst);

				//Zapis obrazow na dysk
				cout << "--------------------------WRITE IMAGE--------------------" << endl;

				memset(file_path_out, 0, sizeof(file_path_out));
				sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/RED/%d_mser_red.jpg", file_number);
				imwrite(file_path_out, dst);
				break;

			case MSER_BLUE:
				
				fcMSER(MSERblue, gB_channel, dst);

				//Zapis obrazow na dysk
				cout << "--------------------------WRITE IMAGE--------------------" << endl;

				memset(file_path_out, 0, sizeof(file_path_out));
				sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/BLUE/%d_mser_blue.jpg", file_number);
				imwrite(file_path_out, dst);
				break;

			case MSER_YELLOW:
				
				fcMSER(MSERyellow, gY_channel, dst);

				//Zapis obrazow na dysk
				cout << "--------------------------WRITE IMAGE--------------------" << endl;

				memset(file_path_out, 0, sizeof(file_path_out));
				sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/YELLOW/%d_mser_yellow.jpg", file_number);
				imwrite(file_path_out, dst);
				break;

			case MSER_WHITE:
								
				fcMSER(MSERwhite, gW_channel, dst);

				//Zapis obrazow na dysk
				cout << "--------------------------WRITE IMAGE--------------------" << endl;

				memset(file_path_out, 0, sizeof(file_path_out));
				sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/WHITE/%d_mser_white.jpg", file_number);
				imwrite(file_path_out, dst);
				break;

			case MSER_BLACK:
				
				fcMSER(MSERblack, gK_channel, dst);

				//Zapis obrazow na dysk
				cout << "--------------------------WRITE IMAGE--------------------" << endl;

				memset(file_path_out, 0, sizeof(file_path_out));
				sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/BLACK/%d_mser_black.jpg", file_number);
				imwrite(file_path_out, dst);
				break;
		}
#endif

#if 0
		cout << "--------------------------WRITE IMAGE--------------------" << endl;

		//Zapis obrazow na dysk

		memset(file_path_out, 0, sizeof(file_path_out));
		sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/BLUE/%d_mser_blue.jpg", file_number);
		//imwrite(file_path_out, gR_channel);
		imwrite(file_path_out, dst);
/*
		memset(file_path, 0, sizeof(file_path));
		sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Yellow.jpg");
		imwrite(file_path, gY_channel);

		memset(file_path, 0, sizeof(file_path));
		sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Blue.jpg");
		imwrite(file_path, gB_channel);

		memset(file_path, 0, sizeof(file_path));
		sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/White.jpg");
		imwrite(file_path, gW_channel);

		memset(file_path, 0, sizeof(file_path));
		sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Black.jpg");
		imwrite(file_path, gK_channel);
*/
#endif

		gOrginalImage.release();

		gBGRImage[0].release();
		gBGRImage[1].release();
		gBGRImage[2].release();

		gR_channel.release();
		gB_channel.release();
		gY_channel.release();
		gW_channel.release();

		dst.release();

		file_number++;

	}

	cout << "--------------------------END----------------------------" << endl;

    return 0;
}

#endif