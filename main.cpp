#include "main.h"
#include "matlab_fcn.h"
#include <algorithm>

#define SIMPLE_IMG		0

#define MSER_DELTA			5
#define MSER_MIN_AREA		1000
#define MSER_MAX_AREA		15000
#define MSER_VARIATION		0.2

RNG rng(12345);
char file_path[256], tempString[100];
fstream fOriginalImg, fB_channel, fG_channel, fR_channel;

//Praca na pojedynczym obrazie
#if SIMPLE_IMG

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

	//Przycinanie obrazu
	//ImCropp(gOrginalImage, gOrginalImage, Top_X, Top_Y, Bottom_X, Bottom_Y);

	//Konwersja obrazu
	gOrginalImage.convertTo(gOrginalImage, CV_32FC3); 
	gOrginalImage = gOrginalImage/255;

	//Rozdzielenie kanalow B, G, R
	split(gOrginalImage, gBGRImage);
	
	ImAdjust(gBGRImage[2], gR_ch_main, 0.5, 1, 0, 1);
	ImAdjust(gBGRImage[1], gG_ch_main, 0.5, 1, 0, 1);
	ImAdjust(gBGRImage[0], gB_ch_main, 0.5, 1, 0, 1);
	

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
	gR_channel.convertTo(gR_channel, CV_8UC3, 255);
	gY_channel.convertTo(gY_channel, CV_8UC1, 255);
	gB_channel.convertTo(gB_channel, CV_8UC1, 255);
	gW_channel.convertTo(gW_channel, CV_8UC1, 255);
	gK_channel.convertTo(gK_channel, CV_8UC1, 255);

//	line( gR_channel, Point(0, 0), Point(100, 100), Scalar( 255, 0, 0 ), 2, 8 );
//	circle( gR_channel, Point(100, 100), 1, Scalar( 255, 0, 0 ), -1, 8);
	
//	MSER
	{
		MSER ms_red(MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION);

		vector<vector<Point>> regions_red;
		vector<Point> temp_vect;
		ms_red(gR_channel, regions_red, Mat());

		cvtColor(gR_channel, gR_channel, COLOR_GRAY2BGR);

		for (int i = 0; i < regions_red.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
			temp_vect = regions_red[i];

			for(int j =0; j < regions_red[i].size() ; j++)
			{
				circle( gR_channel, temp_vect[j], 1,  color, -1, 8);
			}

		}
	}

/*
	cout << "--------------------------WRITE TO FILE------------------" << endl;
	fB_channel << regions_red[0];
	fG_channel << gG_channel;
	fR_channel << gR_channel;
*/
	cout << "--------------------------WRITE IMAGE--------------------" << endl;

	//Zapis obrazow na dysk
	memset(file_path, 0, sizeof(file_path));
	sprintf(file_path, "C:/Users/Pawel/Desktop/baza_znakow/Red.jpg");
	imwrite(file_path, gR_channel);

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

	cout << "--------------------------END----------------------------" << endl;
	
	fB_channel.close();
	fG_channel.close();
	fR_channel.close();
	fOriginalImg.close();

	gOrginalImage.release();

	gBGRImage[0].release();
	gBGRImage[1].release();
	gBGRImage[2].release();

	gR_channel.release();
	gB_channel.release();
	gY_channel.release();
	gW_channel.release();

	//dst.release();

    return 0;
}

#endif

//Praca na wielu obrazach

#if (!SIMPLE_IMG)

char file_out_name[10], file_name[10], file_path_in[256], file_path_out[256], string_temp[256];
unsigned int file_number;


