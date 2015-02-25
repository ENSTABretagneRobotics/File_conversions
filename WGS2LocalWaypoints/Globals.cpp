// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

// Environment parameters.
double angle_env = 0, lat_env = 0, long_env = 0, alt_env = 0;
int nb_circles = 0, nb_walls = 0;
vector<double> circles_x;
vector<double> circles_y;
vector<double> circles_r;
vector<double> walls_xa;
vector<double> walls_ya;
vector<double> walls_xb;
vector<double> walls_yb;

// Environment variables.
COORDSYSTEM csMap;
