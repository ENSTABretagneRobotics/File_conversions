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
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double t = 0, latitude = 0, longitude = 0, latitude2 = 0, longitude2 = 0, distance = 0, t_raw = 0;
	time_t tt;
	struct tm* timeptr = NULL;
	char timebuf[32];

	if (argc != 2)
	{
		strcpy(szFileInPath, "mdmrng.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : MDMRNG2KML file.csv.\n");
		printf("Default : MDMRNG2KML %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, ".kml", szFileOutPath, szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create kml file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}
	
	fprintf(fileout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileout, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	fprintf(fileout, "<Document>\n");
	fprintf(fileout, "<name>%.249s</name>\n", szName);
	fprintf(fileout, "\t<Style id=\"sn_ylw-pushpin\">\n\t\t<LineStyle>\n\t\t\t<color>ff0000ff</color>\n\t\t\t<width>2</width>\n\t\t</LineStyle>\n\t</Style>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf", 
			&t, &latitude, &longitude, &latitude2, &longitude2, &distance, &t_raw) == 7) 
		{
			tt = (time_t)t;
			timeptr = gmtime(&tt);
			if (timeptr != NULL) strftime(timebuf, 32, "%Y-%m-%dT%H:%M:%SZ", timeptr);
			fprintf(fileout, "\t<Placemark>\n\t\t<name>t=%.2f,d=%.1f</name>\n", t_raw, distance);
			fprintf(fileout, "\t\t<TimeStamp>\n\t\t\t<when>%.32s</when>\n\t\t</TimeStamp>\n", timebuf);
			fprintf(fileout, "\t\t<styleUrl>#sn_ylw-pushpin</styleUrl>\n\t\t<LineString>\n\t\t\t<tessellate>1</tessellate>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n\t\t\t<coordinates>\n");
			fprintf(fileout, "\t\t\t\t%.8f,%.8f,%f %.8f,%.8f,%f\n", longitude2, latitude2, 0.0, longitude, latitude, 0.0);
			fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the csv file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(fileout, "</Document>\n</kml>\n");

	printf("Found %u rng.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
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
