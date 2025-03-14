// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

int main()
{
	FILE* xmlfile = NULL;
	FILE* kmlfile = NULL;
	char line[4096];
	unsigned int i = 0;
	double latitude = 0, longitude = 0, altitude = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	xmlfile = fopen("sms.xml", "r");
	if (xmlfile == NULL)
	{
		printf("Unable to open XML file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	kmlfile = fopen("lognav.kml", "w");
	if (kmlfile == NULL)
	{
		printf("Unable to create KML file.\n");
		fclose(xmlfile);
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
	while (fgets3(xmlfile, line, sizeof(line)) != NULL) 
	{
		char* ptr = strstr(line, "LAT:");
		if (ptr&&(sscanf(ptr, "LAT:%lf&#10;LON:%lf&#10;ALT:%lf", &latitude, &longitude, &altitude) == 3))
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(kmlfile, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(kmlfile, "\t<Placemark>\n\t\t<name>lognav</name>\n");
				fprintf(kmlfile, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(kmlfile, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			fprintf(kmlfile, "%.8f,%.8f,%.3f ", longitude, latitude, altitude);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the XML file.\n");
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
		printf("Error closing KML file.\n");
		fclose(xmlfile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(xmlfile) != EXIT_SUCCESS) 
	{
		printf("Error closing XML file.\n");
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
