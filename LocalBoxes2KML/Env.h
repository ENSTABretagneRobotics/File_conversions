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
	int i = 0;
	double d0 = 0, d1 = 0, d2 = 0, d3 = 0;

	memset(line, 0, sizeof(line));

	// Default values.
	angle_env = M_PI/2.0-90.0*M_PI/180.0;
	lat_env = 0;
	long_env = 0;
	alt_env = 0;
	nb_circles = 0;
	circles_x.clear();
	circles_y.clear();
	circles_r.clear();
	nb_walls = 0;
	walls_xa.clear();
	walls_ya.clear();
	walls_xb.clear();
	walls_yb.clear();
	csMap.xMin = -10; csMap.xMax = 10; csMap.yMin = -10; csMap.yMax = 10; 

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

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &nb_circles) != 1) printf("Invalid configuration file.\n");
		for (i = 0; i < nb_circles; i++)
		{
			if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
			if (sscanf(line, "%lf %lf %lf", &d0, &d1, &d2) != 3) printf("Invalid configuration file.\n");
			circles_x.push_back(d0);
			circles_y.push_back(d1);
			circles_r.push_back(d2);
		}

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &nb_walls) != 1) printf("Invalid configuration file.\n");
		for (i = 0; i < nb_walls; i++)
		{
			if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
			if (sscanf(line, "%lf %lf %lf %lf", &d0, &d1, &d2, &d3) != 4) printf("Invalid configuration file.\n");
			walls_xa.push_back(d0);
			walls_ya.push_back(d1);
			walls_xb.push_back(d2);
			walls_yb.push_back(d3);
		}

		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &csMap.xMin) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &csMap.xMax) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &csMap.yMin) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &csMap.yMax) != 1) printf("Invalid configuration file.\n");

		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	if ((csMap.xMin == csMap.xMax)||(csMap.yMin == csMap.yMax))
	{
		printf("Invalid parameter : csMap.\n");
		csMap.xMin = -10; csMap.xMax = 10; csMap.yMin = -10; csMap.yMax = 10; 
	}

	return EXIT_SUCCESS;
}

inline int UnloadEnv()
{
	// Default values.
	angle_env = M_PI/2.0-90.0*M_PI/180.0;
	lat_env = 0;
	long_env = 0;
	alt_env = 0;
	nb_circles = 0;
	circles_x.clear();
	circles_y.clear();
	circles_r.clear();
	nb_walls = 0;
	walls_xa.clear();
	walls_ya.clear();
	walls_xb.clear();
	walls_yb.clear();
	csMap.xMin = -10; csMap.xMax = 10; csMap.yMin = -10; csMap.yMax = 10; 

	return EXIT_SUCCESS;
}

#endif // ENV_H
