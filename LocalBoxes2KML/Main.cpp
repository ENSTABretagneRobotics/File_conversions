// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Env.h"
	
// Parameters.
unsigned int skipfactor = 0;

inline int LoadConfig()
{
	FILE* file = NULL;
	char line[256];

	memset(line, 0, sizeof(line));

	// Default values.
	skipfactor = 10;

	file = fopen("LocalBoxes2KML.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%u", &skipfactor) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

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
	double d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, d9 = 0, d10 = 0; 
	double dt = 0, t0 = 0, t = 0, latmin = 0, longmin = 0, latmax = 0, longmax = 0, altitude = 0;
	double xmin = 0, ymin = 0, xmax = 0, ymax = 0, z = 0;
	char line[4096];
	unsigned int i = 0;
	time_t tt;
	struct tm* timeptr = NULL;
	char timebuf[32];

	if (argc != 2)
	{
		strcpy(szFileInPath, "logstate.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : LocalBoxes2KML file.csv.\n");
		printf("Default : LocalBoxes2KML %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.kml", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadConfig();
	LoadEnv();

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
		printf("Unable to create KML file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	strcpy(timebuf, "0");

	fprintf(fileout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileout, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	fprintf(fileout, "<Document>\n");
	fprintf(fileout, "\t<name>logstate</name>\n\t<open>1</open>\n\t<visibility>1</visibility>\n");
	fprintf(fileout, "\t<Style id=\"sn_poly\">\n\t\t<PolyStyle>\n\t\t\t<fill>0</fill>\n\t\t\t<color>ff0000ff</color>\n\t\t\t<width>2</width>\n\t\t</PolyStyle>\n\t</Style>\n");

	printf("Converting...\n");

	t = 0;
	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		t0 = t;
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf", 
			&t, &d0, &d1, &d2, &d3, &d4, &d5, &d6, &d7, &d8, &d9, &d10, 
			&xmin, &xmax, &ymin, &ymax) == 16) 
		{
			dt = t-t0;
			if ((i%skipfactor) == skipfactor-1)
			{
				tt = (time_t)t;
				timeptr = gmtime(&tt);
				if (timeptr != NULL) strftime(timebuf, 32, "%Y-%m-%dT%H:%M:%SZ", timeptr);

				EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, xmin, ymin, z, &latmin, &longmin, &altitude);
				EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, xmax, ymax, z, &latmax, &longmax, &altitude);

				fprintf(fileout, "\t<Placemark>\n\t\t<visibility>0</visibility>\n\t\t<TimeStamp>\n\t\t\t<when>%.32s</when>\n\t\t</TimeStamp>\n\t\t<name>%d</name>\n\t\t<styleUrl>#sn_poly</styleUrl>\n\t\t<Polygon>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n"
					"\t\t\t<tessellate>1</tessellate>\n\t\t\t<outerBoundaryIs>\n\t\t\t\t<LinearRing>\n", timebuf, i);

				fprintf(fileout, "\t\t\t\t\t<coordinates>"
					"%.8f,%.8f,%.3f "
					"%.8f,%.8f,%.3f "
					"%.8f,%.8f,%.3f "
					"%.8f,%.8f,%.3f "
					"%.8f,%.8f,%.3f "
					"</coordinates>\n", 
					longmin, latmin, altitude, 
					longmin, latmax, altitude, 
					longmax, latmax, altitude, 
					longmax, latmin, altitude, 
					longmin, latmin, altitude);

				fprintf(fileout, "\t\t\t\t</LinearRing>\n\t\t\t</outerBoundaryIs>\n\t\t</Polygon>\n\t</Placemark>\n");
			}
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the CSV file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(fileout, "</Document>\n</kml>\n");

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing KML file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing CSV file.\n");
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
