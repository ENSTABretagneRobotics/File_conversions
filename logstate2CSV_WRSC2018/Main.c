// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#define CSV_2S_WRSC_FORMAT 0
#define CSV_3S_WRSC_FORMAT 1
//#define CSV_MS_WRSC_FORMAT 2

int CSV_format = CSV_3S_WRSC_FORMAT;

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double t_epoch = 0, latitude = 0, longitude = 0, altitude_AMSL = 0, heading = 0, COG = 0, SOG = 0, altitude_AGL = 0, pressure = 0, fluiddira = 0, fluidspeeda = 0, fluiddir = 0, fluidspeed = 0, range = 0, bearing = 0, elevation = 0, utc = 0;
	double t_app = 0, xhat = 0, yhat = 0, zhat = 0, phihat = 0, thetahat = 0, psihat = 0;
	double vrxhat = 0, vryhat = 0, vrzhat = 0, omegaxhat = 0, omegayhat = 0, omegazhat = 0, accrxhat = 0, accryhat = 0, accrzhat = 0;
	double xhat_err = 0, yhat_err = 0, zhat_err = 0, phihat_err = 0, thetahat_err = 0, psihat_err = 0;
	double vrxhat_err = 0, vryhat_err = 0, vrzhat_err = 0, omegaxhat_err = 0, omegayhat_err = 0, omegazhat_err = 0, accrxhat_err = 0, accryhat_err = 0, accrzhat_err = 0;
	double wx = 0, wy = 0, wz = 0, wphi = 0, wtheta = 0, wpsi = 0, wd = 0, wu = 0, wagl = 0;
	double uvx = 0, uvy = 0, uvz = 0, uwx = 0, uwy = 0, uwz = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0, u6 = 0, u7 = 0, u8 = 0, u9 = 0, u10 = 0, u11 = 0, u12 = 0, u13 = 0, u14 = 0;
	double Energy_electronics = 0, Energy_actuators = 0;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0;
	double second = 0; 
	time_t tt = 0;
	struct tm* timeptr = NULL;

	if (argc != 2)
	{
		strcpy(szFileInPath, "logstate.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : logstate2CSV_WRSC2018 file.csv.\n");
		printf("Default : logstate2CSV_WRSC2018 %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, "_wrsc2018.csv", szFileOutPath, szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

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

	switch (CSV_format)
	{
	case CSV_3S_WRSC_FORMAT:
		fprintf(fileout, "lat (in 1/10000000 deg),lon (in 1/10000000 deg),depth (in mm),hhmmsssdd\n");
		break;
	case CSV_2S_WRSC_FORMAT:
	default:
		fprintf(fileout, "lat (in 1/10000000 deg),lon (in 1/10000000 deg),depth (in mm),hhmmssdd\n");
		break;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf", 
			&t_epoch, &latitude, &longitude, &altitude_AMSL, &heading, &COG, &SOG, &altitude_AGL, &pressure, &fluiddira, &fluidspeeda, &fluiddir, &fluidspeed, &range, &bearing, &elevation, &utc,
			&t_app, &xhat, &yhat, &zhat, &phihat, &thetahat, &psihat,
			&vrxhat, &vryhat, &vrzhat, &omegaxhat, &omegayhat, &omegazhat, &accrxhat, &accryhat, &accrzhat,
			&xhat_err, &yhat_err, &zhat_err, &phihat_err, &thetahat_err, &psihat_err,
			&vrxhat_err, &vryhat_err, &vrzhat_err, &omegaxhat_err, &omegayhat_err, &omegazhat_err, &accrxhat_err, &accryhat_err, &accrzhat_err,
			&wx, &wy, &wz, &wphi, &wtheta, &wpsi, &wd, &wu, &wagl, 
			&uvx, &uvy, &uvz, &uwx, &uwy, &uwz, &u1, &u2, &u3, &u4, &u5, &u6, &u7, &u8, &u9, &u10, &u11, &u12, &u13, &u14, 
			&Energy_electronics, &Energy_actuators) == 79)
			&&(latitude != 0)&&(longitude != 0))
	{
			tt = (time_t)t_epoch;
			timeptr = localtime(&tt);
			if (timeptr != NULL)
			{
				year = timeptr->tm_year+1900;
				month = timeptr->tm_mon+1;
				day = timeptr->tm_mday;
				hour = timeptr->tm_hour;
				minute = timeptr->tm_min;
				second = timeptr->tm_sec;
			}

			switch (CSV_format)
			{
			case CSV_3S_WRSC_FORMAT:
				fprintf(fileout, "%d,%d,%d,%02d%02d%02d%01d%02d\n", (int)(10000000*latitude), (int)(10000000*longitude), (int)(1000.0*altitude_AGL*cos(phihat)), hour, minute, (int)second, (int)((second-(int)second)*10.0), day);
				break;
			case CSV_2S_WRSC_FORMAT:
			default:
				fprintf(fileout, "%d,%d,%d,%02d%02d%02d%02d\n", (int)(10000000*latitude), (int)(10000000*longitude), (int)(1000.0*altitude_AGL*cos(phihat)), hour, minute, (int)second, day);
				break;
			}

			i++;
		}
		else
		{
			printf("Skipping an invalid line in the CSV file.\n");
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
