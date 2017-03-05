// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Env.h"

int main()
{
	double latitude = 0, longitude = 0, altitude = 0;
	double x = 0, y = 0, z = 0;
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char line[MAX_BUF_LEN];
	int i = 0;
	int nbWPs = 0;

	LoadEnv();

	filein = fopen("wplocal.txt", "r");
	fileout = fopen("Waypoints.csv", "w");

	if ((filein == NULL)||(fileout == NULL))
	{
		printf("File error.\n");
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
		if (sscanf(line, "%lf %lf", &x, &y) != 2) 
		{
			printf("Invalid file.\n");
			nbWPs = i;
			fclose(fileout);
			fclose(filein);
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}
		EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, x, y, z, &latitude, &longitude, &altitude);
		fprintf(fileout, "%.8f;%.8f;\n", latitude, longitude);
		i++;
		memset(line, 0, sizeof(line));
	}
	nbWPs = i;
	fclose(fileout);
	fclose(filein);

	if (nbWPs <= 0)
	{
		printf("No waypoints.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Successfully converted %d waypoints.\n", nbWPs);

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
