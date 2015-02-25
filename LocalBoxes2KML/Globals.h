// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef GLOBALS_H
#define GLOBALS_H

#include "Waypoints.h"

#include <vector>

using namespace std;

// Environment parameters.
extern double angle_env, lat_env, long_env, alt_env;
extern int nb_circles, nb_walls;
extern vector<double> circles_x;
extern vector<double> circles_y;
extern vector<double> circles_r;
extern vector<double> walls_xa;
extern vector<double> walls_ya;
extern vector<double> walls_xb;
extern vector<double> walls_yb;

// Environment variables.
extern COORDSYSTEM csMap;

#endif // GLOBALS_H
