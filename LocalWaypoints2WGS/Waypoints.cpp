// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Waypoints.h"

int LoadWaypoints(double wpslat[], double wpslong[], int* pNbWPs)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	int i = 0;

	memset(wpslat, 0, MAX_NB_WP*sizeof(double));
	memset(wpslong, 0, MAX_NB_WP*sizeof(double));
	*pNbWPs = 0;

	file = fopen("Waypoints.csv", "r");
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

int CheckWaypoints(double wpslat[], double wpslong[], int nbWPs)
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	double wpslattmp = 0;
	double wpslongtmp = 0;
	int i = 0;

	file = fopen("Waypoints.csv", "r");
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
			fclose(file);
			return EXIT_FAILURE;
		}
		wpslattmp = 0; wpslongtmp = 0;
		if (sscanf(line, "%lf;%lf", &wpslattmp, &wpslongtmp) == 2) 
		{
			if ((i >= nbWPs)||(fabs(wpslattmp-wpslat[i]) > DBL_EPSILON)||(fabs(wpslongtmp-wpslong[i]) > DBL_EPSILON))
			{
				printf("Waypoints file has changed.\n");
				if (fclose(file) != EXIT_SUCCESS) 
				{
					printf("Error closing waypoints file.\n");
					return EXIT_FAILURE;
				}
				return EXIT_CHANGED;
			}
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the waypoints file.\n");
		}
		memset(line, 0, sizeof(line));
	}
	if (i <= 0)
	{
		printf("Invalid waypoints file.\n");
		fclose(file);
		return EXIT_FAILURE;
	}
	if (i != nbWPs)
	{
		printf("Waypoints file has changed.\n");
		if (fclose(file) != EXIT_SUCCESS) 
		{
			printf("Error closing waypoints file.\n");
			return EXIT_FAILURE;
		}
		return EXIT_CHANGED;
	}
	if (fclose(file) != EXIT_SUCCESS) 
	{
		printf("Error closing waypoints file.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int GetCurrentWaypoint(int* pCurWP)
{

	// Should use an interprocess semaphore...

	FILE* file = NULL;

	*pCurWP = 0;

	file = fopen("CurWP.txt", "r");
	if (file == NULL)
	{
		printf("Current waypoint file not found.\n");
		return EXIT_FAILURE;
	}
	if (fscanf(file, "%d", pCurWP) != 1) 
	{
		// Wait and retry in case the file was being modified.
		mSleep(75);
		rewind(file);
		if (fscanf(file, "%d", pCurWP) != 1) 
		{
			printf("Error reading current waypoint file.\n");
			*pCurWP = 0;
			fclose(file);
			return EXIT_FAILURE;
		}
	}
	if (fclose(file) != EXIT_SUCCESS) 
	{
		printf("Error closing current waypoint file.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int SetCurrentWaypoint(int CurWP)
{

	// Should use an interprocess semaphore...

	FILE* file = NULL;

	file = fopen("CurWP.txt", "w");
	if (file == NULL)
	{
		printf("Unable to create current waypoint file.\n");
		return EXIT_FAILURE;
	}
	if (fprintf(file, "%d", CurWP) <= 0)
	{
		printf("Error writing current waypoint file.\n");
		fclose(file);
		return EXIT_FAILURE;
	}
	if (fclose(file) != EXIT_SUCCESS) 
	{
		printf("Error closing current waypoint file.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
