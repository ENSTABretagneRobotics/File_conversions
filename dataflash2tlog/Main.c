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
	mavlink_message_t msg;
	//uint8_t target_system = 1, target_component = 0;
	uint8_t system_id = 1, component_id = 1;
	uint64_t TimeUS = 0;
	unsigned int I = 0;
	unsigned int Status = 0, GMS = 0, GWk = 0, NSats = 0, U = 0;
	double HDop = 0, Lat = 0, Lng = 0, Alt_AMSL_GPS = 0, Spd = 0, GCrs = 0, VZ = 0, GPSYaw = 0;
	unsigned int Subsys = 0,ECode = 0;
	unsigned int Id = 0;
	BOOL bArmed = FALSE;
	int ModeNum = 0, Rsn = 0;
	double ThI = 0, ABst = 0, ThO = 0, ThH = 0, DAlt = 0, Alt = 0, BAlt = 0, DSAlt = 0, SAlt = 0, TAlt = 0, DCRt = 0, CRt = 0;
	double DPosX = 0, DPosY = 0, PosX = 0, PosY = 0, DVelX = 0, DVelY = 0, VelX = 0, VelY = 0, DAccX = 0, DAccY = 0;
	double Lat_POS = 0, Lng_POS = 0, Alt_POS = 0, RelHomeAlt = 0, RelOriginAlt = 0;
	unsigned int AEKF = 0;
	double DesRoll = 0, Roll = 0, DesPitch = 0, Pitch = 0, DesYaw = 0, Yaw = 0, ErrRP = 0, ErrYaw = 0;
	char Message[51];
	char Name[17];
	double Value = 0;
	double Alt_BARO = 0, Press = 0, Temp_BARO = 0, CRt_BARO = 0, Offset = 0, GndTemp = 0;
	unsigned int SMS = 0, Stat1 = 0;
	double Dist1 = 0, Orient1 = 0, Dist2 = 0, Orient2 = 0;
	unsigned int Qual = 0;
	double flowX = 0, flowY = 0, bodyX = 0, bodyY = 0;
	//double Q1 = 0, Q2 = 0, Q3 = 0, Q4 = 0;
	double VibeX = 0, VibeY = 0, VibeZ = 0;
	unsigned int Clip0 = 0, Clip1 = 0, Clip2 = 0;
	unsigned int CTot = 0, CNum = 0, CId = 0;
	double Prm1 = 0, Prm2 = 0, Prm3 = 0, Prm4 = 0, CLat = 0, CLng = 0, CAlt = 0;
	double Volt = 0, VoltR = 0, Curr = 0, CurrTot = 0, EnrgTot = 0, Temp = 0, Res = 0;
	double Vcc = 0, VServo = 0;
	unsigned int Flags = 0;
	unsigned int C1 = 0, C2 = 0, C3 = 0, C4 = 0, C5 = 0, C6 = 0, C7 = 0, C8 = 0, C9 = 0, C10 = 0, C11 = 0, C12 = 0, C13 = 0, C14 = 0;
	int MagX = 0, MagY = 0, MagZ = 0, Mag2X = 0, Mag2Y = 0, Mag2Z = 0, Mag3X = 0, Mag3Y = 0, Mag3Z = 0;
	double GyrX = 0, GyrY = 0, GyrZ = 0, AccX = 0, AccY = 0, AccZ = 0;
	double Gyr2X = 0, Gyr2Y = 0, Gyr2Z = 0, Acc2X = 0, Acc2Y = 0, Acc2Z = 0;
	double Gyr3X = 0, Gyr3Y = 0, Gyr3Z = 0, Acc3X = 0, Acc3Y = 0, Acc3Z = 0;

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
		printf("Usage : dataflash2tlog file.log.\n");
		printf("Default : dataflash2tlog %.255s.\n", szFileInPath);
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
		memset(Message, 0, sizeof(Message));
		memset(Name, 0, sizeof(Name));

