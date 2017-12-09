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
	unsigned int i = 0;
	mavlink_message_t msg;
	//uint8_t target_system = 1, target_component = 0;
	uint8_t system_id = 1, component_id = 1;
	uint64_t TimeUS = 0;
	double t_epoch = 0, latitude = 0, longitude = 0, altitude_AMSL = 0, heading = 0, COG = 0, SOG = 0, altitude_AGL = 0, pressure = 0, fluiddira = 0, fluidspeeda = 0, fluiddir = 0, fluidspeed = 0, range = 0, bearing = 0, elevation = 0, utc = 0;
	double t_app = 0, xhat = 0, yhat = 0, zhat = 0, phihat = 0, thetahat = 0, psihat = 0;
	double vrxhat = 0, vryhat = 0, vrzhat = 0, omegaxhat = 0, omegayhat = 0, omegazhat = 0;
	double xhat_err = 0, yhat_err = 0, zhat_err = 0, phihat_err = 0, thetahat_err = 0, psihat_err = 0;
	double vrxhat_err = 0, vryhat_err = 0, vrzhat_err = 0, omegaxhat_err = 0, omegayhat_err = 0, omegazhat_err = 0;
	double wx = 0, wy = 0, wz = 0, wphi = 0, wtheta = 0, wpsi = 0, wd = 0, wu = 0, wagl = 0;
	double uvx = 0, uvy = 0, uvz = 0, uwx = 0, uwy = 0, uwz = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0, u6 = 0, u7 = 0, u8 = 0, u9 = 0, u10 = 0, u11 = 0, u12 = 0, u13 = 0, u14 = 0;
	double Energy_electronics = 0, Energy_actuators = 0;
	double f0, f1, f2, f3, f5, f6, f7, f8, f9, f10, f11, f12; 
	double f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23;
	int tv_sec = 0, tv_usec = 0;

	if (argc != 2)
	{
		strcpy(szFileInPath, "logstate.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : logstate2TLOG file.csv.\n");
		printf("Default : logstate2TLOG %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	GetFileNameAndFilePathAndChangeExtension(szFileInPath, ".tlog", szFileOutPath, szName);

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

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf", 
			&t_epoch, &latitude, &longitude, &altitude_AMSL, &heading, &COG, &SOG, &altitude_AGL, &pressure, &fluiddira, &fluidspeeda, &fluiddir, &fluidspeed, &range, &bearing, &elevation, &utc,
			&t_app, &xhat, &yhat, &zhat, &phihat, &thetahat, &psihat,
			&vrxhat, &vryhat, &vrzhat, &omegaxhat, &omegayhat, &omegazhat,
			&xhat_err, &yhat_err, &zhat_err, &phihat_err, &thetahat_err, &psihat_err,
			&vrxhat_err, &vryhat_err, &vrzhat_err, &omegaxhat_err, &omegayhat_err, &omegazhat_err,
			&wx, &wy, &wz, &wphi, &wtheta, &wpsi, &wd, &wu, &wagl, 
			&uvx, &uvy, &uvz, &uwx, &uwy, &uwz, &u1, &u2, &u3, &u4, &u5, &u6, &u7, &u8, &u9, &u10, &u11, &u12, &u13, &u14, 
			&Energy_electronics, &Energy_actuators) == 73)
			&&(latitude != 0)&&(longitude != 0))
	{
			TimeUS = (uint64_t)(t_epoch*1000000);
			if ((i%50) == 0)
			{
				// Send heartbeat regularly...
				mavlink_heartbeat_t heartbeat;
				memset(&heartbeat, 0, sizeof(heartbeat));
				heartbeat.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;

				heartbeat.type = MAV_TYPE_GENERIC;
				heartbeat.type = MAV_TYPE_SUBMARINE;
				heartbeat.type = MAV_TYPE_SURFACE_BOAT;
				heartbeat.type = MAV_TYPE_SURFACE_BOAT;
				heartbeat.type = MAV_TYPE_GROUND_ROVER;
				heartbeat.type = MAV_TYPE_QUADROTOR;
				heartbeat.type = 2; // Temp...

				//heartbeat.base_mode = 209;
				heartbeat.base_mode = MAV_MODE_FLAG_MANUAL_INPUT_ENABLED|MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | MAV_MODE_FLAG_AUTO_ENABLED;
				heartbeat.base_mode |= MAV_MODE_FLAG_SAFETY_ARMED;
				heartbeat.system_status = MAV_STATE_ACTIVE;

				mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
				fwrite_tlog_unix(TimeUS, msg, fileout);
			}

			mavlink_gps_raw_int_t gps_raw_int;
			memset(&gps_raw_int, 0, sizeof(gps_raw_int));
			gps_raw_int.time_usec = TimeUS;
			gps_raw_int.fix_type = (uint8_t)3;
			gps_raw_int.lat = (int32_t)(latitude*10000000.0);
			gps_raw_int.lon = (int32_t)(longitude*10000000.0);
			gps_raw_int.alt = (int32_t)(altitude_AMSL*1000.0);
			gps_raw_int.satellites_visible = 255;
			gps_raw_int.eph = UINT16_MAX;
			gps_raw_int.epv = UINT16_MAX;
			gps_raw_int.cog = UINT16_MAX;
			gps_raw_int.vel = UINT16_MAX;
			mavlink_msg_gps_raw_int_encode(system_id, component_id, &msg, &gps_raw_int);
			fwrite_tlog_unix(TimeUS, msg, fileout);
			
			mavlink_attitude_t attitude;
			memset(&attitude, 0, sizeof(attitude));
			attitude.time_boot_ms = (uint32_t)(TimeUS/1000);
			attitude.roll = (float)fmod_2PI(phihat);
			attitude.pitch = (float)fmod_2PI(-thetahat);
			attitude.yaw = (float)fmod_2PI(-psihat+M_PI/2.0);
			mavlink_msg_attitude_encode(system_id, component_id, &msg, &attitude);
			fwrite_tlog_unix(TimeUS, msg, fileout);

			i++;
		}
		else if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%d;%d;%lf;%lf;%lf",
			&f0, &f1, &f2, &f3, &psihat, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23,
			&tv_sec, &tv_usec, &latitude, &longitude, &altitude_AMSL) == 29)
			&&(latitude != 0)&&(longitude != 0))
		{
			TimeUS = 1000000*(uint64_t)tv_sec+(uint64_t)tv_usec;
			if ((i%50) == 0)
			{
				// Send heartbeat regularly...
				mavlink_heartbeat_t heartbeat;
				memset(&heartbeat, 0, sizeof(heartbeat));
				heartbeat.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;

				heartbeat.type = MAV_TYPE_GENERIC;
				heartbeat.type = MAV_TYPE_SUBMARINE;
				heartbeat.type = MAV_TYPE_SURFACE_BOAT;
				heartbeat.type = MAV_TYPE_SURFACE_BOAT;
				heartbeat.type = MAV_TYPE_GROUND_ROVER;
				heartbeat.type = MAV_TYPE_QUADROTOR;
				heartbeat.type = 2; // Temp...

				//heartbeat.base_mode = 209;
				heartbeat.base_mode = MAV_MODE_FLAG_MANUAL_INPUT_ENABLED|MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | MAV_MODE_FLAG_AUTO_ENABLED;
				heartbeat.base_mode |= MAV_MODE_FLAG_SAFETY_ARMED;
				heartbeat.system_status = MAV_STATE_ACTIVE;

				mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
				fwrite_tlog_unix(TimeUS, msg, fileout);
			}

			mavlink_gps_raw_int_t gps_raw_int;
			memset(&gps_raw_int, 0, sizeof(gps_raw_int));
			gps_raw_int.time_usec = TimeUS;
			gps_raw_int.fix_type = (uint8_t)3;
			gps_raw_int.lat = (int32_t)(latitude*10000000.0);
			gps_raw_int.lon = (int32_t)(longitude*10000000.0);
			gps_raw_int.alt = (int32_t)(altitude_AMSL*1000.0);
			gps_raw_int.satellites_visible = 255;
			gps_raw_int.eph = UINT16_MAX;
			gps_raw_int.epv = UINT16_MAX;
			gps_raw_int.cog = UINT16_MAX;
			gps_raw_int.vel = UINT16_MAX;
			mavlink_msg_gps_raw_int_encode(system_id, component_id, &msg, &gps_raw_int);
			fwrite_tlog_unix(TimeUS, msg, fileout);
			
			mavlink_attitude_t attitude;
			memset(&attitude, 0, sizeof(attitude));
			attitude.time_boot_ms = (uint32_t)(TimeUS/1000);
			attitude.roll = (float)fmod_2PI(0);
			attitude.pitch = (float)fmod_2PI(0);
			attitude.yaw = (float)fmod_2PI(-psihat+M_PI/2.0);
			mavlink_msg_attitude_encode(system_id, component_id, &msg, &attitude);
			fwrite_tlog_unix(TimeUS, msg, fileout);

			i++;
		}
		else
		{
			printf("Skipping an invalid line in the CSV file.\n");
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
