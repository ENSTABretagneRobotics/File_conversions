// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szTemp[256];
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double latitude = 0, longitude = 0;
	struct tm ts;
	double t = 0;
	// Temporary buffers for sscanf().
	double utc = 0, date = 0;
	int latdeg = 0, longdeg = 0;
	double latmin = 0, longmin = 0;
	char szlatdeg[3];
	char szlongdeg[4];
	char north = 0, east = 0;
	double sog = 0, cog = 0;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0;
	double second = 0; 

	if (argc != 2)
	{
		strcpy(szFileInPath, "lognav.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : NMEA2CSV_WRSC2016 file.txt.\n");
		printf("Default : NMEA2CSV_WRSC2016 %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s_wrsc2016.csv", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open NMEA file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create CSV file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line), filein) != NULL) 
	{
		utc = 0;
		//date = 0;
		//t = 0;
		memset(szlatdeg, 0, sizeof(szlatdeg));
		memset(szlongdeg, 0, sizeof(szlongdeg));
		if (sscanf(line, "$GPRMC,%lf,A,%c%c%lf,%c,%c%c%c%lf,%c,%lf,%lf,%lf", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east, 
			&sog, &cog, &date) == 13) 
		{
			if (utc > 0)
			{
				hour = (int)utc/10000;
				minute = (int)utc/100-hour*100;
				second = (utc-hour*10000)-minute*100;
			}

			if ((strlen(szlatdeg) > 0)&&(strlen(szlongdeg) > 0))
			{
				latdeg = atoi(szlatdeg);
				longdeg = atoi(szlongdeg);

				// Convert GPS latitude and longitude in decimal.
				latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
				longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);
			}

			if (date > 0)
			{
				day = (int)date/10000;
				month = (int)date/100-day*100;
				year = 2000+((int)date-day*10000)-month*100;
			}

			if (utc > 0)
			{
				if (date > 0)
				{
					memset(&ts, 0, sizeof(ts));
					ts.tm_year = year;
					ts.tm_mon = month;
					ts.tm_mday = day;
					ts.tm_hour = hour;
					ts.tm_min = minute;
					ts.tm_sec = (int)second;
					ts.tm_year-=1900; // Conversion...
					ts.tm_mon--; // Conversion...
					t = _mkgmtime(&ts)+(second-(int)second);
				}
				else 
				{
					t = hour*3600+minute*60+second;
				}
			}

			if ((utc > 0)&&(strlen(szlatdeg) > 0)&&(strlen(szlongdeg) > 0)&&(date > 0))
			{
				fprintf(fileout, "%02d%02d%02d%02d,%d,%d\n", ts.tm_hour, ts.tm_min, ts.tm_sec, ts.tm_mday, (int)(10000000*latitude), (int)(10000000*longitude));
				i++;
			}
		}
		else
		{
			//printf("Skipping an invalid line in the nmea file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing CSV file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing NMEA file.\n");
		fclose(fileout);
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
