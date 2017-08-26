// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

#ifdef _MSC_VER
// Disable some Visual Studio warnings.
#pragma warning(disable : 4201) 
#pragma warning(disable : 4244) 
#pragma warning(disable : 4459) 
#pragma warning(disable : 4214) 
#endif // _MSC_VER
#include "mavlink/common/mavlink.h"
#include "mavlink/ardupilotmega/mavlink_msg_rangefinder.h"
#ifdef _MSC_VER
// Restore the Visual Studio warnings previously disabled.
#pragma warning(default : 4214) 
#pragma warning(default : 4459) 
#pragma warning(default : 4244) 
#pragma warning(default : 4201) 
#endif // _MSC_VER

/*
	// From Mission Planner...
	// Should be nb us since 1970...?

	int tem = logplaybackfile.BaseStream.Read(datearray, 0, datearray.Length);

	Array.Reverse(datearray);

	DateTime date1 = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);

	UInt64 dateint = BitConverter.ToUInt64(datearray, 0);

	try
	{
		// array is reversed above
		if (datearray[7] == 254 || datearray[7] == 253)
		{
			//rewind 8bytes
			logplaybackfile.BaseStream.Seek(-8, SeekOrigin.Current);
		}
		else
		{
			if ((dateint/1000/1000/60/60) < 9999999)
			{
				date1 = date1.AddMilliseconds(dateint/1000);

				lastlogread = date1.ToLocalTime();
			}
		}
	}
*/

