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
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	double x = 0, y = 0, z = 0;
	int nbWPs = 0;
	FILE* wplocalfile = NULL;
	int i = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadEnv();

	if (LoadWaypoints(wpslat, wpslong, &nbWPs) != EXIT_SUCCESS)
	{
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	wplocalfile = fopen("wplocal.txt", "w");
	if (wplocalfile == NULL)
	{
		printf("Unable to create wplocal file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");
	for (i = 0; i < nbWPs; i++)
	{
		GPS2EnvCoordSystem(lat_env, long_env, alt_env, angle_env, wpslat[i], wpslong[i], 0, &x, &y, &z);
		fprintf(wplocalfile, "%f %f\n", x, y);
	}

	printf("Found %d waypoints.\n", nbWPs);

	if (fclose(wplocalfile) != EXIT_SUCCESS) 
	{
		printf("Error closing wplocal file.\n");
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
