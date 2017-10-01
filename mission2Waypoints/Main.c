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
	unsigned int i = 0;
	double lat0 = 0, long0 = 0, alt0 = 0, lat1 = 0, long1 = 0, alt1 = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "mission.txt");
		printf("Warning : No parameter specified.\n");
		printf("Usage : mission2Waypoints file.txt.\n");
		printf("Default : mission2Waypoints %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.241s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.241s.csv", szTemp);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open mission file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create waypoints file.\n");
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
		if (sscanf(line, "linefollowingwgs %lf %lf %lf %lf", &lat0, &long0, &lat1, &long1) == 4)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			fprintf(fileout, "%.8f;%.8f;\n", lat1, long1);
			i += 2;
		}
		else if (sscanf(line, "linefollowingtwgs %lf %lf %lf %lf", &lat0, &long0, &lat1, &long1) == 4)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			fprintf(fileout, "%.8f;%.8f;\n", lat1, long1);
			i += 2;
		}
		else if (sscanf(line, "linefollowingwgsstation %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}
		else if (sscanf(line, "gotoxywgs %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}
		else if (sscanf(line, "gotoxytwgs %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}

		// Local waypoints are treated the same...

		else if (sscanf(line, "linefollowing %lf %lf %lf %lf", &lat0, &long0, &lat1, &long1) == 4)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			fprintf(fileout, "%.8f;%.8f;\n", lat1, long1);
			i += 2;
		}
		else if (sscanf(line, "linefollowingt %lf %lf %lf %lf", &lat0, &long0, &lat1, &long1) == 4)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			fprintf(fileout, "%.8f;%.8f;\n", lat1, long1);
			i += 2;
		}
		else if (sscanf(line, "linefollowingstation %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}
		else if (sscanf(line, "gotoxy %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}
		else if (sscanf(line, "gotoxyt %lf %lf", &lat0, &long0) == 2)
		{
			fprintf(fileout, "%.8f;%.8f;\n", lat0, long0);
			i++;
		}
		else
		{
			//printf("Skipping an invalid line in the mission file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u waypoints.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS)
	{
		printf("Error closing waypoints file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS)
	{
		printf("Error closing mission file.\n");
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
