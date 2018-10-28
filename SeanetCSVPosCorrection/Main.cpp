// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"
#include "Seanet.h"

int main(int argc, char* argv[])
{
	char szFileInPath1[256];
	char szFileInPath2[256];
	char szFileOutPath[256];
	FILE* filein1 = NULL;
	FILE* filein2 = NULL;
	FILE* fileout = NULL;
	char szName[256];
	char line[4096];
	
	//char line[4*MAX_NB_BYTES_SEANET+1];

	unsigned int i = 0;
	double t_app = 0, xhat = 0, yhat = 0, zhat = 0, psihat = 0;

	if (argc != 3)
	{
		strcpy(szFileInPath1, "Seanet.csv");
		strcpy(szFileInPath2, "logstate.csv");
		printf("Warning : Wrong number of parameters.\n");
		printf("Usage : SeanetCSVPosCorrection Seanet.csv logstate.csv.\n");
		printf("Default : SeanetCSVPosCorrection %.255s %.255s.\n", szFileInPath1, szFileInPath2);
	}
	else
	{
		sprintf(szFileInPath1, "%.249s", argv[1]);
		sprintf(szFileInPath2, "%.249s", argv[2]);
	}

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein1 = fopen(szFileInPath1, "r");
	if (filein1 == NULL)
	{
		printf("Unable to open file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	filein2 = fopen(szFileInPath2, "r");
	if (filein2 == NULL)
	{
		printf("Unable to open file.\n");
		fclose(filein1);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}
		
	memset(line, 0, sizeof(line));
	if ((fgets3(filein1, line, sizeof(line)) != NULL)&&(strncmp(line, "SOf", 3) == 0))
	{
		// This file contains the sonar data.
	}
	else
	{
		// We need to swap the input files.
		fileout = filein1;
		memcpy(szFileOutPath, szFileInPath1, sizeof(szFileOutPath));
		filein1 = filein2;
		memcpy(szFileInPath1, szFileInPath2, sizeof(szFileOutPath));
		filein2 = fileout;
		memcpy(szFileInPath2, szFileOutPath, sizeof(szFileOutPath));
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath1, ".csv", szFileOutPath, szName);

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create file.\n");
		fclose(filein1);
		fclose(filein2);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}



	// For the time there is no year, month, day info inside the Seanet file so we need to get it from the filename, 
	// and for old versions of the logstate file only the seconds since the creation of the file are available 
	// inside so we need also to check the filename...

	// The logstate file should be always older than the Seanet file...
	
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	struct tm ts;
	double t0_logstate = 0, t0_data = 0, t1_data = 0;
	
	sscanf(szFileInPath2, "logstate_%d-%d-%d_%dh%dmin%ds", &year, &month, &day, &hour, &minute, &second);
	memset(&ts, 0, sizeof(ts));
	ts.tm_year = year-1900; ts.tm_mon = month-1; ts.tm_mday = day; ts.tm_hour = hour; ts.tm_min = minute; ts.tm_sec = second;
	t0_logstate = (double)_mkgmtime(&ts);

	sscanf(szFileInPath1, "Seanet0_%d-%d-%d_%dh%dmin%ds", &year, &month, &day, &hour, &minute, &second);
	memset(&ts, 0, sizeof(ts));
	ts.tm_year = year-1900; ts.tm_mon = month-1; ts.tm_mday = day; ts.tm_hour = hour; ts.tm_min = minute; ts.tm_sec = second;
	t0_data = (double)_mkgmtime(&ts);
	
	memset(&ts, 0, sizeof(ts));
	ts.tm_year = year-1900; ts.tm_mon = month-1; ts.tm_mday = day; ts.tm_hour = 0; ts.tm_min = 0; ts.tm_sec = 0;
	t1_data = (double)_mkgmtime(&ts);







	struct timeval tv;
	double angle = 0;
	unsigned char scanline[MAX_NB_BYTES_SEANET];
	unsigned char auxbuf[128]; // For daisy-chained device.
	int nbauxbytes = 0; // For daisy-chained device.


				time_t tt = 0;
				struct tm* timeptr = NULL;
				int Hour = 0, Min = 0;
				double Seconds = 0;

				//if (gettimeofday(&tv, NULL) != EXIT_SUCCESS) { tv.tv_sec = 0; tv.tv_usec = 0; }
				//...

				tt = tv.tv_sec;
				timeptr = localtime(&tt);
				if (timeptr != NULL)
				{
					Hour = timeptr->tm_hour;
					Min = timeptr->tm_min;
					Seconds = timeptr->tm_sec+0.000001*tv.tv_usec;
				}







	fprintf(fileout,
		"SOf,DateTime,Node,Status,Hdctrl,Rangescale,Gain,Slope,AdLow,AdSpan,LeftLim,RightLim,Steps,Bearing,Dbytes,Dbytes of DATA\n"
	);

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein1, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf", &t_app, &xhat, &yhat, &zhat, &psihat) == 5)
		{



			// Read sonar data...
			{
				int Hour = 0, Min = 0, Node = 0, Status = 0, Rangescale = 0, Gain = 0, Slope = 0,
					AdLow = 0, AdSpan = 0, LeftLim = 0, RightLim = 0, Steps = 0, Bearing = 0, Dbytes = 0;
				double Seconds = 0;
				union HDCTRL Hdctrl;
				char SOf[3+1];
				char data[4*MAX_NB_BYTES_SEANET+1];
				char* str = NULL;
				int byte = 0;
				int NBins = 0;
				int NSteps = 0;
				double StepAngleSize = 0;

				// Get a sonar scanline from the file.
				if (fgets3(file, line, sizeof(line)) == NULL) break;

				memset(SOf, 0, sizeof(SOf));
				//memset(data, 0, sizeof(data));
				data[sizeof(data)-1] = 0; // The last character must be a 0 to be a valid string for sscanf.
				Hdctrl.i = 0;

				if (sscanf(line, "%03s,%d:%d:%lf,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%8192s",
					SOf, &Hour, &Min, &Seconds, &Node, &Status, &Hdctrl.i, &Rangescale, &Gain, &Slope,
					&AdLow, &AdSpan, &LeftLim, &RightLim, &Steps, &Bearing, &Dbytes, data) != 18)
				{
					printf("Skipping an invalid line in the CSV file.\n");
					continue;
				}

				NBins = Hdctrl.bits.adc8on?Dbytes:2*Dbytes;
				NSteps = RESOLUTION2NUMBER_OF_STEPS(Steps);
				StepAngleSize = RESOLUTION2STEP_ANGLE_SIZE_IN_DEGREES(Steps);
				rangescale = Rangescale/10;

				tv.tv_sec = Hour*3600+Min*60+(long)Seconds;
				tv.tv_usec = (long)((Seconds-(long)Seconds)*1000000.0);
				angle = ((Bearing-3200+6400)%6400)*0.05625; // Angle of the transducer in degrees (0.05625 = (1/16)*(9/10)).

				// We should take into account ADLow and ADSpan here?

				str = data;
				if (!Hdctrl.bits.adc8on)
				{
					for (i = 0; i < Dbytes; i++)
					{
						(void)sscanf(str, "%d", &byte);
						scanline[2*i+0] = (unsigned char)((byte>>4)*16);
						scanline[2*i+1] = (unsigned char)(((byte<<4)>>4)*16);
						str = strchr(str, ',');
						if (!str) break;
						str++;
					}
				}
				else
				{
					for (i = 0; i < Dbytes; i++)
					{
						(void)sscanf(str, "%d", &byte);
						scanline[i] = (unsigned char)byte;
						str = strchr(str, ',');
						if (!str) break;
						str++;
					}
				}
			}










			// Write sonar data...
				{
					char SOf[3+1];
					int Bearing = 0;

					if (seanet.bDST) strcpy(SOf, "DIG"); else strcpy(SOf, "SON");

					Bearing = ((int)(angle/0.05625+3200+6400))%6400; // Angle of the transducer (0..6399 in 1/16 Gradian units, 0.05625 = (1/16)*(9/10)).

					fprintf(fileout, "%.3s,%02d:%02d:%06.3f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
						SOf, Hour, Min, Seconds, SERIAL_PORT_SONAR_NODE_NUMBER_SEANET, 
						seanet.HeadStatus, seanet.HeadHdCtrl.i, seanet.HeadRangescale, seanet.HeadIGain, 
						seanet.HeadSlope, seanet.HeadADLow, seanet.HeadADSpan, seanet.HeadLeftLim, seanet.HeadRightLim, 
						seanet.HeadSteps, Bearing, seanet.Dbytes
						);

					// We should take into account ADLow and ADSpan here?

					if (!seanet.adc8on)	
					{ 
						for (i = 0; i < seanet.Dbytes; i++)
						{
							int byte = ((scanline[2*i]/16)<<4)+scanline[2*i+1]/16;
							fprintf(fileout, ",%d", byte);
						}
					}
					else
					{ 
						for (i = 0; i < seanet.Dbytes; i++)
						{
							fprintf(fileout, ",%d", (int)scanline[i]);
						}
					}

					fprintf(fileout, "\n");

					fflush(fileout);
				}
















			i++;
		}
		else if ()
		{
			// Should remove the final LF from auxbuf (maybe in HeadDataReplyAndAuxDataSeanet()...) 
			// and end-of-line characters might be inconsistent with Seanet log files (especially on Linux)... 

			fprintf(fileout, "%02d:%02d:%06.3f,%.100s\n", Hour, Min, Seconds, (char*)auxbuf);

		}
		else
		{
			printf("Skipping an invalid line in the CSV file.\n");
		}
		memset(line, 0, sizeof(line));
	}





	printf("Found %u positions.\n", i);



	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing file.\n");
		fclose(filein2);
		fclose(filein1);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein2) != EXIT_SUCCESS) 
	{
		printf("Error closing file.\n");
		fclose(filein1);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein1) != EXIT_SUCCESS) 
	{
		printf("Error closing file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