int main(int argc, char** argv)
{
	cout << "--------------------------START--------------------------" << endl;

	cout << "Enter image path: " << endl;
	cin >> file_path_in;

	cout << "Image name: ";
	cin >> file_name;

	memcpy(string_temp, file_path_in, sizeof(file_path_in));

	file_number = 1;

	while(1)
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
			getchar();
			return -1;
		}

		//Konwersja obrazu
		gOrginalImage.convertTo(gOrginalImage, CV_32FC3); 
		gOrginalImage = gOrginalImage/255;

		//Rozdzielenie kanalow B, G, R
		split(gOrginalImage, gBGRImage);
	
		ImAdjust(gBGRImage[2], gR_ch_main, 0.5, 1, 0, 1);
		ImAdjust(gBGRImage[1], gG_ch_main, 0.5, 1, 0, 1);
		ImAdjust(gBGRImage[0], gB_ch_main, 0.5, 1, 0, 1);

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
		
		int w(gR_channel.cols);
		int h(gR_channel.rows);

		Mat dst(5*w, 5*h, CV_8UC3);

		temp[0].zeros(gR_channel.rows, gR_channel.cols, CV_8UC1);
		temp[0] = gR_channel;

		gOrginalImage = gOrginalImage * 255;
		gOrginalImage.convertTo(gOrginalImage, CV_8UC3); 

		//Wyciecie pierwszego okna
		gOrginalImage.copyTo(dst(Rect(0, 0, w, h)));

		//MSER 1
		{
			MSER ms_red(MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION);

			vector<vector<Point>> regions_red;
			vector<Point> temp_vect;
			ms_red(gR_channel, regions_red, Mat());

			cvtColor(gR_channel, gR_channel, COLOR_GRAY2BGR);

			for (int i = 0; i < regions_red.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
				temp_vect = regions_red[i];

				for(int j =0; j < regions_red[i].size() ; j++)
				{
					circle( gR_channel, temp_vect[j], 1,  color, -1, 8);
				}

			}

			gR_channel.copyTo(dst(Rect(0, h, w, h)));
		}

		//MSER 2
		{
			gR_channel.zeros(gR_channel.rows, gR_channel.cols, CV_8UC1);
			gR_channel = temp[0];
			MSER ms_red(MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION+0.1);

			vector<vector<Point>> regions_red;
			vector<Point> temp_vect;
			ms_red(gR_channel, regions_red, Mat());

			cvtColor(gR_channel, gR_channel, COLOR_GRAY2BGR);

			for (int i = 0; i < regions_red.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
				temp_vect = regions_red[i];

				for(int j =0; j < regions_red[i].size() ; j++)
				{
					circle( gR_channel, temp_vect[j], 1,  color, -1, 8);
				}

			}

			gR_channel.copyTo(dst(Rect(w, 0, w, h)));
		}

		//MSER 2
		{
			gR_channel.zeros(gR_channel.rows, gR_channel.cols, CV_8UC1);
			gR_channel = temp[0];
			MSER ms_red(MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION+0.2);

			vector<vector<Point>> regions_red;
			vector<Point> temp_vect;
			ms_red(gR_channel, regions_red, Mat());

			cvtColor(gR_channel, gR_channel, COLOR_GRAY2BGR);

			for (int i = 0; i < regions_red.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
				temp_vect = regions_red[i];

				for(int j =0; j < regions_red[i].size() ; j++)
				{
					circle( gR_channel, temp_vect[j], 1,  color, -1, 8);
				}

			}

			gR_channel.copyTo(dst(Rect(w, h, w, h)));
		}
/*
		cvtColor(gB_channel, gB_channel, COLOR_GRAY2BGR);
		cvtColor(gY_channel, gY_channel, COLOR_GRAY2BGR);
		cvtColor(gW_channel, gW_channel, COLOR_GRAY2BGR);
*/		
		ImCropp(dst, dst, 0, 0, 2*w, 2*h);

		putText(dst, "Original Image", Point2f(50,50), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
		
		sprintf(tempString, "MSER(delta= %d, min_area= %d, max_area= %d, max_variation= %0.2f)", MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION);
		putText(dst, tempString, Point2f(50,50+h), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
		memset(tempString, 0, sizeof(tempString));

		sprintf(tempString, "MSER(delta= %d, min_area= %d, max_area= %d, max_variation= %0.2f)", MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION+0.1);
		putText(dst, tempString, Point2f(50+w,50), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
		memset(tempString, 0, sizeof(tempString));

		sprintf(tempString, "MSER(delta= %d, min_area= %d, max_area= %d, max_variation= %0.2f)", MSER_DELTA, MSER_MIN_AREA, MSER_MAX_AREA, MSER_VARIATION+0.2);
		putText(dst, tempString, Point2f(50+w,50+h), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255), 2);
		memset(tempString, 0, sizeof(tempString));

		cout << "--------------------------WRITE IMAGE--------------------" << endl;

		//Zapis obrazow na dysk

		memset(file_path_out, 0, sizeof(file_path_out));
		sprintf(file_path_out, "C:/Users/Pawel/Desktop/baza_znakow/MSER/RED/%d_mser_red.jpg", file_number);
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