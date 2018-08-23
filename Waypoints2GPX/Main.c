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

	memset(wpslat, 0, MAX_NB_WP*sizeof(double));
	memset(wpslong, 0, MAX_NB_WP*sizeof(double));
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
		if ((sscanf(line, "%lf;%lf", &wpslat[i], &wpslong[i]) == 2)||
			(sscanf(line, "%lf %lf", &wpslat[i], &wpslong[i]) == 2))
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
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	int i = 0;
	int nbWPs = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "Waypoints.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : Waypoints2GPX file.csv.\n");
		printf("Default : Waypoints2GPX %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.gpx", szTemp);

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
		printf("Unable to create GPX file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(fileout, "<?xml version=\"1.0\"?>\n");
	fprintf(fileout, "<gpx version=\"1.1\" creator=\"OpenCPN\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\" xmlns:opencpn=\"http://www.opencpn.org\">\n");
	fprintf(fileout, "\t<rte>\n");
	fprintf(fileout, "\t\t<extensions>\n");
	fprintf(fileout, "\t\t\t<opencpn:guid>31350000-3648-4ee2-8c36-b0bb0c6c0000</opencpn:guid>\n");
	fprintf(fileout, "\t\t\t<opencpn:viz>1</opencpn:viz>\n");
	fprintf(fileout, "\t\t\t<opencpn:planned_speed>6.00</opencpn:planned_speed>\n");
	fprintf(fileout, "\t\t\t<opencpn:time_display>PC</opencpn:time_display>\n");
	fprintf(fileout, "\t\t\t<gpxx:RouteExtension>\n");
	fprintf(fileout, "\t\t\t\t<gpxx:IsAutoNamed>false</gpxx:IsAutoNamed>\n");
	fprintf(fileout, "\t\t\t</gpxx:RouteExtension>\n");
	fprintf(fileout, "\t\t</extensions>\n");

	printf("Converting...\n");
	for (i = 0; i < nbWPs; i++)
	{
		fprintf(fileout, "\t\t<rtept lat=\"%.9f\" lon=\"%.9f\">\n", wpslat[i], wpslong[i]);
		//fprintf(fileout, "\t\t\t<time>2018-08-23T14:36:57Z</time>\n");
		fprintf(fileout, "\t\t\t<name>%03d</name>\n", i+1);
		fprintf(fileout, "\t\t\t<sym>diamond</sym>\n");
		fprintf(fileout, "\t\t\t<type>WPT</type>\n");
		fprintf(fileout, "\t\t\t<extensions>\n");
		//fprintf(fileout, "\t\t\t\t<opencpn:guid>4646ffff-192e-4860-9629-4e0e71bcffff</opencpn:guid>\n");
		fprintf(fileout, "\t\t\t\t<opencpn:auto_name>1</opencpn:auto_name>\n");
		fprintf(fileout, "\t\t\t\t<opencpn:arrival_radius>0.050</opencpn:arrival_radius>\n");
		fprintf(fileout, "\t\t\t\t<opencpn:waypoint_range_rings visible=\"false\" number=\"0\" step=\"1\" units=\"0\" colour=\"#FF0000\" />\n");
		fprintf(fileout, "\t\t\t</extensions>\n");
		fprintf(fileout, "\t\t</rtept>\n");
	}

	fprintf(fileout, "\t</rte>\n");
	fprintf(fileout, "</gpx>\n");

	printf("Found %d waypoints.\n", nbWPs);

	if (fclose(fileout) != EXIT_SUCCESS)
	{
		printf("Error closing GPX file.\n");
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
