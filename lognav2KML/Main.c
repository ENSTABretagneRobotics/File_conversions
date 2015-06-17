// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

int main()
{
	FILE* lognavfile = NULL;
	FILE* kmlfile = NULL;
	char line[4096];
	unsigned int i = 0;
	double utc = 0, latitude = 0, longitude = 0, altitude = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	lognavfile = fopen("lognav.csv", "r");
	if (lognavfile == NULL)
	{
		printf("Unable to open csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	kmlfile = fopen("lognav.kml", "w");
	if (kmlfile == NULL)
	{
		printf("Unable to create kml file.\n");
		fclose(lognavfile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(kmlfile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(kmlfile, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	//fprintf(kmlfile, "<Folder>\n");
	//fprintf(kmlfile, "<name>lognav</name>\n");
	//fprintf(kmlfile, "<open>1</open>\n");

	fprintf(kmlfile, "<Document>\n");
	fprintf(kmlfile, "<name>lognav</name>\n");
	fprintf(kmlfile, "\t<Placemark>\n\t\t<name>lognav</name>\n");
	fprintf(kmlfile, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
	fprintf(kmlfile, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<coordinates>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(lognavfile, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf", &utc, &latitude, &longitude, &altitude) == 4) 
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(kmlfile, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(kmlfile, "\t<Placemark>\n\t\t<name>lognav</name>\n");
				fprintf(kmlfile, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(kmlfile, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			fprintf(kmlfile, "%f,%f,%f ", longitude, latitude, altitude);
			i++;
		}
		else if (sscanf(line, "%lf;%lf;%lf", &utc, &latitude, &longitude) == 3) 
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(kmlfile, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(kmlfile, "\t<Placemark>\n\t\t<name>lognav</name>\n");
				fprintf(kmlfile, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(kmlfile, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			// Optimization for the altitude : integer.
			fprintf(kmlfile, "%f,%f,%d ", longitude, latitude, 0);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the csv file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(kmlfile, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
	fprintf(kmlfile, "</Document>\n");
	//fprintf(kmlfile, "</Folder>\n");
	fprintf(kmlfile, "</kml>\n");

	printf("Found %u positions.\n", i);

	if (fclose(kmlfile) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
		fclose(lognavfile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(lognavfile) != EXIT_SUCCESS) 
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