/*
mavlink_nav_controller_output_t
mavlink_mission_current_t
mavlink_ekf_status_report_t
mavlink_ahrs2_t/mavlink_ahrs3_t???
*/

		//GPS, 182929586, 0, 6, 231688000, 2167, 23, 0.77, 48.4181976, -4.4719822, 94.57, 0.014, 116.528, 0.016, 181.4797, 1
		//GPS, 380997198, 3, 141473400, 1961, 10, 0.89, 48.4762186, -4.5440161, 119.53, 1.108, 198.187, -0.49, 1
		if ((sscanf(line, "GPS, %I64u, %u, %u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", 
			&TimeUS, &I, &Status, &GMS, &GWk, &NSats, &HDop, &Lat, &Lng, &Alt_AMSL_GPS, &Spd, &GCrs, &VZ, &GPSYaw, &U) == 15)||
			(sscanf(line, "GPS, %I64u, %u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %u", 
				&TimeUS, &Status, &GMS, &GWk, &NSats, &HDop, &Lat, &Lng, &Alt_AMSL_GPS, &Spd, &GCrs, &VZ, &U) == 13))
		{
			j++;
			mavlink_gps_raw_int_t gps_raw_int;
			memset(&gps_raw_int, 0, sizeof(gps_raw_int));
			gps_raw_int.time_usec = TimeUS;
			gps_raw_int.fix_type = (uint8_t)Status;
			gps_raw_int.lat = (int32_t)(Lat*10000000.0);
			gps_raw_int.lon = (int32_t)(Lng*10000000.0);
			gps_raw_int.alt = (int32_t)(Alt_AMSL_GPS*1000.0);
			gps_raw_int.satellites_visible = (uint8_t)NSats;
			gps_raw_int.eph = (uint16_t)(HDop*100);
			gps_raw_int.epv = UINT16_MAX;
			gps_raw_int.cog = (uint16_t)(GCrs*100.0);
			gps_raw_int.vel = (uint16_t)(Spd*100.0);
			mavlink_msg_gps_raw_int_encode(system_id, component_id, &msg, &gps_raw_int);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_system_time_t system_time;
			memset(&system_time, 0, sizeof(system_time));
			system_time.time_boot_ms = (uint32_t)(TimeUS/1000);
			system_time.time_unix_usec = (uint64_t)GPSTOW2UNIX(GMS, GWk);
			mavlink_msg_system_time_encode(system_id, component_id, &msg, &system_time);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//CMD, 325675152, 15, 2, 16, 10, 0, 0, 0, 48.47622, -4.544042, 25
		else if (sscanf(line, "CMD, %I64u, %u, %u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &CTot, &CNum, &CId, &Prm1, &Prm2, &Prm3, &Prm4, &CLat, &CLng, &CAlt) == 11)
		{
			j++;
/*			mavlink_command_long_t command_long;
			memset(&command_long, 0, sizeof(command_long));
			command_long.command = (uint16_t)CId;
			command_long.confirmation = 0;
			command_long.param1 = (float)Prm1;
			command_long.param2 = (float)Prm2;
			command_long.param3 = (float)Prm3;
			command_long.param4 = (float)Prm4;
			command_long.param5 = (float)CLat;
			command_long.param6 = (float)CLng;
			command_long.param7 = (float)CAlt;
			command_long.target_component = target_component;
			command_long.target_system = target_system;
			mavlink_msg_command_long_encode(system_id, component_id, &msg, &command_long);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
*/		}
		//ERR, 441502700, 24, 0
		else if (sscanf(line, "ERR, %I64u, %u, %u", &TimeUS, &Subsys, &ECode) == 3)
		{
			j++;
			// Check more ERR and EV to get "Bad LIDAR Health", "Error Compass Variance" messages...? Or maybe it is inferred from mavlink_ekf_status_report_t...? 
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
			(sscanf(line, "MODE, %I64u, Acro, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, AltHold, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Auto, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Guided, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Loiter, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, RTL, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Circle, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Land, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Drift, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Sport, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Flip, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, AutoTune, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, PosHold, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Brake, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Throw, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Avoid_ADSB, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, Guided_NoGPS, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3)||
			(sscanf(line, "MODE, %I64u, SmartRTL, %d, %d", &TimeUS, &ModeNum, &Rsn) == 3))
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
			heartbeat.type = 2; // Temp...

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
				heartbeat.system_status = MAV_STATE_STANDBY;
			}

			heartbeat.custom_mode = ModeNum;
			mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//CTUN, 63730420, 0, 0, 0, 0.2366303, 0, 0.04648887, 0.08, 0.00, 0.03, 0, 0, 1
		else if (sscanf(line, "CTUN, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &ThI, &ABst, &ThO, &ThH, &DAlt, &Alt, &BAlt, &DSAlt, &SAlt, &TAlt, &DCRt, &CRt) == 13)
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
			heartbeat.type = 2; // Temp...

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
				heartbeat.system_status = MAV_STATE_STANDBY;
			}

			heartbeat.custom_mode = ModeNum;
			mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_vfr_hud_t vfr_hud;
			memset(&vfr_hud, 0, sizeof(vfr_hud));
			vfr_hud.alt = (float)Alt;
			vfr_hud.heading = (int16_t)Yaw;
			vfr_hud.climb = (float)CRt;
			vfr_hud.groundspeed = (float)Spd;
			vfr_hud.throttle = (uint16_t)ThO;
			mavlink_msg_vfr_hud_encode(system_id, component_id, &msg, &vfr_hud);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//NTUN, 549071374, -206.6111, -76.6899, -235.0755, -81.34146, 27.78842, 4.615891, -64.85806, -10.16781, 322.2938, 6.05488
		else if (sscanf(line, "NTUN, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf",
			&TimeUS, &DPosX, &DPosY, &PosX, &PosY, &DVelX, &DVelY, &VelX, &VelY, &DAccX, &DAccY) == 11)
		{
			j++;
		}
		//POS, 380979773, 48.4762173, -4.5440273, 119.95, 24.19995, 26.45995
		else if (sscanf(line, "POS, %I64u, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Lat_POS, &Lng_POS, &Alt_POS, &RelHomeAlt, &RelOriginAlt) == 6)
		{
			j++;
			mavlink_global_position_int_t global_position_int;
			memset(&global_position_int, 0, sizeof(global_position_int));
			global_position_int.time_boot_ms = (uint32_t)(TimeUS/1000);
			global_position_int.lat = (int32_t)(Lat_POS*10000000.0);
			global_position_int.lon = (int32_t)(Lng_POS*10000000.0);
			global_position_int.alt = (int32_t)(Alt_POS*1000.0);
			global_position_int.relative_alt = (int32_t)(RelOriginAlt*1000);
			global_position_int.hdg = (uint16_t)Yaw;
			global_position_int.vx = (int16_t)VelX;
			global_position_int.vy = (int16_t)VelY;
			global_position_int.vz = (int16_t)(-VZ*100.0);
			mavlink_msg_global_position_int_encode(system_id, component_id, &msg, &global_position_int);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
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
			mavlink_attitude_t attitude;
			memset(&attitude, 0, sizeof(attitude));
			attitude.time_boot_ms = (uint32_t)(TimeUS/1000);
			attitude.roll = (float)fmod_2PI_deg2rad(Roll);
			attitude.pitch = (float)fmod_2PI_deg2rad(Pitch);
			attitude.yaw = (float)fmod_2PI_deg2rad(Yaw);
			mavlink_msg_attitude_encode(system_id, component_id, &msg, &attitude);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//AHR2, 380979746, 6.23, 4.26, 269.02, 120.72, 48.4762206, -4.544015, 0.6980689, 0.064643, -0.01272105, -0.7129932
/*		else if (sscanf(line, "AHR2, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Roll, &Pitch, &Yaw, &Alt, &Lat, &Lng, &Q1, &Q2, &Q3, &Q4) == 11)
		{
			j++;
		}
*/		else if (sscanf(line, "MSG, %I64u, %[^\r\n]50s", &TimeUS, &Message) == 2)
		{
			j++;
			mavlink_statustext_t statustext;
			memset(&statustext, 0, sizeof(statustext));
			memcpy(statustext.text, Message, sizeof(statustext.text)); // Not always NULL-terminated...
			statustext.severity = MAV_SEVERITY_INFO;
			mavlink_msg_statustext_encode(system_id, component_id, &msg, &statustext);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//PARM, 63455498, SYSID_SW_MREV, 120
		else if (sscanf(line, "PARM, %I64u, %16s, %lf", &TimeUS, &Name, &Value) == 3)
		{
			j++;
			mavlink_param_value_t param_value;
			memset(&param_value, 0, sizeof(param_value));
			memcpy(param_value.param_id, Name, sizeof(param_value.param_id)); // Not always NULL-terminated...
			param_value.param_value = (float)Value;
			param_value.param_type = MAV_PARAM_TYPE_REAL32;
			param_value.param_index = (uint16_t)(-1);
			mavlink_msg_param_value_encode(system_id, component_id, &msg, &param_value);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//BARO, 117509900, 0, 0.01978004, 100902, 40.84, 0.07505818, 117509, 0, 35, 1
		//BARO, 411727952, 17.52584, 100579.2, 25.55, -0.127921, 411727, 0, 32.89091
		else if ((sscanf(line, "BARO, %I64u, %u, %lf, %lf, %lf, %lf, %u, %lf, %lf", &TimeUS, &I, &Alt_BARO, &Press, &Temp_BARO, &CRt_BARO, &SMS, &Offset, &GndTemp) == 9)||
			(sscanf(line, "BARO, %I64u, %lf, %lf, %lf, %lf, %u, %lf, %lf", &TimeUS, &Alt_BARO, &Press, &Temp_BARO, &CRt_BARO, &SMS, &Offset, &GndTemp) == 8))
		{
			j++;
			mavlink_scaled_pressure_t scaled_pressure;
			memset(&scaled_pressure, 0, sizeof(scaled_pressure));
			scaled_pressure.time_boot_ms = (uint32_t)(TimeUS/1000);
			scaled_pressure.press_abs = (float)(Press/100.0);
			//scaled_pressure.press_diff = (float)Alt_BARO;
			scaled_pressure.temperature = (int16_t)(Temp_BARO*100.0);
			mavlink_msg_scaled_pressure_encode(system_id, component_id, &msg, &scaled_pressure);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//RFND,TimeUS,Instance,Dist,Stat,Orient
		else if (sscanf(line, "RFND, %I64u, %u, %lf, %u, %lf", &TimeUS, &I, &Dist1, &Stat1, &Orient1) == 5)
		{
			j++;
			mavlink_rangefinder_t rangefinder;
			memset(&rangefinder, 0, sizeof(rangefinder));
			rangefinder.distance = (float)(Dist1/100);
			rangefinder.voltage = (float)(Dist1/100);
			mavlink_msg_rangefinder_encode(system_id, component_id, &msg, &rangefinder);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//RFND, 381027073, 26.59, 25, 0.00, 0
		else if (sscanf(line, "RFND, %I64u, %lf, %lf, %lf, %lf", &TimeUS, &Dist1, &Orient1, &Dist2, &Orient2) == 5)
		{
			j++;
			mavlink_rangefinder_t rangefinder;
			memset(&rangefinder, 0, sizeof(rangefinder));
			rangefinder.distance = (float)Dist1;
			rangefinder.voltage = (float)Dist1;
			mavlink_msg_rangefinder_encode(system_id, component_id, &msg, &rangefinder);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//OF, 144539588, 255, -0.05597964, 0.3796437, -0.008142494, 0.01424936
		else if (sscanf(line, "OF, %I64u, %u, %lf, %lf, %lf, %lf", &TimeUS, &Qual, &flowX, &flowY, &bodyX, &bodyY) == 6)
		{
			j++;
			mavlink_optical_flow_t optical_flow;
			memset(&optical_flow, 0, sizeof(optical_flow));
			optical_flow.time_usec = TimeUS;
			optical_flow.quality = (uint8_t)Qual;
			optical_flow.flow_comp_m_x = (float)flowX; // To check...
			optical_flow.flow_comp_m_y = (float)flowY; // To check...
			mavlink_msg_optical_flow_encode(system_id, component_id, &msg, &optical_flow);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_optical_flow_rad_t optical_flow_rad;
			memset(&optical_flow_rad, 0, sizeof(optical_flow_rad));
			optical_flow_rad.time_usec = TimeUS;
			optical_flow_rad.quality = (uint8_t)Qual;
			optical_flow_rad.integrated_x = (float)flowX; // To check...
			optical_flow_rad.integrated_y = (float)flowY; // To check...
			mavlink_msg_optical_flow_rad_encode(system_id, component_id, &msg, &optical_flow_rad);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//VIBE, 194910152, 0, 0.01745942, 0.01978292, 0.01973939, 9
		//VIBE, 380979906, 25.80228, 38.46054, 44.74593, 0, 90, 0
		else if ((sscanf(line, "VIBE, %I64u, %u, %lf, %lf, %lf, %u", &TimeUS, &I, &VibeX, &VibeY, &VibeZ, &Clip0) == 6)||
			(sscanf(line, "VIBE, %I64u, %lf, %lf, %lf, %u, %u, %u", &TimeUS, &VibeX, &VibeY, &VibeZ, &Clip0, &Clip1, &Clip2) == 7))
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
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//CURR, 563347566, 23.06225, 2.736309, 218.271, 0.00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		//BAT, 144573364, 13.23588, 14.95658, 0, 0.001671574, 2.478911E-05, 0, 28.04165
		else if ((sscanf(line, "CURR, %I64u, %lf, %lf, %lf, %lf", &TimeUS, &Volt, &Curr, &CurrTot, &Temp) == 5)||
		(sscanf(line, "BAT, %I64u, %u, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &I, &Volt, &VoltR, &Curr, &CurrTot, &EnrgTot, &Temp, &Res) == 9)||
		(sscanf(line, "BAT, %I64u, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Volt, &VoltR, &Curr, &CurrTot, &EnrgTot, &Temp, &Res) == 8))
		{
			j++;
			mavlink_battery_status_t battery_status;
			memset(&battery_status, 0, sizeof(battery_status));
			battery_status.current_consumed = (int32_t)CurrTot;
			battery_status.energy_consumed = -1;
			battery_status.temperature =  INT16_MAX;
			memset(&battery_status.voltages, UINT16_MAX, sizeof(battery_status.voltages));
			// Try to guess the number of cells...
			int nb_cells = (int)round(Volt/3.7);
			int k = 0;
			for (k = 0; k < nb_cells; k++)
			{
				battery_status.voltages[k] = (uint16_t)(Volt*1000.0/nb_cells);
			}
			battery_status.current_battery = (int16_t)(Curr*100.0);
			battery_status.id = 0;
			battery_status.battery_function = 0;
			battery_status.type = 0;
			battery_status.battery_remaining = -1;
			mavlink_msg_battery_status_encode(system_id, component_id, &msg, &battery_status);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_sys_status_t sys_status;
			memset(&sys_status, 0, sizeof(sys_status));
			sys_status.voltage_battery = (uint16_t)(Volt*1000.0);
			sys_status.current_battery = (int16_t)(Curr*100.0);
			sys_status.battery_remaining = -1;
			sys_status.onboard_control_sensors_present = 56753455; // Temp...
			sys_status.onboard_control_sensors_enabled = 56753455; // Temp...
			sys_status.onboard_control_sensors_health = 56753455; // Temp...
			mavlink_msg_sys_status_encode(system_id, component_id, &msg, &sys_status);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//POWR, 63727810, 5.15625, 5.092, 3
		else if (sscanf(line, "POWR, %I64u, %lf, %lf, %u", &TimeUS, &Vcc, &VServo, &Flags) == 4)
		{
			j++;
			mavlink_power_status_t power_status;
			memset(&power_status, 0, sizeof(power_status));
			power_status.Vcc = (uint16_t)(Vcc*1000.0);
			power_status.Vservo = (uint16_t)(VServo*1000.0);
			power_status.flags = (uint16_t)Flags;
			mavlink_msg_power_status_encode(system_id, component_id, &msg, &power_status);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_hwstatus_t hwstatus;
			memset(&hwstatus, 0, sizeof(hwstatus));
			hwstatus.Vcc = (uint16_t)(Vcc*1000.0);
			mavlink_msg_hwstatus_encode(system_id, component_id, &msg, &hwstatus);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//RCIN, 64030866, 1493, 1502, 988, 2011, 1499, 1499, 1499, 1500, 0, 0, 0, 0, 0, 0
		else if (sscanf(line, "RCIN, %I64u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u", &TimeUS, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &C8, &C9, &C10, &C11, &C12, &C13, &C14) == 15)
		{
			j++;
			mavlink_rc_channels_t rc_channels;
			memset(&rc_channels, 0, sizeof(rc_channels));
			rc_channels.time_boot_ms = (uint32_t)(TimeUS/1000);
			rc_channels.chancount = 8; // Temp...
			rc_channels.chan1_raw = (uint16_t)C1;
			rc_channels.chan2_raw = (uint16_t)C2;
			rc_channels.chan3_raw = (uint16_t)C3;
			rc_channels.chan4_raw = (uint16_t)C4;
			rc_channels.chan5_raw = (uint16_t)C5;
			rc_channels.chan6_raw = (uint16_t)C6;
			rc_channels.chan7_raw = (uint16_t)C7;
			rc_channels.chan8_raw = (uint16_t)C8;
			rc_channels.chan9_raw = (uint16_t)C9;
			rc_channels.chan10_raw = (uint16_t)C10;
			rc_channels.chan11_raw = (uint16_t)C11;
			rc_channels.chan12_raw = (uint16_t)C12;
			rc_channels.chan13_raw = (uint16_t)C13;
			rc_channels.chan14_raw = (uint16_t)C14;
			rc_channels.chan15_raw = 0;
			rc_channels.chan16_raw = 0;
			rc_channels.rssi = 255;
			mavlink_msg_rc_channels_encode(system_id, component_id, &msg, &rc_channels);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
			mavlink_rc_channels_raw_t rc_channels_raw;
			memset(&rc_channels_raw, 0, sizeof(rc_channels_raw));
			rc_channels_raw.time_boot_ms = (uint32_t)(TimeUS/1000);
			rc_channels_raw.port = 1; // Temp...
			rc_channels_raw.chan1_raw = (uint16_t)C1;
			rc_channels_raw.chan2_raw = (uint16_t)C2;
			rc_channels_raw.chan3_raw = (uint16_t)C3;
			rc_channels_raw.chan4_raw = (uint16_t)C4;
			rc_channels_raw.chan5_raw = (uint16_t)C5;
			rc_channels_raw.chan6_raw = (uint16_t)C6;
			rc_channels_raw.chan7_raw = (uint16_t)C7;
			rc_channels_raw.chan8_raw = (uint16_t)C8;
			rc_channels_raw.rssi = 255;
			mavlink_msg_rc_channels_raw_encode(system_id, component_id, &msg, &rc_channels_raw);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//RCOU, 64030938, 987, 987, 987, 987, 987, 987, 0, 0, 0, 0, 0, 0, 0, 0
		else if (sscanf(line, "RCOU, %I64u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u", &TimeUS, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &C8, &C9, &C10, &C11, &C12, &C13, &C14) == 15)
		{
			j++;
			mavlink_servo_output_raw_t servo_output_raw;
			memset(&servo_output_raw, 0, sizeof(servo_output_raw));
			servo_output_raw.time_usec = (uint32_t)TimeUS;
			servo_output_raw.port = 1; // Temp...
			servo_output_raw.servo1_raw = (uint16_t)C1;
			servo_output_raw.servo2_raw = (uint16_t)C2;
			servo_output_raw.servo3_raw = (uint16_t)C3;
			servo_output_raw.servo4_raw = (uint16_t)C4;
			servo_output_raw.servo5_raw = (uint16_t)C5;
			servo_output_raw.servo6_raw = (uint16_t)C6;
			servo_output_raw.servo7_raw = (uint16_t)C7;
			servo_output_raw.servo8_raw = (uint16_t)C8;
			mavlink_msg_servo_output_raw_encode(system_id, component_id, &msg, &servo_output_raw);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		//MAG, 194709930, 0, -209, -75, 411, 0, 0, 0, 0, 0, 0, 1, 194709926
		//MAG, 411625728, -53, 150, 369, 1, 17, -77, 0, 0, 0, 1, 411625723
		else if ((sscanf(line, "MAG, %I64u, %u, %d, %d, %d", &TimeUS, &I, &MagX, &MagY, &MagZ) == 5)||
			(sscanf(line, "MAG, %I64u, %d, %d, %d", &TimeUS, &MagX, &MagY, &MagZ) == 4))
		{
			j++;
		}
		else if (sscanf(line, "MAG2, %I64u, %d, %d, %d", &TimeUS, &Mag2X, &Mag2Y, &Mag2Z) == 4)
		{
			j++;
		}
		else if (sscanf(line, "MAG3, %I64u, %d, %d, %d", &TimeUS, &Mag3X, &Mag3Y, &Mag3Z) == 4)
		{
			j++;
		}
		//IMU, 182929178, 0, 0.002522397, -0.001043871, -0.0002923453, 0.3591292, 0.1684806, -10.07592, 0, 0, 43.86654, 1, 1, 8019, 4009
		//IMU, 411575125, 0.2243799, 0.061694, -0.3788114, -0.7872514, 0.6289721, -10.71861, 0, 0, 24.81149, 1, 1, 999, 999
		else if ((sscanf(line, "IMU, %I64u, %u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &I, &GyrX, &GyrY, &GyrZ, &AccX, &AccY, &AccZ) == 8)||
			(sscanf(line, "IMU, %I64u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &GyrX, &GyrY, &GyrZ, &AccX, &AccY, &AccZ) == 7))
		{
			j++;
			mavlink_raw_imu_t raw_imu; // Why raw for the first IMU and scaled for the second one...?
			memset(&raw_imu, 0, sizeof(raw_imu));
			raw_imu.time_usec = (uint64_t)TimeUS;
			raw_imu.xacc = (int16_t)(AccX*1000.0/STANDARD_GRAVITY);
			raw_imu.yacc = (int16_t)(AccY*1000.0/STANDARD_GRAVITY);
			raw_imu.zacc = (int16_t)(AccZ*1000.0/STANDARD_GRAVITY);
			raw_imu.xgyro = (int16_t)(GyrX*1000.0);
			raw_imu.ygyro = (int16_t)(GyrY*1000.0);
			raw_imu.zgyro = (int16_t)(GyrZ*1000.0);
			raw_imu.xmag = (int16_t)MagX;
			raw_imu.ymag = (int16_t)MagY;
			raw_imu.zmag = (int16_t)MagZ;
			mavlink_msg_raw_imu_encode(system_id, component_id, &msg, &raw_imu);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		else if (sscanf(line, "IMU2, %I64u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Gyr2X, &Gyr2Y, &Gyr2Z, &Acc2X, &Acc2Y, &Acc2Z) == 7)
		{
			j++;
			mavlink_scaled_imu2_t scaled_imu2;
			memset(&scaled_imu2, 0, sizeof(scaled_imu2));
			scaled_imu2.time_boot_ms = (uint32_t)(TimeUS/1000);
			scaled_imu2.xacc = (int16_t)(Acc2X*1000.0/STANDARD_GRAVITY);
			scaled_imu2.yacc = (int16_t)(Acc2Y*1000.0/STANDARD_GRAVITY);
			scaled_imu2.zacc = (int16_t)(Acc2Z*1000.0/STANDARD_GRAVITY);
			scaled_imu2.xgyro = (int16_t)(Gyr2X*1000.0);
			scaled_imu2.ygyro = (int16_t)(Gyr2Y*1000.0);
			scaled_imu2.zgyro = (int16_t)(Gyr2Z*1000.0);
			scaled_imu2.xmag = (int16_t)Mag2X;
			scaled_imu2.ymag = (int16_t)Mag2Y;
			scaled_imu2.zmag = (int16_t)Mag2Z;
			mavlink_msg_scaled_imu2_encode(system_id, component_id, &msg, &scaled_imu2);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
			i++;
		}
		else if (sscanf(line, "IMU3, %I64u, %lf, %lf, %lf, %lf, %lf, %lf", &TimeUS, &Gyr3X, &Gyr3Y, &Gyr3Z, &Acc3X, &Acc3Y, &Acc3Z) == 7)
		{
			j++;
			mavlink_scaled_imu3_t scaled_imu3;
			memset(&scaled_imu3, 0, sizeof(scaled_imu3));
			scaled_imu3.time_boot_ms = (uint32_t)(TimeUS/1000);
			scaled_imu3.xacc = (int16_t)(Acc3X*1000.0/STANDARD_GRAVITY);
			scaled_imu3.yacc = (int16_t)(Acc3Y*1000.0/STANDARD_GRAVITY);
			scaled_imu3.zacc = (int16_t)(Acc3Z*1000.0/STANDARD_GRAVITY);
			scaled_imu3.xgyro = (int16_t)(Gyr3X*1000.0);
			scaled_imu3.ygyro = (int16_t)(Gyr3Y*1000.0);
			scaled_imu3.zgyro = (int16_t)(Gyr3Z*1000.0);
			scaled_imu3.xmag = (int16_t)Mag3X;
			scaled_imu3.ymag = (int16_t)Mag3Y;
			scaled_imu3.zmag = (int16_t)Mag3Z;
			mavlink_msg_scaled_imu3_encode(system_id, component_id, &msg, &scaled_imu3);
			fwrite_tlog_gpstow(GMS, GWk, msg, fileout);
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
