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
	int i = 0;
	int i0, i1, i2, i3;
	double d0, d1, d2, d3;
	double latitude = 0, longitude = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "Waypoints.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : ArduPilot2Waypoints file.csv.\n");
		printf("Default : ArduPilot2Waypoints %.255s.\n", szFileInPath);
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
		printf("Unable to open ArduPilot file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create csv file.\n");
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
		if ((sscanf(line, "%d %d %d %d %lf %lf %lf %lf %lf %lf", 
			&i0, &i1, &i2, &i3, &d0, &d1, &d2, &d3, &latitude, &longitude) == 10)&&
			(latitude != 0)&&(longitude != 0))
		{
			fprintf(fileout, "%f;%f;\n", latitude, longitude);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the ArduPilot file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %d waypoints.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
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
