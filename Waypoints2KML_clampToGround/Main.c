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
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	int nbWPs = 0;
	FILE* kmlfile = NULL;
	int i = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	if (LoadWaypoints(wpslat, wpslong, &nbWPs) != EXIT_SUCCESS)
	{
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	kmlfile = fopen("Waypoints.kml", "w");
	if (kmlfile == NULL)
	{
		printf("Unable to create kml file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(kmlfile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(kmlfile, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	fprintf(kmlfile, "<Document>\n");
	fprintf(kmlfile, "\t<Style id=\"sn_ylw-pushpin\">\n\t\t<LineStyle>\n\t\t\t<color>ff0000ff</color>\n\t\t\t<width>2</width>\n\t\t</LineStyle>\n\t</Style>\n");

	if (nbWPs >= 1)
	{
		fprintf(kmlfile, "\t<Placemark>\n\t\t<name>%d</name>\n\t\t<Point>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n", 0);
		fprintf(kmlfile, "\t\t\t<coordinates>%f,%f,%f</coordinates>\n", wpslong[0], wpslat[0], 0.0);
		fprintf(kmlfile, "\t\t</Point>\n\t</Placemark>\n");
	}
	printf("Converting...\n");
	for (i = 1; i < nbWPs; i++)
	{
		fprintf(kmlfile, "\t<Placemark>\n\t\t<name>%d</name>\n\t\t<Point>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n", i);
		fprintf(kmlfile, "\t\t\t<coordinates>%f,%f,%f</coordinates>\n", wpslong[i], wpslat[i], 0.0);
		fprintf(kmlfile, "\t\t</Point>\n\t</Placemark>\n");

		fprintf(kmlfile, "\t<Placemark>\n\t\t<name>%d-%d</name>\n", i-1, i);
		fprintf(kmlfile, "\t\t<styleUrl>#sn_ylw-pushpin</styleUrl>\n\t\t<LineString>\n\t\t\t<tessellate>1</tessellate>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\t\t\t<coordinates>\n");
		fprintf(kmlfile, "\t\t\t\t%f,%f,%f %f,%f,%f\n", wpslong[i-1], wpslat[i-1], 0.0, wpslong[i], wpslat[i], 0.0);
		fprintf(kmlfile, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
	}

	fprintf(kmlfile, "</Document>\n</kml>\n");

	printf("Found %d lines.\n", nbWPs-1);

	if (fclose(kmlfile) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
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
