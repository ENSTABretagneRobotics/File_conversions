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
	FILE* kmlfile = NULL;
	FILE* csvfile = NULL;
	char line[4096];
	char* substr = NULL;
	int i = 0;
	double latitude = 0, longitude = 0, altitude = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	kmlfile = fopen("Waypoints.kml", "r");
	if (kmlfile == NULL)
	{
		printf("Unable to open kml file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	csvfile = fopen("Waypoints.csv", "w");
	if (csvfile == NULL)
	{
		printf("Unable to create csv file.\n");
		fclose(kmlfile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(kmlfile, line, sizeof(line)) != NULL) 
	{
		if (strstr(line, "<Point>") != NULL)
		{
			memset(line, 0, sizeof(line));
			if (fgets3(kmlfile, line, sizeof(line)) == NULL) break;
			if (strstr(line, "<altitudeMode>") != NULL)
			{
				memset(line, 0, sizeof(line));
				if (fgets3(kmlfile, line, sizeof(line)) == NULL) break;
			}
			substr = strstr(line, "<coordinates>");
			if (substr && 
				sscanf(substr, "<coordinates>%lf,%lf,%lf</coordinates>", 
				&longitude, &latitude, &altitude) == 3) 
			{
				fprintf(csvfile, "%f;%f;\n", latitude, longitude);
				i++;
			}
			else
			{
				printf("Error reading kml file.\n");
			}
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %d waypoints.\n", i);

	if (fclose(csvfile) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(kmlfile) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
		fclose(csvfile);
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
