// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "MAVLinkProtocol.h"

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szName[256];
	char line[4096];
	unsigned int i = 0, j = 0;

	uint64_t TimeUS = 0;
	unsigned int I = 0;
	unsigned int Status = 0, GMS = 0, GWk = 0, NSats = 0, U = 0;
	double HDop = 0, Lat = 0, Lng = 0, Alt_AMSL_GPS = 0, Spd = 0, GCrs = 0, VZ = 0, GPSYaw = 0;
	//unsigned int State = 0, Back = 0;
	double DPosX = 0, DPosY = 0, PosX = 0, PosY = 0, DVelX = 0, DVelY = 0, VelX = 0, VelY = 0, DAccX = 0, DAccY = 0;
	unsigned int AEKF = 0;
	double DesRoll = 0, Roll = 0, DesPitch = 0, Pitch = 0, DesYaw = 0, Yaw = 0, ErrRP = 0, ErrYaw = 0;
	int MagX = 0, MagY = 0, MagZ = 0;
	double GyrX = 0, GyrY = 0, GyrZ = 0, AccX = 0, AccY = 0, AccZ = 0;

	int UTC_Year = 0, UTC_Month = 0, UTC_Day = 0, UTC_Hour = 0, UTC_Minute = 0; 
	double UTC_Seconds = 0;
	double TimeStamp = 0, GPS_Time = 0, UTC_Time = 0;

