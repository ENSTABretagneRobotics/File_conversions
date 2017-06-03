// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
// Disable some Visual Studio warnings.
#ifndef CRT_SECURE_NO_DEPRECATE
#define CRT_SECURE_NO_DEPRECATE
#endif // CRT_SECURE_NO_DEPRECATE
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

// Enable additional features in math.h.
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#endif // _WIN32

// C Standard Library headers.
#include <assert.h> // Diagnostics.
#include <ctype.h> // Character Class Tests.
#include <errno.h> // Error Codes Reported by (Some) Library Functions.
#include <float.h> // Implementation-defined Floating-Point Limits.
#include <limits.h> // Implementation-defined Limits.
#include <locale.h> // Locale-specific Information.
#include <math.h> // Mathematical Functions.
#include <setjmp.h> // Non-local Jumps.
#include <signal.h> // Signals.
#include <stdarg.h> // Variable Argument Lists.
#include <stddef.h> // Definitions of General Use.
#include <stdio.h> // Input and Output.
#include <stdlib.h> // Utility functions.
#include <string.h> // String functions.
#include <time.h> // Time and Date functions.

#include <fcntl.h>

#ifdef _WIN32
// Prevent Winsock.h from being included by the Windows.h header.
// This must be done if we plan to include Winsock2.h.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#else 
// Linux useful headers.
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdint.h> // Integer types with size constraints.

// POSIX threads.
//#include <pthread.h>

// Socket related definitions.
typedef int SOCKET;

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

#define SD_RECEIVE SHUT_RD
#define SD_SEND SHUT_WR
#define SD_BOTH SHUT_RDWR

#define closesocket(sock) close((sock))

// CLOCK_MONOTONIC is a clock that cannot be set and represents monotonic time since 
// some unspecified starting point.
// CLOCK_MONOTONIC_RAW is available since Linux 2.6.28 and provides access to a raw 
// hardware-based time that is not subject to NTP adjustments.
#ifndef CLOCK_MONOTONIC_RAW 
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif // CLOCK_MONOTONIC_RAW 

// To change terminal options.
#include <termios.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif // max
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif // min
#endif // _WIN32

#ifndef __cplusplus
#ifdef _MSC_VER
#define inline __inline
#endif // _MSC_VER
#ifdef __GNUC__
// extern __inline__ in ws2tcpip.h for GNU?
#define inline static __inline__
#endif // __GNUC__
#endif // __cplusplus

#ifdef __GNUC__
#undef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (void)(P)
#endif // __GNUC__

#define EXIT_TIMEOUT 2
#define EXIT_INVALID_PARAMETER 3
#define EXIT_NAME_TOO_LONG 4
#define EXIT_OUT_OF_MEMORY 5
#define EXIT_OBJECT_NONSIGNALED 6
#define EXIT_KILLED_THREAD 7
#define EXIT_CANCELED_THREAD 8
#define EXIT_IO_PENDING 9
#define EXIT_KILLED_PROCESS 10
#define EXIT_CHANGED 11
#define EXIT_NOT_CHANGED 12
#define EXIT_FOUND 13
#define EXIT_NOT_FOUND 14
#define EXIT_NOT_IMPLEMENTED 15

#define MAX_BUF_LEN 256
#define DEFAULT_SOCK_TIMEOUT 10000

// Earth radius in m.
#define EARTH_RADIUS 6371000

#define EAST_NORTH_UP_COORDINATE_SYSTEM 0
#define NORTH_EAST_DOWN_COORDINATE_SYSTEM 1
#define NORTH_WEST_UP_COORDINATE_SYSTEM 2

