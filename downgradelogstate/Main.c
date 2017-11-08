// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double t_epoch = 0, latitude = 0, longitude = 0, altitude_AMSL = 0, heading = 0, COG = 0, SOG = 0, altitude_AGL = 0, pressure = 0, fluiddir = 0, fluidspeed = 0, range = 0, bearing = 0, elevation = 0, utc = 0;
	double t_app = 0, xhat = 0, yhat = 0, zhat = 0, phihat = 0, thetahat = 0, psihat = 0;
	double vrxhat = 0, vryhat = 0, vrzhat = 0, omegaxhat = 0, omegayhat = 0, omegazhat = 0;
	double xhat_err = 0, yhat_err = 0, zhat_err = 0, phihat_err = 0, thetahat_err = 0, psihat_err = 0;
	double vrxhat_err = 0, vryhat_err = 0, vrzhat_err = 0, omegaxhat_err = 0, omegayhat_err = 0, omegazhat_err = 0;
	double wx = 0, wy = 0, wz = 0, wpsi = 0, wd = 0, wu = 0, wagl = 0;
	double u = 0, uw = 0, uv = 0, ul = 0, up = 0, ur = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0, u6 = 0, u7 = 0, u8 = 0, u9 = 0, u10 = 0, u11 = 0, u12 = 0, u13 = 0, u14 = 0;
	double Energy_electronics = 0, Energy_actuators = 0;
	double vxyhat = 0, vxyhat_err = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "logstate.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : downgradelogstate file.csv.\n");
		printf("Default : downgradelogstate %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, "_downgraded.csv", szFileOutPath, szName);

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

	fprintf(fileout, 
		"t (in s);xhat;yhat;zhat;thetahat;vxyhat;omegahat;u1;u2;u3;u;uw;xhat-;xhat+;yhat-;yhat+;zhat-;zhat+;thetahat-;thetahat+;vxyhat-;vxyhat+;omegahat-;omegahat+;tv_sec;tv_usec;lathat;longhat;althat;headinghat;Energy_electronics;Energy_actuators;\n"
		); 

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf", 
			&t_epoch, &latitude, &longitude, &altitude_AMSL, &heading, &COG, &SOG, &altitude_AGL, &pressure, &fluiddir, &fluidspeed, &range, &bearing, &elevation, &utc,
			&t_app, &xhat, &yhat, &zhat, &phihat, &thetahat, &psihat,
			&vrxhat, &vryhat, &vrzhat, &omegaxhat, &omegayhat, &omegazhat,
			&xhat_err, &yhat_err, &zhat_err, &phihat_err, &thetahat_err, &psihat_err,
			&vrxhat_err, &vryhat_err, &vrzhat_err, &omegaxhat_err, &omegayhat_err, &omegazhat_err,
			&wx, &wy, &wz, &wpsi, &wd, &wu, &wagl, 
			&u, &uw, &uv, &ul, &up, &ur, &u1, &u2, &u3, &u4, &u5, &u6, &u7, &u8, &u9, &u10, &u11, &u12, &u13, &u14, 
			&Energy_electronics, &Energy_actuators) == 69)
		{
			vxyhat = sqrt(sqr(vrxhat)+sqr(vryhat));
			vxyhat_err = sqrt(sqr(vrxhat_err)+sqr(vryhat_err));
			fprintf(fileout, 
				"%f;"
				"%.3f;%.3f;%.3f;%f;"
				"%f;%f;"
				"%f;%f;%f;"
				"%f;%f;"
				"%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%f;%f;"
				"%f;%f;%f;%f;"
				"%d;%d;%.8f;%.8f;%.3f;%.2f;"
				"%.3f;%.3f;\n", 
				t_app, xhat, yhat, zhat, psihat, 
				vxyhat, omegazhat, 
				u1, u2, u3, 
				u, uw,
				xhat-xhat_err, xhat+xhat_err, yhat-yhat_err, yhat+yhat_err, zhat-zhat_err, zhat+zhat_err, psihat-psihat_err, psihat+psihat_err, 
				vxyhat-vxyhat_err, vxyhat+vxyhat_err, omegazhat-omegazhat_err, omegazhat+omegazhat_err,
				(int)t_epoch, (int)((t_epoch-(int)t_epoch)*1000000.0), latitude, longitude, altitude_AMSL, heading, 
				Energy_electronics, Energy_actuators);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the input file.\n");
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