// Convert GPS time of week and week number to UNIX time in ms.
inline uint64_t GPSTOW2UNIX(unsigned int GMS, unsigned int GWk)
{	
	uint64_t datearray = 0;
	int y = 1980, m = 1, d = 6;
	struct tm t;
	time_t tt;

	memset(&t, 0, sizeof(t));
	t.tm_year = y-1900;
	t.tm_mon = m-1;
	t.tm_mday = d+GWk*7;
	t.tm_isdst = 0;
	tt = timegm(&t);
	datearray = (uint64_t)GMS*1000+(uint64_t)tt*(uint64_t)1000000;

	//datearray = (uint64_t)GMS*1000+(uint64_t)GWk*7*24*3600*1000000;
	//datearray = TimeUS+time_tmp*1000000;

	return datearray;
}

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szTemp[256];
	char szName[256];
	char line[4096];
	unsigned int i = 0, j = 0;
	unsigned char sendbuf[256];
	int sendbuflen = 0;
	mavlink_message_t msg;
	//uint8_t target_system = 1, target_component = 0;
	uint8_t system_id = 1, component_id = 1;
	uint64_t TimeUS = 0, datearray = 0;
	unsigned int Id;
	BOOL bArmed = FALSE;
	unsigned int Status, GMS, GWk, NSats, U;
	double HDop, Lat, Lng, Alt_AMSL_GPS, Spd, GCrs, VZ;
	double ThI, ABst, ThO, ThH, DAlt, Alt, BAlt, DSAlt, SAlt, TAlt, DCRt, CRt;
	double DesRoll, Roll, DesPitch, Pitch, DesYaw, Yaw, ErrRP, ErrYaw;
	double Lat_POS, Lng_POS, Alt_POS, RelHomeAlt, RelOriginAlt;
	double VibeX, VibeY, VibeZ;
	unsigned int Clip0, Clip1, Clip2;
	char Message[64];
	int ModeNum, Rsn;
	double Dist1, Dist2;
	unsigned int Orient1, Orient2;
	//double Q1, Q2, Q3, Q4;

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
		printf("Usage : log2tlog file.log.\n");
		printf("Default : log2tlog %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.tlog", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

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

	printf("Converting...\n");

	i = 0;
	j = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{


/*
mavlink_nav_controller_output_t
mavlink_mission_current_t
mavlink_system_time_t
mavlink_battery_status_t
mavlink_rc_channels_t
mavlink_ahrs2_t/mavlink_ahrs3_t???
mavlink_ekf_status_report_t
mavlink_power_status_t
*/


		//GPS, 380997198, 3, 141473400, 1961, 10, 0.89, 48.4762186, -4.5440161, 119.53, 1.108, 198.187, -0.49, 1
		if (sscanf(line, "GPS, %I64u, %u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", &TimeUS, &Status, &GMS, &GWk, &NSats, &HDop, &Lat, &Lng, &Alt_AMSL_GPS, &Spd, &GCrs, &VZ, &U) == 13)
		{
			j++;
			mavlink_gps_raw_int_t gps_raw_int;
			memset(&gps_raw_int, 0, sizeof(gps_raw_int));
			gps_raw_int.fix_type = (uint8_t)Status;
			gps_raw_int.lat = (int32_t)(Lat*10000000.0);
			gps_raw_int.lon = (int32_t)(Lng*10000000.0);
			gps_raw_int.alt = (int32_t)(Alt_AMSL_GPS*1000.0);
			gps_raw_int.satellites_visible = (uint8_t)NSats;
			gps_raw_int.eph = UINT16_MAX;
			gps_raw_int.epv = UINT16_MAX;
			gps_raw_int.cog = (uint16_t)(GCrs*100.0);
			gps_raw_int.vel = (uint16_t)(Spd*100.0);
			mavlink_msg_gps_raw_int_encode(system_id, component_id, &msg, &gps_raw_int);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//EV, 563550628, 18
		else if (sscanf(line, "EV, %I64u, %u", &TimeUS, &Id) == 2)
		{
			j++;
			switch (Id)
			{
			case 10:
				bArmed = TRUE;
				break;
			case 11:
				bArmed = FALSE;
				break;
			default:
				break;
			}
		}
		//MODE, 63460340, Stabilize, 0, 1
		else if (
			(sscanf(line, "MODE, %I64u, Stabilize, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Land, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, AltHold, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Guided, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Auto, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, RTL, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, AutoTune, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Loiter, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, PosHold, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Circle, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Acro, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3))
		{
			j++;
			mavlink_heartbeat_t heartbeat;
			memset(&heartbeat, 0, sizeof(heartbeat));
			heartbeat.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;

			heartbeat.type = MAV_TYPE_GENERIC;
			heartbeat.type = MAV_TYPE_SUBMARINE;
			heartbeat.type = MAV_TYPE_SURFACE_BOAT;
			heartbeat.type = MAV_TYPE_SURFACE_BOAT;
			heartbeat.type = MAV_TYPE_GROUND_ROVER;
			heartbeat.type = MAV_TYPE_QUADROTOR;
			heartbeat.type = 2;

			//heartbeat.base_mode = 209;
			heartbeat.base_mode = MAV_MODE_FLAG_MANUAL_INPUT_ENABLED|MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
			switch (ModeNum)
			{
			case 0:
				heartbeat.base_mode |= MAV_MODE_FLAG_STABILIZE_ENABLED;
				break;
			case 3:
				heartbeat.base_mode |= MAV_MODE_FLAG_AUTO_ENABLED;
				break;
			case 4:
				heartbeat.base_mode |= MAV_MODE_FLAG_GUIDED_ENABLED;
				break;
			default:
				break;
			}
			if (bArmed)
			{
				heartbeat.base_mode |= MAV_MODE_FLAG_SAFETY_ARMED;
				heartbeat.system_status = MAV_STATE_ACTIVE;
			}
			else
			{
				heartbeat.base_mode |= MAV_MODE_FLAG_SAFETY_ARMED;
				heartbeat.system_status = MAV_STATE_STANDBY;
			}

			heartbeat.custom_mode = ModeNum;
			mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//CTUN, 63730420, 0, 0, 0, 0.2366303, 0, 0.04648887, 0.08, 0.00, 0.03, 0, 0, 1
		else if (sscanf(line, "CTUN, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &ThI, &ABst, &ThO, &ThH, &DAlt, &Alt, &BAlt, &DSAlt, &SAlt, &TAlt, &DCRt, &CRt) == 13)
		{
			j++;
		}
		//POS, 380979773, 48.4762173, -4.5440273, 119.95, 24.19995, 26.45995
		else if (sscanf(line, "POS, %I64u, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Lat_POS, &Lng_POS, &Alt_POS, &RelHomeAlt, &RelOriginAlt) == 6)
		{
			j++;
			mavlink_vfr_hud_t vfr_hud;
			memset(&vfr_hud, 0, sizeof(vfr_hud));
			vfr_hud.alt = (float)Alt;
			vfr_hud.heading = (int16_t)Yaw;
			vfr_hud.climb = (float)CRt;
			vfr_hud.groundspeed = (float)Spd;
			vfr_hud.throttle = (uint16_t)ThO;
			mavlink_msg_vfr_hud_encode(system_id, component_id, &msg, &vfr_hud);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
			mavlink_global_position_int_t global_position_int;
			memset(&global_position_int, 0, sizeof(global_position_int));
			global_position_int.time_boot_ms = (uint32_t)(TimeUS/1000);
			global_position_int.lat = (int32_t)(Lat_POS*10000000.0);
			global_position_int.lon = (int32_t)(Lng_POS*10000000.0);
			global_position_int.alt = (int32_t)(Alt_POS*1000.0);
			global_position_int.relative_alt = (int32_t)(RelOriginAlt*1000);
			global_position_int.hdg = (uint16_t)Yaw;
			global_position_int.vx = 0;
			global_position_int.vy = 0;
			global_position_int.vz = 0;
			mavlink_msg_global_position_int_encode(system_id, component_id, &msg, &global_position_int);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//ATT, 380979385, 3.39, 4.97, -3.56, 0.41, 263.21, 268.16, 0.08, 0.05
		else if (sscanf(line, "ATT, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &DesRoll, &Roll, &DesPitch, &Pitch, &DesYaw, &Yaw, &ErrRP, &ErrYaw) == 9)
		{
			j++;
			mavlink_attitude_t attitude;
			memset(&attitude, 0, sizeof(attitude));
			attitude.time_boot_ms = (uint32_t)(TimeUS/1000);
			attitude.roll = (float)fmod_2PI_deg2rad(Roll);
			attitude.pitch = (float)fmod_2PI_deg2rad(Pitch);
			attitude.yaw = (float)fmod_2PI_deg2rad(Yaw);
			mavlink_msg_attitude_encode(system_id, component_id, &msg, &attitude);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//AHR2, 380979746, 6.23, 4.26, 269.02, 120.72, 48.4762206, -4.544015, 0.6980689, 0.064643, -0.01272105, -0.7129932
/*		else if (sscanf(line, "AHR2, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Roll, &Pitch, &Yaw, &Alt, &Lat, &Lng, &Q1, &Q2, &Q3, &Q4) == 11)
		{
			j++;
			mavlink_attitude_t attitude;
			memset(&attitude, 0, sizeof(attitude));
			attitude.time_boot_ms = (uint32_t)(TimeUS/1000);
			attitude.roll = (float)fmod_2PI_deg2rad(Roll);
			attitude.pitch = (float)fmod_2PI_deg2rad(Pitch);
			attitude.yaw = (float)fmod_2PI_deg2rad(Yaw);
			mavlink_msg_attitude_encode(system_id, component_id, &msg, &attitude);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
*/		else if (sscanf(line, "MSG, %I64u, %[^\r\n]63s", &TimeUS, &Message) == 2)
		{
			j++;
			mavlink_statustext_t statustext;
			memset(&statustext, 0, sizeof(statustext));
			strcpy(statustext.text, Message);
			statustext.severity = MAV_SEVERITY_INFO;
			mavlink_msg_statustext_encode(system_id, component_id, &msg, &statustext);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//RFND, 381027073, 26.59, 25, 0.00, 0
		else if (sscanf(line, "RFND, %I64u, %lf, %u, %lf, %u", &TimeUS, &Dist1, &Orient1, &Dist2, &Orient2) == 5)
		{
			j++;
			mavlink_rangefinder_t rangefinder;
			memset(&rangefinder, 0, sizeof(rangefinder));
			rangefinder.distance = (float)Dist1;
			rangefinder.voltage = (float)Dist1;
			mavlink_msg_rangefinder_encode(system_id, component_id, &msg, &rangefinder);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		//VIBE, 380979906, 25.80228, 38.46054, 44.74593, 0, 90, 0
		else if (sscanf(line, "VIBE, %I64u, %lf, %lf, %lf, %u, %u, %u", &TimeUS, &VibeX, &VibeY, &VibeZ, &Clip0, &Clip1, &Clip2) == 7)
		{
			j++;
			mavlink_vibration_t vibration;
			memset(&vibration, 0, sizeof(vibration));
			vibration.time_usec = TimeUS;
			vibration.vibration_x = (float)VibeX;
			vibration.vibration_y = (float)VibeY;
			vibration.vibration_z = (float)VibeZ;
			vibration.clipping_0 = Clip0;
			vibration.clipping_1 = Clip1;
			vibration.clipping_2 = Clip2;
			mavlink_msg_vibration_encode(system_id, component_id, &msg, &vibration);
			memset(sendbuf, 0, sizeof(sendbuf));
			sendbuflen = mavlink_msg_to_send_buffer(sendbuf, &msg);
			datearray = GPSTOW2UNIX(GMS, GWk);
			SwapBytes((unsigned char*)&datearray, 8);
			fwrite(&datearray, 1, 8, fileout);
			fwrite(sendbuf, 1, sendbuflen, fileout);
			i++;
		}
		else
		{
			//printf("Skipping an invalid line in the input file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Converted %u DataFlash messages to %u MAVLink messages.\n", j, i);

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