int initudpcli(SOCKET* pSock, char* address, char* port);
int inittcpcli(SOCKET* pSock, char* address, char* port);
int setsockettimeouts(SOCKET sock, int timeout);
int waitforsocket(SOCKET sock, int timeout);
int purgesocket(SOCKET sock);
int sendtoall(SOCKET sock, char* sendbuf, int sendbuflen, struct sockaddr* sa, int salen);
int recvfromall(SOCKET sock, char* recvbuf, int recvbuflen, struct sockaddr* sa, int* pSalen);
int sendall(SOCKET sock, char* sendbuf, int sendbuflen);
int recvall(SOCKET sock, char* recvbuf, int recvbuflen);
int recvlatest(SOCKET sock, char* recvbuf, int recvbuflen);
int recvatleastuntil(SOCKET sock, char* recvbuf, char endchar, int maxrecvbuflen);
int recvuntil(SOCKET sock, char* recvbuf, char endchar, int maxrecvbuflen);
int releaseudpcli(SOCKET sock);
int releasetcpcli(SOCKET sock);
int initudpsrv(SOCKET* pSock, char* address, char* port, int timeout);
int inittcpsrv(SOCKET* pSock, char* address, char* port, int maxnbcli, int timeout);
int waitforclifortcpsrv(SOCKET socksrv, SOCKET* pSockCli, int timeout);
int disconnectclifromtcpsrv(SOCKET sock);
int releaseudpsrv(SOCKET sock);
int releasetcpsrv(SOCKET sock);
int LaunchUDPSrv(char* port, int (*handlecli)(SOCKET, void*), void* pParam);
int LaunchSingleCliTCPSrv(char* port, int (*handlecli)(SOCKET, void*), void* pParam);

// A NMEA sentence begins with a '$' and ends with a carriage return/line feed sequence and can 
// be no longer than 80 characters of visible text (plus the line terminators). The data is contained 
// within this single line with data items separated by commas. The data itself is just ascii text. 
// There is a provision for a checksum at the end of each sentence which may or may not be checked by 
// the unit that reads the data. The checksum field consists of a '*' and two hex digits representing 
// an 8 bit exclusive OR of all characters between, but not including, the '$' and '*'.

// Maximum number of characters of a NMEA sentence (excluding the line terminators CR and LF).
#define MAX_NB_BYTES_NMEA_SENTENCE 80

#define MAX_NB_BYTES_CHECKSUM_NMEA 3

char* FindNMEASentence(char sentencebegin[7], char* str);
char* FindLatestNMEASentence(char sentencebegin[7], char* str);

void mSleep(long Milliseconds);

#ifdef _WIN32
struct CHRONO
{
	LARGE_INTEGER Start;
	LARGE_INTEGER Finish;
	LARGE_INTEGER Duration;
	LARGE_INTEGER Frequency;
	int Suspended; // Used to know if the chronometer is currently suspended.
};
typedef struct CHRONO CHRONO;
#else 
struct CHRONO
{
	struct timespec Start;
	struct timespec Finish;
	struct timespec Duration;
	struct timespec Frequency;
	int Suspended; // Used to know if the chronometer is currently suspended.
};
typedef struct CHRONO CHRONO;
#endif // _WIN32

int StartChrono(CHRONO* pChrono);
int SuspendChrono(CHRONO* pChrono, double* pDuration);
double SuspendChronoQuick(CHRONO* pChrono);
int ResumeChrono(CHRONO* pChrono);
int GetTimeElapsedChrono(CHRONO* pChrono, double* pDuration);
double GetTimeElapsedChronoQuick(CHRONO* pChrono);
int StopChrono(CHRONO* pChrono, double* pDuration);
double StopChronoQuick(CHRONO* pChrono);

#define MAX_TIMEOUT_PROMPTGETUSERINPUTTIMEOUT 25500

char GetUserInputTimeout(unsigned int timeout);

char* fgets3(FILE* file, char* line, int nbChar);

char* strtime_m(void);
char* strtime_fns(void);
void DecSec2DaysHoursMinSec(double decsec, int* pDays, int* pHours, int* pMin, int* pSec, double* pDeccsec);

double sqr(double x);
double sign(double x, double epsilon);
double fmod_2PI(double theta);

union usShort
{
	short v;  
	unsigned char c[2];
};
typedef union usShort usShort;

union uShort
{
	unsigned short v;  
	unsigned char c[2];
};
typedef union uShort uShort;

union usLong
{
	long v;  
	unsigned char c[4];
};
typedef union usLong usLong;

union uLong
{
	unsigned long v;  
	unsigned char c[4];
};
typedef union uLong uLong;

union uFloat
{
	float v;  
	unsigned char c[4];
};
typedef union uFloat uFloat;

