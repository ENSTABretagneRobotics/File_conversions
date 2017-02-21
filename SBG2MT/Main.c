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
	char szTemp[256];
	char szName[256];
	char line[4096];
	unsigned int i = 0;

	// Temporary buffers for sscanf().
	int UTC_Year = 0, UTC_Month = 0, UTC_Day = 0, UTC_Hour = 0, UTC_Minute = 0; 
	double UTC_Seconds = 0;
	double PacketCounter = 0, SampleTimeFine = 0, 
		UTC_Nano = 0, UTC_Valid = 0, 
		StatusWord = 0,
		Acc_X = 0, Acc_Y = 0, Acc_Z = 0, 
		Gyr_X = 0, Gyr_Y = 0, Gyr_Z = 0, 
		Mag_X = 0, Mag_Y = 0, Mag_Z = 0, 
		Roll = 0, Pitch = 0, Yaw = 0, 
		Latitude = 0, Longitude = 0, Altitude = 0, 
		Vel_X = 0, Vel_Y = 0, Vel_Z = 0;
	struct timeval tv;

	if (argc != 2)
	{
		strcpy(szFileInPath, "SBG.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : SBG2MT file.csv.\n");
		printf("Default : SBG2MT %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s_MT.csv", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	//LoadConfig();

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open SBG file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create MT file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(fileout, 
		"PacketCounter;SampleTimeFine;"
		"UTC_Nano;UTC_Year;UTC_Month;UTC_Day;UTC_Hour;UTC_Minute;UTC_Second;UTC_Valid;"
		"StatusWord;"
		"Acc_X;Acc_Y;Acc_Z;"
		"Gyr_X;Gyr_Y;Gyr_Z;"
		"Mag_X;Mag_Y;Mag_Z;"
		"Roll;Pitch;Yaw;"
		"Latitude;Longitude;Altitude;"
		"Vel_X;Vel_Y;Vel_Z;"
		"tv_sec;tv_usec;\n"
		); 

	printf("Converting...\n");

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line), filein) != NULL) 
	{
		if (sscanf(line, 
			"%lf;%lf;"
			"%lf;%d;%d;%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%d;%d",
			&PacketCounter, &SampleTimeFine, 
			&UTC_Nano, &UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&UTC_Valid, &StatusWord, &Acc_X, &Acc_Y, &Acc_Z, &Gyr_X, &Gyr_Y, &Gyr_Z, &Mag_X, &Mag_Y, &Mag_Z, 
			&Roll, &Pitch, &Yaw, 
			&Latitude, &Longitude, &Altitude,
			&Vel_X, &Vel_Y, &Vel_Z,
			&tv.tv_sec, &tv.tv_usec) == 31)
		{
			fprintf(fileout, "%f;%f;"
				"%f;%d;%d;%d;%d;%d;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%d;%d;\n", 
				PacketCounter, SampleTimeFine, 
				UTC_Nano, UTC_Year, UTC_Month, UTC_Day, UTC_Hour, UTC_Minute, UTC_Seconds, 
				UTC_Valid, StatusWord, Acc_X, -Acc_Y, -Acc_Z, Gyr_X, -Gyr_Y, -Gyr_Z, Mag_X, -Mag_Y, -Mag_Z, 
				fmod_360(-Roll), fmod_360(Pitch), fmod_360(-Yaw), 
				Latitude, Longitude, Altitude,
				Vel_X, -Vel_Y, -Vel_Z,
				tv.tv_sec, tv.tv_usec);

			i++;
		}
		else if ((sscanf(line, 
			"%lf;%lf;"
			"%lf;%d;%d;%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf",
			&PacketCounter, &SampleTimeFine, 
			&UTC_Nano, &UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&UTC_Valid, &StatusWord, &Acc_X, &Acc_Y, &Acc_Z, &Gyr_X, &Gyr_Y, &Gyr_Z, &Mag_X, &Mag_Y, &Mag_Z, 
			&Roll, &Pitch, &Yaw, 
			&Latitude, &Longitude, &Altitude,
			&Vel_X, &Vel_Y, &Vel_Z) == 29)||
			(sscanf(line, 
			"%lf;%lf;"
			"%lf;%d-%d-%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf",
			&PacketCounter, &SampleTimeFine, 
			&UTC_Nano, &UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&UTC_Valid, &StatusWord, &Acc_X, &Acc_Y, &Acc_Z, &Gyr_X, &Gyr_Y, &Gyr_Z, &Mag_X, &Mag_Y, &Mag_Z, 
			&Roll, &Pitch, &Yaw, 
			&Latitude, &Longitude, &Altitude,
			&Vel_X, &Vel_Y, &Vel_Z) == 29))
		{

			// Should check which data give the best info on date/time...

			fprintf(fileout, "%f;%f;"
				"%f;%d;%d;%d;%d;%d;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;\n", 
				PacketCounter, SampleTimeFine, 
				UTC_Nano, UTC_Year, UTC_Month, UTC_Day, UTC_Hour, UTC_Minute, UTC_Seconds, 
				UTC_Valid, StatusWord, Acc_X, -Acc_Y, -Acc_Z, Gyr_X, -Gyr_Y, -Gyr_Z, Mag_X, -Mag_Y, -Mag_Z, 
				fmod_360(-Roll), fmod_360(Pitch), fmod_360(-Yaw), 
				Latitude, Longitude, Altitude,
				Vel_X, -Vel_Y, -Vel_Z);

			i++;
		}
		else
		{
			//printf("Skipping an invalid line in the SBG file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing MT file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing SBG file.\n");
		fclose(fileout);
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
