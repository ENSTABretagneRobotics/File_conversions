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
	double latitude = 0, longitude = 0, altitude = 0;
	// Temporary buffers for sscanf().
	double utc;
	int latdeg, longdeg;
	double latmin, longmin;
	char szlatdeg[3];
	char szlongdeg[4];
	char north, east;
	int GPS_quality_indicator, nbsat;
	double hdop;
	double height_geoid;

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
		memset(szlatdeg, 0, sizeof(szlatdeg));
		memset(szlongdeg, 0, sizeof(szlongdeg));
		if (sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d,%d,%lf,%lf,M,%lf,M", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator, &nbsat, &hdop, &altitude, &height_geoid) == 15) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			fprintf(fileout, "%f;%f;%f;%f;%f;\n", utc, latitude, longitude, altitude, height_geoid);
			i++;
		}
		else if (sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d,%d,%lf,%lf,M", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator, &nbsat, &hdop, &altitude) == 14) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			fprintf(fileout, "%f;%f;%f;%f;\n", utc, latitude, longitude, altitude);
			i++;
		}
		else if (sscanf(line, "$GPGGA,%lf,%c%c%lf,%c,%c%c%c%lf,%c,%d", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east,
			&GPS_quality_indicator) == 11) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			fprintf(fileout, "%f;%f;%f;\n", utc, latitude, longitude);
			i++;
		}
		// In case of no GPGGA...
		else if ((!utc)&&(sscanf(line, "$GPRMC,%lf,A,%c%c%lf,%c,%c%c%c%lf,%c", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east) == 10)) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			fprintf(fileout, "%f;%f;%f;\n", utc, latitude, longitude);
			i++;
		}
		// In case of no GPGGA nor GPRMC...
		else if ((!utc)&&(sscanf(line, "$GPGLL,%c%c%lf,%c,%c%c%c%lf,%c,%lf,A", 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east, &utc) == 10)) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			fprintf(fileout, "%f;%f;%f;\n", utc, latitude, longitude);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the nmea file.\n");
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
