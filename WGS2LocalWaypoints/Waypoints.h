// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include "Utils.h"

#define MAX_NB_WP 16384

int LoadWaypoints(double wpslat[], double wpslong[], int* pNbWPs);
int CheckWaypoints(double wpslat[], double wpslong[], int nbWPs);
int GetCurrentWaypoint(int* pCurWP);
int SetCurrentWaypoint(int CurWP);

#endif // WAYPOINTS_H
