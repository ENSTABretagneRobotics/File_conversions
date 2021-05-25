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
	double t_epoch = 0, latitude = 0, longitude = 0, altitude_AMSL = 0, heading = 0, COG = 0, SOG = 0, altitude_AGL = 0, pressure = 0, fluiddira = 0, fluidspeeda = 0, fluiddir = 0, fluidspeed = 0, range = 0, bearing = 0, elevation = 0, utc = 0;
	double t_app = 0, xhat = 0, yhat = 0, zhat = 0, phihat = 0, thetahat = 0, psihat = 0;
	double vrxhat = 0, vryhat = 0, vrzhat = 0, omegaxhat = 0, omegayhat = 0, omegazhat = 0, accrxhat = 0, accryhat = 0, accrzhat = 0;
	double xhat_err = 0, yhat_err = 0, zhat_err = 0, phihat_err = 0, thetahat_err = 0, psihat_err = 0;
	double vrxhat_err = 0, vryhat_err = 0, vrzhat_err = 0, omegaxhat_err = 0, omegayhat_err = 0, omegazhat_err = 0, accrxhat_err = 0, accryhat_err = 0, accrzhat_err = 0;
	double wx = 0, wy = 0, wz = 0, wphi = 0, wtheta = 0, wpsi = 0, wd = 0, wu = 0, wagl = 0;
	double uvx = 0, uvy = 0, uvz = 0, uwx = 0, uwy = 0, uwz = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0, u6 = 0, u7 = 0, u8 = 0, u9 = 0, u10 = 0, u11 = 0, u12 = 0, u13 = 0, u14 = 0;
	double EPU1 = 0, EPU2 = 0;
	double vxyhat = 0, vxyhat_err = 0;
	double xhat_inf = 0, xhat_sup = 0, yhat_inf = 0, yhat_sup = 0, zhat_inf = 0, zhat_sup = 0, psihat_inf = 0, psihat_sup = 0, vxyhat_inf = 0, vxyhat_sup = 0, omegazhat_inf = 0, omegazhat_sup = 0;
	int tv_sec = 0, tv_usec = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "logstate.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : upgradelogstate file.csv.\n");
		printf("Default : upgradelogstate %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, "_upgraded.csv", szFileOutPath, szName);

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
		"t_epoch (in s);lat;lon;alt_amsl;hdg;cog;sog;alt_agl;pressure (in bar);fluiddira (in deg);fluidspeeda;fluiddir (in deg);fluidspeed;range;bearing (in deg);elevation (in deg);utc (in ms);"
		"t_app (in s);xhat;yhat;zhat;phihat;thetahat;psihat;vrxhat;vryhat;vrzhat;omegaxhat;omegayhat;omegazhat;accrxhat;accryhat;accrzhat;"
		"xhat_err;yhat_err;zhat_err;phihat_err;thetahat_err;psihat_err;vrxhat_err;vryhat_err;vrzhat_err;omegaxhat_err;omegayhat_err;omegazhat_err;accrxhat_err;accryhat_err;accrzhat_err;"
		"wx;wy;wz;wphi;wtheta;wpsi;wd;wu;wagl;"
		"uvx;uvy;uvz;uwx;uwy;uwz;u1;u2;u3;u4;u5;u6;u7;u8;u9;u10;u11;u12;u13;u14;"
		"EPU1;EPU2;\n"
		); 

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%d;%d;%lf;%lf;%lf;%lf;"
			"%lf;%lf", 
			&t_app, &xhat, &yhat, &zhat, &psihat, &vxyhat, &omegazhat,
			&u1, &u2, &u3, &uvx, &uwz, 
			&xhat_inf, &xhat_sup, &yhat_inf, &yhat_sup, &zhat_inf, &zhat_sup, 
			&psihat_inf, &psihat_sup, &vxyhat_inf, &vxyhat_sup, &omegazhat_inf, &omegazhat_sup,
			&tv_sec, &tv_usec, &latitude, &longitude, &altitude_AMSL, &heading, 
			&EPU1, &EPU2) == 32)||
			(sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%d;%d;%lf;%lf;%lf;%lf", 
			&t_app, &xhat, &yhat, &zhat, &psihat, &vxyhat, &omegazhat,
			&u1, &u2, &u3, &uvx, &uwz, 
			&xhat_inf, &xhat_sup, &yhat_inf, &yhat_sup, &zhat_inf, &zhat_sup, 
			&psihat_inf, &psihat_sup, &vxyhat_inf, &vxyhat_sup, &omegazhat_inf, &omegazhat_sup,
			&tv_sec, &tv_usec, &latitude, &longitude, &altitude_AMSL, &heading) == 30))
		{
			t_epoch = tv_sec+0.000001*tv_usec;
			COG = 0;
			SOG = 0;
			pressure = 0;
			//utc = 1000.0*tv_sec+0.001*tv_usec;
			vrxhat = vxyhat*cos(psihat);
			vryhat = vxyhat*sin(psihat);
			xhat_err = (xhat_sup-xhat_inf)/2.0;
			yhat_err = (yhat_sup-yhat_inf)/2.0;
			zhat_err = (zhat_sup-zhat_inf)/2.0;
			vxyhat_err = (vxyhat_sup-vxyhat_inf)/2.0;
			vrxhat_err = vxyhat_err*cos(psihat);
			vryhat_err = vxyhat_err*sin(psihat);
			psihat_err = (psihat_sup-psihat_inf)/2.0;
			omegazhat_err = (omegazhat_sup-omegazhat_inf)/2.0;
			fprintf(fileout, 			
				"%f;%.8f;%.8f;%.3f;%.2f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%.3f;%.3f;%.3f;%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%.3f;%.3f;%.3f;%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%.3f;%.3f;\n", 
				t_epoch, latitude, longitude, altitude_AMSL, heading, COG, SOG, altitude_AGL, pressure, fluiddira, fluidspeeda, fluiddir, fluidspeed, range, bearing, elevation, utc,
				t_app, xhat, yhat, zhat, phihat, thetahat, psihat,
				vrxhat, vryhat, vrzhat, omegaxhat, omegayhat, omegazhat, accrxhat, accryhat, accrzhat,
				xhat_err, yhat_err, zhat_err, phihat_err, thetahat_err, psihat_err,
				vrxhat_err, vryhat_err, vrzhat_err, omegaxhat_err, omegayhat_err, omegazhat_err, accrxhat_err, accryhat_err, accrzhat_err,
				wx, wy, wz, wphi, wtheta, wpsi, wd, wu, wagl, 
				uvx, uvy, uvz, uwx, uwy, uwz, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14, 
				EPU1, EPU2);
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
