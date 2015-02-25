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

#define EARTH_RADIUS 6371000.0

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

char* FindNMEASentence(char sentencebegin[7], char* str);
char* FindLatestNMEASentence(char sentencebegin[7], char* str);
void ComputeNMEAchecksum(char* sentence, char checksum[4]);

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

void GPS2RefCoordSystem(double lat0, double long0, double latitude, double longitude, double* pX, double* pY);
void RefCoordSystem2GPS(double lat0, double long0, double x, double y, double* pLatitude, double* pLongitude);

void DecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin);
void DecDeg2DegMinDecSec(double decdeg, int* pDeg, int* pMin, double* pDecSec);
void DegDecMin2DecDeg(int deg, double decmin, double* pDecDeg);
void DegMinDecSec2DecDeg(int deg, int min, double decsec, double* pDecDeg);
void GPSLatitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pNorthOrSouth);
void GPSLongitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pEastOrWest);
void GPSLatitudeDegDecMin2DecDeg(int deg, double decmin, char NorthOrSouth, double* pDecDeg);
void GPSLongitudeDegDecMin2DecDeg(int deg, double decmin, char EastOrWest, double* pDecDeg);
void GPSLatitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pNorthOrSouth);
void GPSLongitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pEastOrWest);
void GPSLatitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char NorthOrSouth, double* pDecDeg);
void GPSLongitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char EastOrWest, double* pDecDeg);

#endif // UTILS_H
