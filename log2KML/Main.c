// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#define ALTITUDE_MODE "absolute"
//#define ALTITUDE_MODE "clampToGround"

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
	double f0 = 0, f1 = 0, latitude = 0, longitude = 0, altitude = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "log.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : log2KML file.txt.\n");
		printf("Default : log2KML %.255s.\n", szFileInPath);
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

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open txt file.\n");
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
	//fprintf(fileout, "<Folder>\n");
	//fprintf(fileout, "<name>%.249s</name>\n", szName);
	//fprintf(fileout, "<open>1</open>\n");

	fprintf(fileout, "<Document>\n");
	fprintf(fileout, "<name>%.249s</name>\n", szName);
	fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
	fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
	fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>"ALTITUDE_MODE"</altitudeMode>\n\t\t\t<coordinates>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf", &f0, &f1, &latitude, &longitude, &altitude) == 5) 
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
				fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>"ALTITUDE_MODE"</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			fprintf(fileout, "%f,%f,%f ", longitude, latitude, altitude);
			i++;
		}
		else if (sscanf(line, "%lf;%lf;%lf;%lf", &f0, &f1, &latitude, &longitude) == 4) 
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
				fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>"ALTITUDE_MODE"</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			// Optimization for the altitude : integer.
			fprintf(fileout, "%f,%f,%d ", longitude, latitude, 0);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the txt file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
	fprintf(fileout, "</Document>\n");
	//fprintf(fileout, "</Folder>\n");
	fprintf(fileout, "</kml>\n");

	printf("Found %u positions.\n", i);

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
		printf("Error closing txt file.\n");
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
