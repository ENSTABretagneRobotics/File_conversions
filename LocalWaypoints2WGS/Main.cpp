// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Env.h"

#ifdef ENABLE_PROJ
PJ_CONTEXT* C = proj_context_create();
PJ* P;
PJ* P_for_GIS;
PJ_COORD a, b;

void latLonToXY(double lat_, double lon_, double& x_, double& y_)
{
	a = proj_coord(lon_, lat_, 0, 0);
	b = proj_trans(P, PJ_FWD, a);
	x_ = b.enu.e;
	y_ = b.enu.n;
}

void xyToLatLon(double x_, double y_, double& lat_, double& lon_)
{
	a = proj_coord(y_, x_, 0, 0);
	b = proj_trans(P, PJ_INV, a);
	lat_ = b.lp.phi;
	lon_ = b.lp.lam;
}

void setProj(double lon)
{
	int zone = (int)(std::floor((lon+180.0)/6)+1);
	std::string s = "+proj=utm +zone="+std::to_string(zone)+" +datum=WGS84";

	P = proj_create_crs_to_crs(C, "EPSG:4326", s.c_str(), NULL);
	P_for_GIS = proj_normalize_for_visualization(C, P);
	proj_destroy(P);
	P = P_for_GIS;
}
#endif // ENABLE_PROJ

int main()
{
	double latitude = 0, longitude = 0, altitude = 0;
	double x = 0, y = 0, z = 0;
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char line[MAX_BUF_LEN];
	int i = 0;
	int nbWPs = 0;

	LoadEnv();

#ifdef ENABLE_PROJ
	setProj(long_env);
#endif // ENABLE_PROJ

	filein = fopen("wplocal.txt", "r");
	fileout = fopen("Waypoints.csv", "w");

	if ((filein == NULL)||(fileout == NULL))
	{
		printf("File error.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL)
	{
		if (sscanf(line, "%lf %lf", &x, &y) != 2)
		{
			printf("Invalid file.\n");
			nbWPs = i;
			fclose(fileout);
			fclose(filein);
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}
#ifdef ENABLE_PROJ
		xyToLatLon(x, y, latitude, longitude);
		UNREFERENCED_PARAMETER(z);
		UNREFERENCED_PARAMETER(altitude);
#else
		EnvCoordSystem2GPS(lat_env, long_env, alt_env, angle_env, x, y, z, &latitude, &longitude, &altitude);
#endif // ENABLE_PROJ
		fprintf(fileout, "%.8f;%.8f;\n", latitude, longitude);
		i++;
		memset(line, 0, sizeof(line));
	}
	nbWPs = i;
	fclose(fileout);
	fclose(filein);

	if (nbWPs <= 0)
	{
		printf("No waypoints.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Successfully converted %d waypoints.\n", nbWPs);

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
