// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#define NB_SECTIONS_I 10
#define NB_SECTIONS_J 10

struct SECTION
{
	int i;
	int j;
	double latmin;
	double latmax;
	double longmin;
	double longmax;
	double latpos;
	double longpos;
	int bEntered;
	int time;
	int date;
};
typedef struct SECTION SECTION;

int main(int argc, char* argv[])
{
	double latW = 60.105, latE = 60.108, longS = 19.950, longN = 19.956;
	SECTION sections[NB_SECTIONS_I][NB_SECTIONS_J];
	int i = 0, j = 0;
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szTemp[256];
	char szName[256];
	char line[4096];
	unsigned int n = 0, ns = 0;
	double latitude = 0, longitude = 0;//, altitude = 0;
	// Temporary buffers for sscanf().
	double utc, date;
	int latdeg, longdeg;
	double latmin, longmin;
	char szlatdeg[3];
	char szlongdeg[4];
	char north, east;
	double sog, cog;
	int year, month, day, hour, minute, second; 
	//int GPS_quality_indicator, nbsat;
	//double hdop;
	//double height_geoid;

	if (argc != 2)
	{
		strcpy(szFileInPath, "nmea.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : NMEA2XML_WRSC2015 file.xml.\n");
		printf("Default : NMEA2XML_WRSC2015 %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.xml", szTemp);
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
		printf("Unable to create xml file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	// TODO : functions to compute lat,long from the 4 buoys positions, etc.

	for (i = 0; i < NB_SECTIONS_I; i++)
	{
		for (j = 0; j < NB_SECTIONS_J; j++)
		{
			sections[i][j].i = i;
			sections[i][j].j = j;
			sections[i][j].latmin = latW+(latE-latW)*i/10.0;
			sections[i][j].latmax = latW+(latE-latW)*(i+1)/10.0;
			sections[i][j].longmin = longS+(longN-longS)*j/10.0;
			sections[i][j].longmax = longS+(longN-longS)*(j+1)/10.0;
			sections[i][j].latpos = (sections[i][j].latmin+sections[i][j].latmax)/2;
			sections[i][j].longpos = (sections[i][j].longmin+sections[i][j].longmax)/2;
			sections[i][j].bEntered = 0;
			sections[i][j].time = 0;
			sections[i][j].date = 0;
			printf("%f;%f\n", sections[i][j].latpos, sections[i][j].longpos);
		}
	}

	fprintf(fileout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileout, "<AreaScanning Teamid=\"\" xmlns:gml=\"http://www.opengis.net/gml\" xmlns:smil20=\"http://www.w3.org/2001/SMIL20/\" xmlns:smil20lang=\"http://www.w3.org/2001/SMIL20/Language\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:xml=\"http://www.w3.org/XML/1998/namespace\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"AreaScanningResult.xsd\">\n");
	fprintf(fileout, "\t<TeamName>ENSTA Bretagne</TeamName>\n");

	printf("Converting...\n");

	n = 0;
	ns = 0;
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line), filein) != NULL) 
	{
		utc = 0;
		memset(szlatdeg, 0, sizeof(szlatdeg));
		memset(szlongdeg, 0, sizeof(szlongdeg));
		if ((!utc)&&(sscanf(line, "$GPRMC,%lf,A,%c%c%lf,%c,%c%c%c%lf,%c,%lf,%lf,%lf", &utc, 
			&szlatdeg[0], &szlatdeg[1], &latmin, &north, 
			&szlongdeg[0], &szlongdeg[1], &szlongdeg[2], &longmin, &east, 
			&sog, &cog, &date) == 13)) 
		{
			latdeg = atoi(szlatdeg);
			longdeg = atoi(szlongdeg);

			// Convert GPS latitude and longitude in decimal.
			latitude = (north == 'N')?(latdeg+latmin/60.0):-(latdeg+latmin/60.0);
			longitude = (east == 'E')?(longdeg+longmin/60.0):-(longdeg+longmin/60.0);

			hour = (int)utc/10000;
			minute = (int)utc/100-hour*100;
			second = ((int)utc-hour*10000)-minute*100;

			day = (int)date/10000;
			month = (int)date/100-day*100;
			year = ((int)date-day*10000)-month*100;

			for (i = 0; i < NB_SECTIONS_I; i++)
			{
				for (j = 0; j < NB_SECTIONS_J; j++)
				{
					if ((!sections[i][j].bEntered)&&
						(sections[i][j].latmin <= latitude)&&(latitude <= sections[i][j].latmax)&&
						(sections[i][j].longmin <= longitude)&&(longitude <= sections[i][j].longmax))
					{
						sections[i][j].bEntered = 1;
						fprintf(fileout, "\t<Section>\n");
						fprintf(fileout, "\t\t<sectioni>%d</sectioni>\n", sections[i][j].i);
						fprintf(fileout, "\t\t<sectionj>%d</sectionj>\n", sections[i][j].j);
						fprintf(fileout, "\t\t<dateTime>20%02d-%02d-%02dT%02d:%02d:%02dZ</dateTime>\n", 
							year, month, day, hour, minute, second);
						fprintf(fileout, "\t\t<gml:pos srsDimension=\"2\" srsName=\"urn:ogc:def:crs:EPSG:6.6:4326\">%f %f</gml:pos>\n", 
							sections[i][j].latpos, sections[i][j].longpos);
						//fprintf(fileout, "\t\t<depth>0.0</depth>\n");
						//fprintf(fileout, "\t\t<watertemp>0.0</watertemp>\n");
						//fprintf(fileout, "\t\t<airtemp>0.0</airtemp>\n");
						//fprintf(fileout, "\t\t<watersalinity>0.0</watersalinity>\n");
						//fprintf(fileout, "\t\t<conductivity>0.0</conductivity>\n");
						//fprintf(fileout, "\t\t<chlorophyll>0.0</chlorophyll>\n");
						//fprintf(fileout, "\t\t<ammonium>0.0</ammonium>\n");
						//fprintf(fileout, "\t\t<nitrate>0.0</nitrate>\n");
						//fprintf(fileout, "\t\t<chloride>0.0</chloride>\n");
						//fprintf(fileout, "\t\t<totaldissolvedsolids>0.0</totaldissolvedsolids>\n");
						fprintf(fileout, "\t</Section>\n");
						ns++;
					}
				}
			}
			n++;
		}
		else
		{
			printf("Skipping an invalid line in the nmea file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(fileout, "</AreaScanning>\n");

	printf("Found %u positions.\n", n);
	printf("Found %u sections.\n", ns);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing xml file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
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
