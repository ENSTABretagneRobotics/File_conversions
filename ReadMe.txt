Waypoints2KML : convert a waypoints file Waypoints.csv in the Excel compatible waypoints file format used in the other programs of this folder (maximum of 256 waypoints, in decimal degrees, first column is latitude and second is longitude, column separator is ';' character and decimal separator is '.' character) to a Google Earth KML file. The reverse operation can be done using KML2Waypoints. Google Earth can be used to prepare the waypoints by placing Placemarks and saving My Places as Waypoints.kml.


Waypoints2mission : convert a waypoints file Waypoints.csv compatible with Waypoints2KML to a simple UxVCtrl mission file that would make the robot use line following to go through the waypoints.


mission2Waypoints : convert an UxVCtrl mission file containing waypoints-related commands to a waypoints file Waypoints.csv compatible with Waypoints2KML. Warning : no distinction is made for local waypoints, they should be converted manually...


ArduPilot2Waypoints : convert a waypoints file Waypoints.txt from Mission Planner to a waypoints file Waypoints.csv compatible with Waypoints2KML. The reverse operation can be done using Waypoints2Ardupilot.


logstate2TLOG : convert a log file containing data from logstate.csv from UxVCtrl program to a binary MAVLink .tlog file for easy replay in Mission Planner. Warning : some information are missing or extrapolated and there might be some confusion about the type of attitude and position (e.g. whether it comes from GPS, baro, IMU, or the result of a data fusion, whether it is relative to Home, etc.)...


dataflash2tlog : convert an ArduPilot DataFlash log from Mission Planner to a binary MAVLink .tlog file for easy replay in Mission Planner. Warning : some information are missing or extrapolated and there might be some confusion about the type of attitude and position (e.g. whether it comes from GPS, baro, IMU, or the result of a data fusion, whether it is relative to Home, etc.)...


GPSFileConverter : convert a waypoints file waypoints_deg_min.txt containting a list of waypoints expressed as

48 23.473500 N  4 25.752420 W

(latitude followed by longitude in decimal degrees and minutes each line, with a space between degrees and minutes and 2 spaces between latitude and longitude) to a waypoints file Waypoints.csv compatible with Waypoints2KML.
 The reverse operation can be done using GPSFileConverterInv.


LocalWaypoints2WGS/WGS2LocalWaypoints : need env.txt to convert waypoints expressed as 

10 5.5

(x,y position in m for each ligne, with a space between x and y) from a wplocal.txt file to a Waypoints.csv file compatible with Waypoints2KML, and conversely.


lognav2KML : convert a log file containing
_ data from lognav.csv from VAIMOS VCtrl program or
_ data from logstate.csv from UxVCtrl program or 
_ data from log.txt from ASLogger Android app or 
_ a list of positions in decimal degrees, first column is time, second column is latitude, third column is longitude, fourth column is altitude (optional), column separator is ';' character and decimal separator is '.' character
to a Google Earth KML file representing the trajectory. The input format and altitude mode need to be set in the defines in the beginning of the program.


NMEA2CSV : convert a raw NMEA data file lognav.txt to a log file compatible with lognav2KML. Before use, remove binary data with Notepad++ regex [^\x1F-\x7F]+ and replace with \r\n. A configuration file NMEA2CSV.txt can be used to change some parameters.


MT2KML : convert a MT Manager log file (using the provided specific log exporter settings, see SBG2MT) to a Google Earth KML file.


MT2NMEA : convert a MT Manager log file (using the provided specific log exporter settings, see SBG2MT) to a raw NMEA data file nmea.txt.


SBG2MT : convert a sbgCenter log file to a MT Manager log file (using the provided specific log exporter settings).


Mind the line endings in the configuration files depending on the OS (use e.g. the command dos2unix *.txt to convert line endings for Linux)!
