// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Env.h"

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double t = 0, latitude = 0, longitude = 0, altitude = 0, distance = 0;
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	// Temporary buffers for sscanf().
	int tv_sec = 0, tv_usec = 0;
	unsigned int hex = 0;
	double x = 0, y = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "mdm.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : MDM2CSV file.csv.\n");
		printf("Default : MDM2CSV %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, "_converted.csv", szFileOutPath, szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadEnv();

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open input file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create output file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(fileout, "t (in s);latitude;longitude;distance (in m);\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%d;%d;7;r;%x (%lf,%lf)", &tv_sec, &tv_usec, &hex, &x, &y) == 5)
		{
			t =  tv_sec+0.000001*tv_usec;
			x2 = x1;
			y2 = y1;
			x1 = x;
			y1 = y;
			// If 2 successive positions are the same, they are considered valid.
			if ((x1 == x2)&&(y1 == y2))
			{
				EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, x, y, 0, &latitude, &longitude, &altitude);
			}
			else
			{
				latitude = 0; 
				longitude = 0;
			}
		}
		else if (sscanf(line, "\rRange=%lfm", &distance) == 1)
		{
			if ((latitude != 0)&&(longitude != 0))
			{
				fprintf(fileout, "%f;%.8f;%.8f;%f;\n", t, latitude, longitude, distance);
				i++;
			}
		}
		else
		{
			//printf("Skipping an invalid line in the csv file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing output file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing input file.\n");
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