#ifndef _WIN32
typedef union _LARGE_INTEGER {
	struct {
		unsigned long LowPart;
		long HighPart;
	};
	struct {
		unsigned long LowPart;
		long HighPart;
	} u;
	long long QuadPart;
} LARGE_INTEGER;

typedef union _ULARGE_INTEGER {
	struct {
		unsigned long LowPart;
		unsigned long HighPart;
	};
	struct {
		unsigned long LowPart;
		unsigned long HighPart;
	} u;
	unsigned long long QuadPart;
} ULARGE_INTEGER;
#endif // _WIN32

void CalcCRC16(unsigned char* SC_Buffer, int SC_Amount, unsigned char* crc_h, unsigned char* crc_l);

/*
Convert GPS data to coordinates in a reference coordinate system.

double lat0 : (IN) GPS latitude of the reference coordinate system origin (in decimal deg).
double long0 : (IN) GPS longitude of the reference coordinate system origin (in decimal deg).
double alt0 : (IN) GPS altitude of the reference coordinate system origin (in m).
double latitude : (IN) GPS latitude (in decimal deg).
double longitude : (IN) GPS longitude (in decimal deg).
double altitude : (IN) GPS altitude (in m).
double* pX : (INOUT) Valid pointer that will receive the x coordinate in the reference coordinate 
system (in m).
double* pY : (INOUT) Valid pointer that will receive the y coordinate in the reference coordinate 
system (in m).
double* pZ : (INOUT) Valid pointer that will receive the z coordinate in the reference coordinate 
system (in m).
int cstype : (IN) Either EAST_NORTH_UP_COORDINATE_SYSTEM, NORTH_EAST_DOWN_COORDINATE_SYSTEM 
or NORTH_WEST_UP_COORDINATE_SYSTEM.

Return : Nothing.
*/
inline void GPS2RefCoordSystem(double lat0, double long0, double alt0,
							   double latitude, double longitude, double altitude,
							   double* pX, double* pY, double* pZ,
							   int cstype)
{
	switch (cstype)
	{
	case NORTH_EAST_DOWN_COORDINATE_SYSTEM:
		*pX = (M_PI/180.0)*EARTH_RADIUS*(latitude-lat0);
		*pY = (M_PI/180.0)*EARTH_RADIUS*(longitude-long0)*cos((M_PI/180.0)*latitude);
		*pZ = alt0-altitude;
		break;
	case NORTH_WEST_UP_COORDINATE_SYSTEM:
		*pX = (M_PI/180.0)*EARTH_RADIUS*(latitude-lat0);
		*pY = (M_PI/180.0)*EARTH_RADIUS*(long0-longitude)*cos((M_PI/180.0)*latitude);
		*pZ = altitude-alt0;
		break;
	default: // EAST_NORTH_UP_COORDINATE_SYSTEM
		*pX = (M_PI/180.0)*EARTH_RADIUS*(longitude-long0)*cos((M_PI/180.0)*latitude);
		*pY = (M_PI/180.0)*EARTH_RADIUS*(latitude-lat0);
		*pZ = altitude-alt0;
		break;
	}
}

