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
	//double d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, d9 = 0, d10 = 0; 
	double t = 0, latitude = 0, longitude = 0, altitude = 0, v = 0, bearing = 0, psi  =0, theta = 0, phi = 0, x = 0, y = 0, z = 0;
	FILE* logstatefile = NULL;
	FILE* mtfile = NULL;
	FILE* alldatafile = NULL;
	char line[4096];
	//char line2[4096];
	unsigned int i = 0;
	struct tm* timeptr = NULL;
	time_t tt;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadEnv();

	logstatefile = fopen("logstate.csv", "r");
	if (logstatefile == NULL)
	{
		printf("Unable to open csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	mtfile = fopen("MT.csv", "r");
	if (mtfile == NULL)
	{
		printf("Unable to open csv file.\n");
		fclose(logstatefile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	alldatafile = fopen("alldata.csv", "w");
	if (alldatafile == NULL)
	{
		printf("Unable to create csv file.\n");
		fclose(mtfile);
		fclose(logstatefile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	tt = (time_t)time(&tt);
	timeptr = localtime(&tt);
	memset(timeptr, 0, sizeof(struct tm));

	// CHANGE HERE (UTC)...

	timeptr->tm_year = 2014;
	timeptr->tm_mon = 10;
	timeptr->tm_mday = 3;
	timeptr->tm_hour = 11;
	timeptr->tm_min = 15;
	timeptr->tm_sec = 56;

	timeptr->tm_isdst = 0;
	timeptr->tm_year-=1900;
	timeptr->tm_mon--;

	tt = _mkgmtime(timeptr);

	if (tt == (time_t)-1) printf("Time conversion failed.\n");

	fprintf(alldatafile, "t;latitude;longitude;altitude;v;bearing;psi;theta;phi;x;y;z\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(logstatefile, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf", &t, &x, &y, &z, &bearing, &v) == 6) 
		{
			EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, x, y, z, &latitude, &longitude, &altitude);
			t = (t+(double)tt)*1000;
			psi = fmod_2PI(bearing+angle_env);
			fprintf(alldatafile, 
				"%lf;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n", 
				t, latitude, longitude, altitude, v, fmod_2PI(bearing+angle_env), psi, theta, phi, x, y, z);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the csv file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u lines.\n", i);

	if (fclose(alldatafile) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
		fclose(mtfile);
		fclose(logstatefile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(mtfile) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
		fclose(logstatefile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(logstatefile) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
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
