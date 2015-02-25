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
	FILE* missionfile = NULL;
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

	missionfile = fopen("mission.txt", "w");
	if (missionfile == NULL)
	{
		printf("Unable to create mission file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");
	for (i = 1; i < nbWPs; i++)
	{
		fprintf(missionfile, "setmissionaction l%d\n", i-1);
		fprintf(missionfile, "linefollowingtwgs %f %f %f %f %f\n", wpslat[i-1], wpslong[i-1], wpslat[i], wpslong[i], 31557600.0);
	}

	printf("Found %d lines.\n", nbWPs-1);

	if (fclose(missionfile) != EXIT_SUCCESS) 
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