/*
Convert coordinates in a reference coordinate system to GPS data.

double lat0 : (IN) GPS latitude of the reference coordinate system origin (in decimal deg).
double long0 : (IN) GPS longitude of the reference coordinate system origin (in decimal deg).
double alt0 : (IN) GPS altitude of the reference coordinate system origin (in m).
double x : (IN) x coordinate in the reference coordinate system (in m).
double y : (IN) y coordinate in the reference coordinate system (in m).
double z : (IN) z coordinate in the reference coordinate system (in m).
double* pLatitude : (INOUT) Valid pointer that will receive the GPS latitude (in decimal deg).
double* pLongitude : (INOUT) Valid pointer that will receive the GPS longitude (in decimal deg).
double* pAltitude : (INOUT) Valid pointer that will receive the GPS altitude (in m).
int cstype : (IN) Either EAST_NORTH_UP_COORDINATE_SYSTEM, NORTH_EAST_DOWN_COORDINATE_SYSTEM 
or NORTH_WEST_UP_COORDINATE_SYSTEM.

Return : Nothing.
*/
inline void RefCoordSystem2GPS(double lat0, double long0, double alt0, 
							   double x, double y, double z,
							   double* pLatitude, double* pLongitude, double* pAltitude,
							   int cstype)
{
	switch (cstype)
	{
	case NORTH_EAST_DOWN_COORDINATE_SYSTEM:
		*pLatitude = (x/(double)EARTH_RADIUS)*(180.0/M_PI)+lat0;
		if ((fabs(*pLatitude-90.0) < DBL_EPSILON)||(fabs(*pLatitude+90.0) < DBL_EPSILON))
		{
			*pLongitude = 0;
		}
		else
		{
			*pLongitude = (y/(double)EARTH_RADIUS)*(180.0/M_PI)/cos((M_PI/180.0)*(*pLatitude))+long0;
		}
		*pAltitude = alt0-z;
		break;
	case NORTH_WEST_UP_COORDINATE_SYSTEM:
		*pLatitude = (x/(double)EARTH_RADIUS)*(180.0/M_PI)+lat0;
		if ((fabs(*pLatitude-90.0) < DBL_EPSILON)||(fabs(*pLatitude+90.0) < DBL_EPSILON))
		{
			*pLongitude = 0;
		}
		else
		{
			*pLongitude = long0-(y/(double)EARTH_RADIUS)*(180.0/M_PI)/cos((M_PI/180.0)*(*pLatitude));
		}
		*pAltitude = z+alt0;
		break;
	default: // EAST_NORTH_UP_COORDINATE_SYSTEM
		*pLatitude = (y/(double)EARTH_RADIUS)*(180.0/M_PI)+lat0;
		if ((fabs(*pLatitude-90.0) < DBL_EPSILON)||(fabs(*pLatitude+90.0) < DBL_EPSILON))
		{
			*pLongitude = 0;
		}
		else
		{
			*pLongitude = (x/(double)EARTH_RADIUS)*(180.0/M_PI)/cos((M_PI/180.0)*(*pLatitude))+long0;
		}
		*pAltitude = z+alt0;
		break;
	}
}

// angle_env : Angle of the x axis of the environment coordinate system 
// w.r.t an East - North - Up coordinate system, should be set to 0 if no 
// specific environment (i.e. an East - North - Up coordinate system) 
// is used as reference (in rad).
inline void GPS2EnvCoordSystem(double lat_env, double long_env, double alt_env, double angle_env,
							   double latitude, double longitude, double altitude,
							   double* pX, double* pY, double* pZ)
{
	double x_enu = 0, y_enu = 0, z_enu = 0;

	GPS2RefCoordSystem(lat_env, long_env, alt_env, latitude, longitude, altitude, &x_enu, &y_enu, &z_enu, EAST_NORTH_UP_COORDINATE_SYSTEM);

	// Conversion from East - North - Up to environment coordinate system.
	*pX = x_enu*cos(-angle_env)-y_enu*sin(-angle_env);
	*pY = x_enu*sin(-angle_env)+y_enu*cos(-angle_env);
	*pZ = z_enu;
}

inline void EnvCoordSystem2GPS(double lat_env, double long_env, double alt_env, double angle_env, 
							   double x, double y, double z,
							   double* pLatitude, double* pLongitude, double* pAltitude)
{
	double x_enu = 0, y_enu = 0, z_enu = 0;

	// Conversion from environment to East - North - Up coordinate system.
	x_enu = x*cos(angle_env)-y*sin(angle_env);
	y_enu = x*sin(angle_env)+y*cos(angle_env);
	z_enu = z;

	RefCoordSystem2GPS(lat_env, long_env, alt_env, x_enu, y_enu, z_enu, pLatitude, pLongitude, pAltitude, EAST_NORTH_UP_COORDINATE_SYSTEM);
}

// The angles conversions are most of the time used separately...
inline void Robot2EnvCoordSystem(double x_robot, double y_robot, double z_robot, double theta_robot,
								 double x, double y, double z, //double theta,
								 double* pX, double* pY, double* pZ//, double* pTheta
								 )
{
	*pX = x*cos(theta_robot)-y*sin(theta_robot)+x_robot;
	*pY = x*sin(theta_robot)+y*cos(theta_robot)+y_robot;
	*pZ = z+z_robot;
	//*pTheta = theta+theta_robot;
}

