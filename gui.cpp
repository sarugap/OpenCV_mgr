#include "gui.h"

extern unsigned int flMserColor;

extern _tMSER MSERred, MSERblue, MSERyellow, MSERwhite, MSERblack;

extern char file_name[10], file_path_in[256], mser_param[50];

void CreateGui(void)
{
	cout << "--------------------------START--------------------------" << endl;

	cout << "Enter image path: " << endl;
	cin >> file_path_in;

	cout << "Image name: ";
	cin >> file_name;

	cout << "Enter color (red-0, blue-1, yellow-2, white-3, black-4): ";
	cin >> flMserColor;

	switch(flMserColor)
	{
		case MSER_RED:
			printf("Default MSER (red):		%d, %d, %d, %.2f\r\n", MSERred.DELTA, MSERred.MIN_AREA, MSERred.MAX_AREA, MSERred.VARIATION);
			break;
		case MSER_BLUE:
			printf("Default MSER (blue):	%d, %d, %d, %.2f\r\n", MSERblue.DELTA, MSERblue.MIN_AREA, MSERblue.MAX_AREA, MSERblue.VARIATION);
			break;
		case MSER_YELLOW:
			printf("Default MSER (yellow):	%d, %d, %d, %.2f\r\n", MSERyellow.DELTA, MSERyellow.MIN_AREA, MSERyellow.MAX_AREA, MSERyellow.VARIATION);
			break;
		case MSER_WHITE:
			printf("Default MSER (white):	%d, %d, %d, %.2f\r\n", MSERwhite.DELTA, MSERwhite.MIN_AREA, MSERwhite.MAX_AREA, MSERwhite.VARIATION);
			break;
		case MSER_BLACK:
			printf("Default MSER (black):	%d, %d, %d, %.2f\r\n", MSERblack.DELTA, MSERblack.MIN_AREA, MSERblack.MAX_AREA, MSERblack.VARIATION);
			break;
	}

	cout << "MSER parameters: ";
	cin >> mser_param;

	//parser
	{
		unsigned int i,j,k;
		char temp_string[10];

		memset(temp_string, 0, sizeof(temp_string));
		i = j = k = 0;
		
		for(i = 0; i <= strlen(mser_param); i++)
		{
			if((mser_param[i] != ',')&&(mser_param[i] != 0))
			{
				temp_string[k] = mser_param[i];
				k++;
			}
			else if(mser_param[i] == 0)
			{
				switch(flMserColor)
				{
					case MSER_RED:
						MSERred.VARIATION = atof(temp_string);
						break;
					case MSER_BLUE:
						MSERblue.VARIATION = atof(temp_string);
						break;
					case MSER_YELLOW:
						MSERyellow.VARIATION = atof(temp_string);
						break;
					case MSER_WHITE:
						MSERwhite.VARIATION = atof(temp_string);
						break;
					case MSER_BLACK:
						MSERblack.VARIATION = atof(temp_string);
						break;
				}
			}
			else
			{
				switch (j)
				{
					case 0:
						switch(flMserColor)
						{
							case MSER_RED:
								MSERred.DELTA = atoi(temp_string);
								break;
							case MSER_BLUE:
								MSERblue.DELTA = atof(temp_string);
								break;
							case MSER_YELLOW:
								MSERyellow.DELTA = atof(temp_string);
								break;
							case MSER_WHITE:
								MSERwhite.DELTA = atof(temp_string);
								break;
							case MSER_BLACK:
								MSERblack.DELTA = atof(temp_string);
								break;
						}
						break;
					case 1:
						switch(flMserColor)
						{
							case MSER_RED:
								MSERred.MIN_AREA = atoi(temp_string);
								break;
							case MSER_BLUE:
								MSERblue.MIN_AREA = atof(temp_string);
								break;
							case MSER_YELLOW:
								MSERyellow.MIN_AREA = atof(temp_string);
								break;
							case MSER_WHITE:
								MSERwhite.MIN_AREA = atof(temp_string);
								break;
							case MSER_BLACK:
								MSERblack.MIN_AREA = atof(temp_string);
								break;
						}
						break;
					case 2:
						switch(flMserColor)
						{
							case MSER_RED:
								MSERred.MAX_AREA = atoi(temp_string);
								break;
							case MSER_BLUE:
								MSERblue.MAX_AREA = atof(temp_string);
								break;
							case MSER_YELLOW:
								MSERyellow.MAX_AREA = atof(temp_string);
								break;
							case MSER_WHITE:
								MSERwhite.MAX_AREA = atof(temp_string);
								break;
							case MSER_BLACK:
								MSERblack.MAX_AREA = atof(temp_string);
								break;
						}
						break;
				}
				memset(temp_string, 0, sizeof(temp_string));
				j++;
				k = 0;
			}
		}
	}

	switch(flMserColor)
	{
		case MSER_RED:
			printf("MSER parameters (red):		%d, %d, %d, %.2f\r\n", MSERred.DELTA, MSERred.MIN_AREA, MSERred.MAX_AREA, MSERred.VARIATION);
			break;
		case MSER_BLUE:
			printf("MSER parameters (blue):		%d, %d, %d, %.2f\r\n", MSERblue.DELTA, MSERblue.MIN_AREA, MSERblue.MAX_AREA, MSERblue.VARIATION);
			break;
		case MSER_YELLOW:
			printf("MSER parameters (yellow):	%d, %d, %d, %.2f\r\n", MSERyellow.DELTA, MSERyellow.MIN_AREA, MSERyellow.MAX_AREA, MSERyellow.VARIATION);
			break;
		case MSER_WHITE:
			printf("MSER parameters (white):	%d, %d, %d, %.2f\r\n", MSERwhite.DELTA, MSERwhite.MIN_AREA, MSERwhite.MAX_AREA, MSERwhite.VARIATION);
			break;
		case MSER_BLACK:
			printf("MSER parameters (black):	%d, %d, %d, %.2f\r\n", MSERblack.DELTA, MSERblack.MIN_AREA, MSERblack.MAX_AREA, MSERblack.VARIATION);
			break;
	}
};