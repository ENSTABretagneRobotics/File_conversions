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
	char szTemp[256];
	char line[4096];
	char* substr = NULL;
	int i = 0;
	double latitude = 0, longitude = 0, altitude = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "Waypoints.kml");
		printf("Warning : No parameter specified.\n");
		printf("Usage : KML2Waypoints file.kml.\n");
		printf("Default : KML2Waypoints %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.csv", szTemp);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open KML file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create CSV file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (strstr(line, "<Point>") != NULL)
		{
			memset(line, 0, sizeof(line));
			if (fgets3(filein, line, sizeof(line)) == NULL) break;
			if (strstr(line, "<altitudeMode>") != NULL)
			{
				memset(line, 0, sizeof(line));
				if (fgets3(filein, line, sizeof(line)) == NULL) break;
			}
			if (strstr(line, "<gx:drawOrder>") != NULL)
			{
				memset(line, 0, sizeof(line));
				if (fgets3(filein, line, sizeof(line)) == NULL) break;
			}
			substr = strstr(line, "<coordinates>");
			if (substr && 
				sscanf(substr, "<coordinates>%lf,%lf,%lf</coordinates>", 
				&longitude, &latitude, &altitude) == 3) 
			{
				fprintf(fileout, "%.8f;%.8f;\n", latitude, longitude);
				i++;
			}
			else
			{
				printf("Error reading KML file.\n");
			}
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %d waypoints.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing CSV file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing KML file.\n");
		fclose(fileout);
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