inline void EnvCoordSystem2Robot(double x_robot, double y_robot, double z_robot, double theta_robot, 
								 double x, double y, double z, //double theta,
								 double* pX, double* pY, double* pZ//, double* pTheta
								 )
{
	*pX = (x-x_robot)*cos(-theta_robot)-(y-y_robot)*sin(-theta_robot);
	*pY = (x-x_robot)*sin(-theta_robot)+(y-y_robot)*cos(-theta_robot);
	*pZ = z-z_robot;
	//*pTheta = theta-theta_robot;
}

inline void DecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin)
{
	*pDeg = (int)decdeg;
	*pDecMin = fabs((decdeg-*pDeg)*60.0);
}

inline void DecDeg2DegMinDecSec(double decdeg, int* pDeg, int* pMin, double* pDecSec)
{
	double decmin;
	DecDeg2DegDecMin(decdeg, pDeg, &decmin);
	*pMin = (int)decmin;
	*pDecSec = (decmin-*pMin)*60.0;
}

inline void DegDecMin2DecDeg(int deg, double decmin, double* pDecDeg)
{
	*pDecDeg = (deg >= 0)?deg+fabs(decmin/60.0):deg-fabs(decmin/60.0);
}

inline void DegMinDecSec2DecDeg(int deg, int min, double decsec, double* pDecDeg)
{
	double decmin = abs(min)+fabs(decsec)/60.0;
	DegDecMin2DecDeg(deg, decmin, pDecDeg);
}

inline void GPSLatitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pNorthOrSouth)
{
	DecDeg2DegDecMin(decdeg, pDeg, pDecMin);
	*pDeg = abs(*pDeg);
	*pNorthOrSouth = (decdeg >= 0)?'N':'S';
}

inline void GPSLongitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pEastOrWest)
{
	DecDeg2DegDecMin(decdeg, pDeg, pDecMin);
	*pDeg = abs(*pDeg);
	*pEastOrWest = (decdeg >= 0)?'E':'W';
}

inline void GPSLatitudeDegDecMin2DecDeg(int deg, double decmin, char NorthOrSouth, double* pDecDeg)
{
	DegDecMin2DecDeg(abs(deg), fabs(decmin), pDecDeg);
	*pDecDeg = (NorthOrSouth == 'N')?*pDecDeg:-*pDecDeg;
}

inline void GPSLongitudeDegDecMin2DecDeg(int deg, double decmin, char EastOrWest, double* pDecDeg)
{
	DegDecMin2DecDeg(abs(deg), fabs(decmin), pDecDeg);
	*pDecDeg = (EastOrWest == 'E')?*pDecDeg:-*pDecDeg;
}

inline void GPSLatitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pNorthOrSouth)
{
	DecDeg2DegMinDecSec(val, pDeg, pMin, pDecSec);
	*pDeg = abs(*pDeg);
	*pNorthOrSouth = (val >= 0)?'N':'S';
}

inline void GPSLongitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pEastOrWest)
{
	DecDeg2DegMinDecSec(val, pDeg, pMin, pDecSec);
	*pDeg = abs(*pDeg);
	*pEastOrWest = (val >= 0)?'E':'W';
}

inline void GPSLatitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char NorthOrSouth, double* pDecDeg)
{
	DegMinDecSec2DecDeg(abs(deg), abs(min), fabs(decsec), pDecDeg);
	*pDecDeg = (NorthOrSouth == 'N')?*pDecDeg:-*pDecDeg;
}

inline void GPSLongitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char EastOrWest, double* pDecDeg)
{
	DegMinDecSec2DecDeg(abs(deg), abs(min), fabs(decsec), pDecDeg);
	*pDecDeg = (EastOrWest == 'E')?*pDecDeg:-*pDecDeg;
}

/*
Structure that defines a coordinate system.
*/
struct _COORDSYSTEM
{
	double xMin;
	double xMax;
	double yMin;
	double yMax;
};
typedef struct _COORDSYSTEM COORDSYSTEM;

