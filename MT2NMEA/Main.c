// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#define MAX_NB_BYTES_NMEA_CHECKSUM 4

inline void ComputeNMEAchecksum(char* sentence, char* checksum)
{
	int i = 0;
	char res = 0;
	BOOL bFoundBeginning = FALSE;

	memset(checksum, 0, MAX_NB_BYTES_NMEA_CHECKSUM);
	while (sentence[i])
	{
		// '$' for classic NMEA, '!' for AIS.
		if ((!bFoundBeginning)&&((sentence[i] == '$')||(sentence[i] == '!')))
		{
			bFoundBeginning = TRUE;
			i++;
			continue;
		}
		if (sentence[i] == '*')
		{
			break;
		}
		res ^= sentence[i];
		i++;
	}

	sprintf(checksum, "*%02x", (int)res);
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
	double t = 0;
	time_t tt;
	struct tm* timeptr = NULL;
	char timebuf[32];
	double utc = 0, date = 0;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0;
	double second = 0; 

	char tmpbuf[MAX_BUF_LEN];
	char checksum[4];
	int latdeg = 0;
	double latdecmin = 0;
	char NorthOrSouth = 0;
	int longdeg = 0;
	double longdecmin = 0;
	char EastOrWest = 0;
	double sog = 0, cog = 0;
	int GPS_quality_indicator = 0, nbsat = 0;
	double hdop = 0;
	double height_geoid = 0;
	
	// Temporary buffers for sscanf().
	double d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, 
		d9 = 0, d10 = 0, d11 = 0, d12 = 0, d13 = 0, d14 = 0, d15 = 0, d16 = 0, 
		roll = 0, pitch = 0, yaw = 0, latitude = 0, longitude = 0, altitude = 0;
	int UTC_Year = 0, UTC_Month = 0, UTC_Day = 0, UTC_Hour = 0, UTC_Minute = 0; 
	double UTC_Seconds = 0;
	struct timeval tv;
	
	if (argc != 2)
	{
		strcpy(szFileInPath, "MT.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : MT2NMEA file.csv.\n");
		printf("Default : MT2NMEA %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s_nmea.txt", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	//LoadConfig();

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open CSV file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create TXT file.\n");
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
		if (sscanf(line, 
			"%lf;%lf;%lf;"
			"%d;%d;%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf",
			&d0, &d1, &d2, 
			&UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&d3, &d4, &d5, &d6, &d7, &d8, &d9, &d10, &d11, &d12, &d13, 
			&roll, &pitch, &yaw, 
			&latitude, &longitude, &altitude,
			&d14, &d15, &d16) == 29) 
		{

			// Should check which data give the best info on date/time...

			//$GPRMC,125357.000,A,4826.822940,N,00424.660078,W,0.9,165.6,220414,,,A*77
			memset(tmpbuf, 0, sizeof(tmpbuf));
			//latitude = 48.39;
			//longitude = -4.425;
			GPSLatitudeDecDeg2DegDecMin(latitude, &latdeg, &latdecmin, &NorthOrSouth);
			GPSLongitudeDecDeg2DegDecMin(longitude, &longdeg, &longdecmin, &EastOrWest);
			sprintf(tmpbuf, "$GPRMC,%02d%02d%02d,A,%02d%07.4f,%c,%03d%07.4f,%c,0,0,%02d%02d%02d,,,A", 
				UTC_Hour, UTC_Minute, (int)UTC_Seconds, 
				latdeg, latdecmin, NorthOrSouth, longdeg, longdecmin, EastOrWest,  
				UTC_Day, UTC_Month, UTC_Year-2000);
			ComputeNMEAchecksum(tmpbuf, checksum);
			fprintf(fileout, "%s%s\r\n", tmpbuf, checksum);

			//$GPGGA,125357.000,4826.822940,N,00424.660078,W,1,14,0.6,118.4,M,51.9,M,,*4B
			memset(tmpbuf, 0, sizeof(tmpbuf));
			//latitude = 48.39;
			//longitude = -4.425;
			GPSLatitudeDecDeg2DegDecMin(latitude, &latdeg, &latdecmin, &NorthOrSouth);
			GPSLongitudeDecDeg2DegDecMin(longitude, &longdeg, &longdecmin, &EastOrWest);
			sprintf(tmpbuf, "$GPGGA,%02d%02d%02d,%02d%07.4f,%c,%03d%07.4f,%c,1,7,1.3,%f,M,,,,", 
				UTC_Hour, UTC_Minute, (int)UTC_Seconds, latdeg, latdecmin, NorthOrSouth, longdeg, longdecmin, EastOrWest, altitude);
			ComputeNMEAchecksum(tmpbuf, checksum);
			fprintf(fileout, "%s%s\r\n", tmpbuf, checksum);
			
			i++;
		}
		else
		{
			//printf("Skipping an invalid line in the CSV file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing TXT file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing CSV file.\n");
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