/* 
Format characters in the format string for binary log messages 
  b   : int8_t 
  B   : uint8_t 
  h   : int16_t 
  H   : uint16_t 
  i   : int32_t 
  I   : uint32_t 
  f   : float 
  n   : char[4] 
  N   : char[16] 
  Z   : char[64] 
  c   : int16_t * 100 
  C   : uint16_t * 100 
  e   : int32_t * 100 
  E   : uint32_t * 100 
  L   : int32_t latitude/longitude // -> Wrong...?
  M   : uint8_t flight mode 
*/ 

	if (argc != 2)
	{
		strcpy(szFileInPath, "log.log");
		printf("Warning : No parameter specified.\n");
		printf("Usage : dataflash2SBG file.log.\n");
		printf("Default : dataflash2SBG %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, ".csv", szFileOutPath, szName);

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

	fileout = fopen(szFileOutPath, "wb");
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
		"Time Stamp;GPS Time;UTC Time;UTC Date;UT;UT;UT;Num Sv Used;Correction Age;"
		"Accelerometer X;Accelerometer Y;Accelerometer Z;Gyroscope X;Gyroscope Y;Gyroscope Z;Magnetometer X;Magnetometer Y;Magnetometer Z;"
		"Roll;Pitch;Yaw;Latitude;Longitude;Altitude MSL;X Velocity;Y Velocity;Z Velocity;GNSS1Yaw;;;;;\n"
		); 

	printf("Converting...\n");

	i = 0;
	j = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		//GPS, 182929586, 0, 6, 231688000, 2167, 23, 0.77, 48.4181976, -4.4719822, 94.57, 0.014, 116.528, 0.016, 181.4797, 1
		//GPS, 380997198, 3, 141473400, 1961, 10, 0.89, 48.4762186, -4.5440161, 119.53, 1.108, 198.187, -0.49, 1
		if ((sscanf(line, "GPS, %I64u, %u, %u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", 
			&TimeUS, &I, &Status, &GMS, &GWk, &NSats, &HDop, &Lat, &Lng, &Alt_AMSL_GPS, &Spd, &GCrs, &VZ, &GPSYaw, &U) == 15)||
			(sscanf(line, "GPS, %I64u, %u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", 
				&TimeUS, &Status, &GMS, &GWk, &NSats, &HDop, &Lat, &Lng, &Alt_AMSL_GPS, &Spd, &GCrs, &VZ, &U) == 13))
		{
			time_t tt = 0;
			struct tm* timeptr = NULL;

			j++;
			TimeStamp = (uint32_t)(TimeUS/1000);
			GPS_Time = 0.000001*TimeUS;
			UTC_Time = GPSTOW2UNIX(GMS, GWk)/1000.0;//-1626566400000;

			tt = (time_t)(GPSTOW2UNIX(GMS, GWk)/1000000.0);
			timeptr = gmtime(&tt);
			if (timeptr != NULL)
			{
				UTC_Hour = timeptr->tm_hour;
				UTC_Minute = timeptr->tm_min;
				UTC_Seconds = timeptr->tm_sec+0.000001*(GPSTOW2UNIX(GMS, GWk)%1000000);
				UTC_Day = timeptr->tm_mday;
				UTC_Month = timeptr->tm_mon+1;
				UTC_Year = timeptr->tm_year+1900;
			}

			fprintf(fileout, "%f;%f;"
				"%f;%02d/%02d/%d;%d;%d;%f;%d;0;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%.8f;%.8f;%f;"
				"%f;%f;%f;"
				"%f;;;;;\n", 
				TimeStamp, GPS_Time, 
				UTC_Time, UTC_Day, UTC_Month, UTC_Year, UTC_Hour, UTC_Minute, UTC_Seconds, NSats,
				AccX, AccY, AccZ, GyrX*180.0/M_PI, GyrY*180.0/M_PI, GyrZ*180.0/M_PI, MagX/1000.0, MagY/1000.0, MagZ/1000.0, 
				fmod_360(Roll), fmod_360(Pitch), fmod_360_pos(Yaw), 
				Lat, Lng, Alt_AMSL_GPS,
				VelX/100.0, VelY/100.0, -VZ,
				fmod_360(GPSYaw));
			i++;
		}
		//NTUN, 549071374, -206.6111, -76.6899, -235.0755, -81.34146, 27.78842, 4.615891, -64.85806, -10.16781, 322.2938, 6.05488
		else if (sscanf(line, "NTUN, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", 
			&TimeUS, &DPosX, &DPosY, &PosX, &PosY, &DVelX, &DVelY, &VelX, &VelY, &DAccX, &DAccY) == 11)
		{
			j++;
			TimeStamp = (uint32_t)(TimeUS/1000);
			GPS_Time = 0.000001*TimeUS;
			fprintf(fileout, "%f;%f;"
				"%f;%02d/%02d/%d;%d;%d;%f;%d;0;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%.8f;%.8f;%f;"
				"%f;%f;%f;"
				"%f;;;;;\n", 
				TimeStamp, GPS_Time, 
				UTC_Time, UTC_Day, UTC_Month, UTC_Year, UTC_Hour, UTC_Minute, UTC_Seconds, NSats,
				AccX, AccY, AccZ, GyrX*180.0/M_PI, GyrY*180.0/M_PI, GyrZ*180.0/M_PI, MagX/1000.0, MagY/1000.0, MagZ/1000.0, 
				fmod_360(Roll), fmod_360(Pitch), fmod_360_pos(Yaw),
				Lat, Lng, Alt_AMSL_GPS,
				VelX/100.0, VelY/100.0, -VZ,
				fmod_360(GPSYaw));
			i++;
		}
		//ATT, 194728916, 0, -1.05, 0, 2.28, 0, 193.01, 0.03, 1, 3
		//ATT, 380979385, 3.39, 4.97, -3.56, 0.41, 263.21, 268.16, 0.08, 0.05
		else if ((sscanf(line, "ATT, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", 
			&TimeUS, &DesRoll, &Roll, &DesPitch, &Pitch, &DesYaw, &Yaw, &ErrRP, &ErrYaw, &AEKF) == 10)||
			(sscanf(line, "ATT, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", 
			&TimeUS, &DesRoll, &Roll, &DesPitch, &Pitch, &DesYaw, &Yaw, &ErrRP, &ErrYaw) == 9))
		{
			j++;
			TimeStamp = (uint32_t)(TimeUS/1000);
			GPS_Time = 0.000001*TimeUS;
			fprintf(fileout, "%f;%f;"
				"%f;%02d/%02d/%d;%d;%d;%f;%d;0;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%.8f;%.8f;%f;"
				"%f;%f;%f;"
				"%f;;;;;\n", 
				TimeStamp, GPS_Time, 
				UTC_Time, UTC_Day, UTC_Month, UTC_Year, UTC_Hour, UTC_Minute, UTC_Seconds, NSats,
				AccX, AccY, AccZ, GyrX*180.0/M_PI, GyrY*180.0/M_PI, GyrZ*180.0/M_PI, MagX/1000.0, MagY/1000.0, MagZ/1000.0, 
				fmod_360(Roll), fmod_360(Pitch), fmod_360_pos(Yaw),
				Lat, Lng, Alt_AMSL_GPS,
				VelX/100.0, VelY/100.0, -VZ,
				fmod_360(GPSYaw));
			i++;
		}
		//MAG, 194709930, 0, -209, -75, 411, 0, 0, 0, 0, 0, 0, 1, 194709926
		//MAG, 411625728, -53, 150, 369, 1, 17, -77, 0, 0, 0, 1, 411625723
		else if ((sscanf(line, "MAG, %I64u, %u, %d, %d, %d", &TimeUS, &I, &MagX, &MagY, &MagZ) == 5)||
			(sscanf(line, "MAG, %I64u, %d, %d, %d", &TimeUS, &MagX, &MagY, &MagZ) == 4))
		{
			j++;
			TimeStamp = (uint32_t)(TimeUS/1000);
			GPS_Time = 0.000001*TimeUS;
			fprintf(fileout, "%f;%f;"
				"%f;%02d/%02d/%d;%d;%d;%f;%d;0;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%.8f;%.8f;%f;"
				"%f;%f;%f;"
				"%f;;;;;\n", 
				TimeStamp, GPS_Time, 
				UTC_Time, UTC_Day, UTC_Month, UTC_Year, UTC_Hour, UTC_Minute, UTC_Seconds, NSats,
				AccX, AccY, AccZ, GyrX*180.0/M_PI, GyrY*180.0/M_PI, GyrZ*180.0/M_PI, MagX/1000.0, MagY/1000.0, MagZ/1000.0, 
				fmod_360(Roll), fmod_360(Pitch), fmod_360_pos(Yaw),
				Lat, Lng, Alt_AMSL_GPS,
				VelX/100.0, VelY/100.0, -VZ,
				fmod_360(GPSYaw));
			i++;
		}
		//IMU, 182929178, 0, 0.002522397, -0.001043871, -0.0002923453, 0.3591292, 0.1684806, -10.07592, 0, 0, 43.86654, 1, 1, 8019, 4009
		//IMU, 411575125, 0.2243799, 0.061694, -0.3788114, -0.7872514, 0.6289721, -10.71861, 0, 0, 24.81149, 1, 1, 999, 999
		else if ((sscanf(line, "IMU, %I64u, %u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &I, &GyrX, &GyrY, &GyrZ, &AccX, &AccY, &AccZ) == 8)||
			(sscanf(line, "IMU, %I64u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &GyrX, &GyrY, &GyrZ, &AccX, &AccY, &AccZ) == 7))
		{
			j++;
			TimeStamp = (uint32_t)(TimeUS/1000);
			GPS_Time = 0.000001*TimeUS;
			fprintf(fileout, "%f;%f;"
				"%f;%02d/%02d/%d;%d;%d;%f;%d;0;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%.8f;%.8f;%f;"
				"%f;%f;%f;"
				"%f;;;;;\n", 
				TimeStamp, GPS_Time, 
				UTC_Time, UTC_Day, UTC_Month, UTC_Year, UTC_Hour, UTC_Minute, UTC_Seconds, NSats,
				AccX, AccY, AccZ, GyrX*180.0/M_PI, GyrY*180.0/M_PI, GyrZ*180.0/M_PI, MagX/1000.0, MagY/1000.0, MagZ/1000.0, 
				fmod_360(Roll), fmod_360(Pitch), fmod_360_pos(Yaw),
				Lat, Lng, Alt_AMSL_GPS,
				VelX/100.0, VelY/100.0, -VZ,
				fmod_360(GPSYaw));
			i++;
		}
		else
		{
			//printf("Skipping an invalid line in the input file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Converted %u DataFlash messages to %u lines.\n", j, i);

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
