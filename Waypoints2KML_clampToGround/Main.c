// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#define MAX_NB_WP 256

int LoadWaypointsEx(char* szFileInPath, double wpslat[], double wpslong[], int* pNbWPs)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	int i = 0;

	memset(wpslat, 0, MAX_NB_WP);
	memset(wpslong, 0, MAX_NB_WP);
	*pNbWPs = 0;

	file = fopen(szFileInPath, "r");
	if (file == NULL)
	{
		printf("Waypoints file not found.\n");
		return EXIT_FAILURE;
	}

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(file, line, sizeof(line)) != NULL) 
	{
		if (i >= MAX_NB_WP) 
		{
			printf("Too many waypoints.\n");
			*pNbWPs = i;
			fclose(file);
			return EXIT_FAILURE;
		}
		if (sscanf(line, "%lf;%lf", &wpslat[i], &wpslong[i]) == 2) 
		{
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the waypoints file.\n");
		}
		memset(line, 0, sizeof(line));
	}
	*pNbWPs = i;
	if (*pNbWPs <= 0)
	{
		printf("Invalid waypoints file.\n");
		*pNbWPs = 0;
		fclose(file);
		return EXIT_FAILURE;
	}
	if (fclose(file) != EXIT_SUCCESS) 
	{
		printf("Error closing waypoints file.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	//FILE* filein = NULL;
	FILE* fileout = NULL;
	char szTemp[256];
	int idx = 0;
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	int i = 0;
	int nbWPs = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "Waypoints.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : Waypoints2KML file.csv.\n");
		printf("Default : Waypoints2KML %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	// Remove the extension.
	for (idx = strlen(szTemp)-1; idx >= 0; idx--) { if (szTemp[idx] == '.') break; }
	if ((idx > 0)&&(idx < (int)strlen(szTemp))) memset(szTemp+idx, 0, strlen(szTemp)-idx);
	sprintf(szFileOutPath, "%.249s.kml", szTemp);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	if (LoadWaypointsEx(szFileInPath, wpslat, wpslong, &nbWPs) != EXIT_SUCCESS)
	{
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
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(fileout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileout, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	fprintf(fileout, "<Document>\n");
	fprintf(fileout, "\t<Style id=\"sn_ylw-pushpin\">\n\t\t<LineStyle>\n\t\t\t<color>ff0000ff</color>\n\t\t\t<width>2</width>\n\t\t</LineStyle>\n\t</Style>\n");

	if (nbWPs >= 1)
	{
		fprintf(fileout, "\t<Placemark>\n\t\t<name>%d</name>\n\t\t<Point>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n", 0);
		fprintf(fileout, "\t\t\t<coordinates>%f,%f,%f</coordinates>\n", wpslong[0], wpslat[0], 0.0);
		fprintf(fileout, "\t\t</Point>\n\t</Placemark>\n");
	}
	printf("Converting...\n");
	for (i = 1; i < nbWPs; i++)
	{
		fprintf(fileout, "\t<Placemark>\n\t\t<name>%d</name>\n\t\t<Point>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n", i);
		fprintf(fileout, "\t\t\t<coordinates>%f,%f,%f</coordinates>\n", wpslong[i], wpslat[i], 0.0);
		fprintf(fileout, "\t\t</Point>\n\t</Placemark>\n");

		fprintf(fileout, "\t<Placemark>\n\t\t<name>%d-%d</name>\n", i-1, i);
		fprintf(fileout, "\t\t<styleUrl>#sn_ylw-pushpin</styleUrl>\n\t\t<LineString>\n\t\t\t<tessellate>1</tessellate>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n\t\t\t<coordinates>\n");
		fprintf(fileout, "\t\t\t\t%f,%f,%f %f,%f,%f\n", wpslong[i-1], wpslat[i-1], 0.0, wpslong[i], wpslat[i], 0.0);
		fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
	}

	fprintf(fileout, "</Document>\n</kml>\n");

	printf("Found %d lines.\n", nbWPs-1);

	if (fclose(fileout) != EXIT_SUCCESS) 
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
