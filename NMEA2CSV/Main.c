// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

// Parameters.
BOOL bDisableAutoDateDetection = FALSE;
BOOL bForce_GPGGA_height_geoid = FALSE;
BOOL bForce_GPGGA_altitude = FALSE;
BOOL bForce_GPGGA = FALSE;
BOOL bForce_GPRMC = FALSE;
BOOL bForce_GPGLL = FALSE;

// Other.
BOOL bAutoSelection = FALSE;

inline int LoadConfig()
{
	FILE* file = NULL;
	char line[256];

	memset(line, 0, sizeof(line));

	// Default values.
	bDisableAutoDateDetection = 0;
	bForce_GPGGA_height_geoid = 0;
	bForce_GPGGA_altitude = 0;
	bForce_GPGGA = 0; 
	bForce_GPRMC = 0;
	bForce_GPGLL = 0;

	file = fopen("NMEA2CSV.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bDisableAutoDateDetection) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForce_GPGGA_height_geoid) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForce_GPGGA_altitude) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForce_GPGGA) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForce_GPRMC) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForce_GPGLL) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	// Manual or auto selection of the best NMEA sentence...
	bAutoSelection = !(bForce_GPGGA_height_geoid|bForce_GPGGA_altitude|bForce_GPGGA|bForce_GPRMC|bForce_GPGLL);

	return EXIT_SUCCESS;
}

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
	double latitude = 0, longitude = 0, altitude = 0;
	struct tm ts;
	double t = 0;
	BOOL bFound_height_geoid = FALSE, bFound_altitude = FALSE, 
		bFound_GPGGA = FALSE, bFound_GPRMC = FALSE, bFound_GPGLL = FALSE;
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
	int GPS_quality_indicator = 0, nbsat = 0;
	double hdop = 0;
	double height_geoid = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "lognav.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : NMEA2CSV file.txt.\n");
		printf("Default : NMEA2CSV %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.csv", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadConfig();

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open nmea file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create csv file.\n");
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
		if (!bDisableAutoDateDetection)
		{
			// Allways try to get the full date...
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
			}
		}

		utc = 0;
		memset(szlatdeg, 0, sizeof(szlatdeg));
		memset(szlongdeg, 0, sizeof(szlongdeg));
		if ((bForce_GPGGA_height_geoid||bAutoSelection)
			&&		
			(sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d,%d,%lf,%lf,M,%lf,M", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator, &nbsat, &hdop, &altitude, &height_geoid) == 15)
			&&
			(GPS_quality_indicator > 0)) 
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

			if (utc > 0)
			{
				if (date > 0)
				{
					// Use the full date provided by another previous NMEA sentence.
				}
				else 
				{
					// Use only utc from this sentence.
					t = hour*3600+minute*60+second;
				}
			}

			bFound_GPGGA = TRUE;
			bFound_altitude = TRUE;
			bFound_height_geoid = TRUE;

			fprintf(fileout, "%f;%f;%f;%f;%f;\n", t, latitude, longitude, altitude, height_geoid);
			i++;
		}
		else if ((bForce_GPGGA_altitude||(bAutoSelection&&!bFound_height_geoid))
			&&
			(sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d,%d,%lf,%lf,M", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator, &nbsat, &hdop, &altitude) == 14)
			&&
			(GPS_quality_indicator > 0)) 
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

			if (utc > 0)
			{
				if (date > 0)
				{
					// Use the full date provided by another previous NMEA sentence.
				}
				else 
				{
					// Use only utc from this sentence.
					t = hour*3600+minute*60+second;
				}
			}

			bFound_GPGGA = TRUE;
			bFound_altitude = TRUE;

			fprintf(fileout, "%f;%f;%f;%f;\n", t, latitude, longitude, altitude);
			i++;
		}
		else if ((bForce_GPGGA||(bAutoSelection&&!bFound_altitude))
			&&
			(sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator) == 11)
			&&
			(GPS_quality_indicator > 0))
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

			if (utc > 0)
			{
				if (date > 0)
				{
					// Use the full date provided by another previous NMEA sentence.
				}
				else 
				{
					// Use only utc from this sentence.
					t = hour*3600+minute*60+second;
				}
			}

			bFound_GPGGA = TRUE;

			fprintf(fileout, "%f;%f;%f;\n", t, latitude, longitude);
			i++;
		}
		// In case of no GPGGA...
		else if ((bForce_GPRMC||(bAutoSelection&&!bFound_GPGGA))
			&&
			(sscanf(line, "$GPRMC,%lf,A,%c%c%lf,%c,%c%c%c%lf,%c,%lf,%lf,%lf", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east, 
			&sog, &cog, &date) == 13)) 
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

			bFound_GPRMC = TRUE;

			fprintf(fileout, "%f;%f;%f;\n", t, latitude, longitude);
			i++;
		}
		// In case of no GPGGA nor GPRMC...
		else if ((bForce_GPGLL||(bAutoSelection&&!bFound_GPGGA&&!bFound_GPRMC))
			&&
			(sscanf(line, "$GPGLL,%c%c%lf,%c,%c%c%c%lf,%c,%lf,A", 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east, &utc) == 10)) 
		{
			if ((strlen(szlatdeg) > 0)&&(strlen(szlongdeg) > 0))
			{
				latdeg = atoi(szlatdeg);
				longdeg = atoi(szlongdeg);

				// Convert GPS latitude and longitude in decimal.
				latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
				longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);
			}

			if (utc > 0)
			{
				hour = (int)utc/10000;
				minute = (int)utc/100-hour*100;
				second = (utc-hour*10000)-minute*100;
			}

			if (utc > 0)
			{
				if (date > 0)
				{
					// Use the full date provided by another previous NMEA sentence.
				}
				else 
				{
					// Use only utc from this sentence.
					t = hour*3600+minute*60+second;
				}
			}

			bFound_GPGLL = TRUE;

			fprintf(fileout, "%f;%f;%f;\n", t, latitude, longitude);
			i++;
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
		printf("Error closing csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing nmea file.\n");
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
