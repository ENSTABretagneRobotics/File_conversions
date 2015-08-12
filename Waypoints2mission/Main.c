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
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	int nbWPs = 0;
	int i = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "Waypoints.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : Waypoints2mission file.csv.\n");
		printf("Default : Waypoints2mission %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.241s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	// Remove the extension.
	for (i = strlen(szTemp)-1; i >= 0; i--) { if (szTemp[i] == '.') break; }
	if ((i > 0)&&(i < (int)strlen(szTemp))) memset(szTemp+i, 0, strlen(szTemp)-i);
	sprintf(szFileOutPath, "%.241s_mission.txt", szTemp);

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
		fprintf(fileout, "setmissionaction l%d\n", i-1);
		fprintf(fileout, "linefollowingtwgs %f %f %f %f %f\n", wpslat[i-1], wpslong[i-1], wpslat[i], wpslong[i], 31557600.0);
	}

	printf("Found %d lines.\n", nbWPs-1);

	if (fclose(fileout) != EXIT_SUCCESS) 
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