#define KEEP_X_RATIO_COORDSYSTEM2IMG 0x00000001
#define KEEP_Y_RATIO_COORDSYSTEM2IMG 0x00000002
#define BEST_RATIO_COORDSYSTEM2IMG 0x00000004

/*
Structure that enables the use of a coordinate system in an image instead of 
using its lines and rows.
*/
struct _COORDSYSTEM2IMG
{
	COORDSYSTEM cs;
	UINT width;
	UINT height;
	double XJRatio;
	double YIRatio;
	double JXRatio;
	double IYRatio;
};
typedef struct _COORDSYSTEM2IMG COORDSYSTEM2IMG;

/*
Initialize a structure COORDSYSTEM2IMG.
See the description of the COORDSYSTEM2IMG structure for more information.

COORDSYSTEM2IMG* pCS2Img : (INOUT) Valid pointer to the structure.
COORDSYSTEM* pCS : (IN) Coordinate system to use.
UINT width : (IN) Width of the picture.
UINT height : (IN) Height of the picture.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
inline int InitCS2Img(COORDSYSTEM2IMG* pCS2Img, COORDSYSTEM* pCS, UINT width, UINT height)
{
	// x = xMin -> j = 0
	// x = xMax -> j = width-1
	// y = yMin -> i = height-1
	// y = yMax -> i = 0

	pCS2Img->cs = *pCS;
	pCS2Img->width = width;
	pCS2Img->height = height;

	pCS2Img->XJRatio = (pCS2Img->cs.xMax - pCS2Img->cs.xMin) / (double)(pCS2Img->width - 1);
	pCS2Img->YIRatio = (pCS2Img->cs.yMax - pCS2Img->cs.yMin) / (double)(pCS2Img->height - 1);

	pCS2Img->JXRatio = 1.0/pCS2Img->XJRatio;
	pCS2Img->IYRatio = 1.0/pCS2Img->YIRatio;

	return EXIT_SUCCESS;
}

/*
Initialize a structure COORDSYSTEM2IMG.
See the description of the COORDSYSTEM2IMG structure for more information.

COORDSYSTEM2IMG* pCS2Img : (INOUT) Valid pointer to the structure.
COORDSYSTEM* pCS : (IN) Coordinate system to use. //Can be modified to keep ratios.//
UINT width : (IN) Width of the picture.
UINT height : (IN) Height of the picture.
int flags : (IN) KEEP_X_RATIO_COORDSYSTEM2IMG to set the y ratio from x ratio, 
KEEP_Y_RATIO_COORDSYSTEM2IMG to set the x ratio from y ratio, BEST_RATIO_COORDSYSTEM2IMG 
to keep the best to avoid loosing any part of the image, 0 for free ratios.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
inline int InitCS2ImgEx(COORDSYSTEM2IMG* pCS2Img, COORDSYSTEM* pCS, UINT width, UINT height, int flags)
{
	// x = xMin -> j = 0
	// x = xMax -> j = width-1
	// y = yMin -> i = height-1
	// y = yMax -> i = 0

	double val = 0, temp = 0;

	pCS2Img->cs = *pCS;
	pCS2Img->width = width;
	pCS2Img->height = height;

	switch (flags)
	{
	case KEEP_X_RATIO_COORDSYSTEM2IMG:
		{
			pCS2Img->XJRatio = (pCS2Img->cs.xMax - pCS2Img->cs.xMin) / (double)(pCS2Img->width - 1);
			pCS2Img->YIRatio = pCS2Img->XJRatio;
			val = pCS2Img->YIRatio * (pCS2Img->height - 1);
			temp = (pCS2Img->cs.yMin + pCS2Img->cs.yMax);
			pCS2Img->cs.yMin = (temp-val)/2.0;
			pCS2Img->cs.yMax = (temp+val)/2.0;
			break;
		}
	case KEEP_Y_RATIO_COORDSYSTEM2IMG:
		{
			pCS2Img->YIRatio = (pCS2Img->cs.yMax - pCS2Img->cs.yMin) / (double)(pCS2Img->height - 1);
			pCS2Img->XJRatio = pCS2Img->YIRatio;
			val = pCS2Img->XJRatio * (pCS2Img->width - 1);
			temp = (pCS2Img->cs.xMin + pCS2Img->cs.xMax);
			pCS2Img->cs.xMin = (temp-val)/2.0;
			pCS2Img->cs.xMax = (temp+val)/2.0;
			break;
		}
	case BEST_RATIO_COORDSYSTEM2IMG:
		{
			pCS2Img->XJRatio = (pCS2Img->cs.xMax - pCS2Img->cs.xMin) / (double)(pCS2Img->width - 1);
			pCS2Img->YIRatio = (pCS2Img->cs.yMax - pCS2Img->cs.yMin) / (double)(pCS2Img->height - 1);

			if (pCS2Img->XJRatio < pCS2Img->YIRatio)
			{
				pCS2Img->XJRatio = pCS2Img->YIRatio;
				val = pCS2Img->XJRatio * (pCS2Img->width - 1);
				temp = (pCS2Img->cs.xMin + pCS2Img->cs.xMax);
				pCS2Img->cs.xMin = (temp-val)/2.0;
				pCS2Img->cs.xMax = (temp+val)/2.0;
			}
			else if (pCS2Img->XJRatio > pCS2Img->YIRatio)
			{
				pCS2Img->YIRatio = pCS2Img->XJRatio;
				val = pCS2Img->YIRatio * (pCS2Img->height - 1);
				temp = (pCS2Img->cs.yMin + pCS2Img->cs.yMax);
				pCS2Img->cs.yMin = (temp-val)/2.0;
				pCS2Img->cs.yMax = (temp+val)/2.0;
			}
			break;
		}
	default:
		{
			pCS2Img->XJRatio = (pCS2Img->cs.xMax - pCS2Img->cs.xMin) / (double)(pCS2Img->width - 1);
			pCS2Img->YIRatio = (pCS2Img->cs.yMax - pCS2Img->cs.yMin) / (double)(pCS2Img->height - 1);
			break;
		}
	}

	pCS2Img->JXRatio = 1.0/pCS2Img->XJRatio;
	pCS2Img->IYRatio = 1.0/pCS2Img->YIRatio;
	//*pCS = pCS2Img->cs;

	return EXIT_SUCCESS;
}

inline int XYCS2IJImg(COORDSYSTEM2IMG* pCS2Img, double x, double y, int* pI, int* pJ)
{
	*pI = (int)((pCS2Img->cs.yMax - y) * pCS2Img->IYRatio);
	*pJ = (int)((x - pCS2Img->cs.xMin) * pCS2Img->JXRatio);

	return EXIT_SUCCESS;
}

inline int IJImg2XYCS(COORDSYSTEM2IMG* pCS2Img, int i, int j, double* pX, double* pY)
{
	*pX = pCS2Img->XJRatio * j + pCS2Img->cs.xMin;
	*pY = pCS2Img->cs.yMax - pCS2Img->YIRatio * i;

	return EXIT_SUCCESS;
}

inline int XCS2JImg(COORDSYSTEM2IMG* pCS2Img, double x)
{
	return (int)((x - pCS2Img->cs.xMin) * pCS2Img->JXRatio);
}

inline int YCS2IImg(COORDSYSTEM2IMG* pCS2Img, double y)
{
	return (int)((pCS2Img->cs.yMax - y) * pCS2Img->IYRatio);
}

inline double IImg2YCS(COORDSYSTEM2IMG* pCS2Img, int i)
{
	return pCS2Img->cs.yMax - pCS2Img->YIRatio * i;
}

inline double JImg2XCS(COORDSYSTEM2IMG* pCS2Img, int j)
{
	return pCS2Img->XJRatio * j + pCS2Img->cs.xMin;
}

inline int GetCSPixelSize(COORDSYSTEM2IMG* pCS2Img, double* pSizeX, double* pSizeY)
{
	*pSizeX = pCS2Img->XJRatio;
	*pSizeY = pCS2Img->YIRatio;

	return EXIT_SUCCESS;
}

inline double GetCSPixelSizeX(COORDSYSTEM2IMG* pCS2Img)
{
	return pCS2Img->XJRatio;
}

inline double GetCSPixelSizeY(COORDSYSTEM2IMG* pCS2Img)
{
	return pCS2Img->YIRatio;
}

#endif // UTILS_H
