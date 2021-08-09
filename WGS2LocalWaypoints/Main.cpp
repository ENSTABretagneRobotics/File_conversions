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
	double wpslat[MAX_NB_WP];
	double wpslong[MAX_NB_WP];
	double x = 0, y = 0, z = 0;
	int nbWPs = 0;
	FILE* wplocalfile = NULL;
	int i = 0;

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	LoadEnv();

#ifdef ENABLE_PROJ
	setProj(long_env);
#endif // ENABLE_PROJ

	if (LoadWaypoints(wpslat, wpslong, &nbWPs) != EXIT_SUCCESS)
	{
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	wplocalfile = fopen("wplocal.txt", "w");
	if (wplocalfile == NULL)
	{
		printf("Unable to create wplocal file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");
	for (i = 0; i < nbWPs; i++)
	{
#ifdef ENABLE_PROJ
		latLonToXY(wpslat[i], wpslong[i], x, y);
		UNREFERENCED_PARAMETER(z);
#else
		GPS2EnvCoordSystem(lat_env, long_env, alt_env, angle_env, wpslat[i], wpslong[i], 0, &x, &y, &z);
#endif // ENABLE_PROJ
		fprintf(wplocalfile, "%f %f\n", x, y);
	}

	printf("Found %d waypoints.\n", nbWPs);

	if (fclose(wplocalfile) != EXIT_SUCCESS)
	{
		printf("Error closing wplocal file.\n");
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
