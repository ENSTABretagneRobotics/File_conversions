// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef ENV_H
#define ENV_H

#include "Globals.h"

inline int LoadEnv()
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];
	double d0 = 0;

	memset(line, 0, sizeof(line));

	// Default values.
	angle_env = M_PI/2.0-90.0*M_PI/180.0;
	lat_env = 0;
	long_env = 0;
	alt_env = 0;

	file = fopen("env.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &d0) != 1) printf("Invalid configuration file.\n");
		angle_env = M_PI/2.0-d0*M_PI/180.0;

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &lat_env) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &long_env) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &alt_env) != 1) printf("Invalid configuration file.\n");

		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	return EXIT_SUCCESS;
}

#endif // !ENV_H
